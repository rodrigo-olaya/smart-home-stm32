#include <stdint.h>

// GM65 barcode scanner bring-up, polling only (no interrupts, no PC link).
//
// Wiring (J7 on the carrier board):
//   pin 1 5V   pin 2 GND   pin 3 PA9 (USART1 TX)   pin 4 PA10 (USART1 RX)
//   GM65 RX <- PA9      GM65 TX -> PA10      (TX and RX cross)
// USART1 is 9600 baud, 8N1: the GM65's factory setting.
//
// There is no console, so the three LEDs are the readout:
//   PC13 (Black Pill LED)  toggles on every byte received from the GM65.
//                          Any flicker means the GM65 -> PA10 wire works.
//   D1 (PB7, target)       comes on once the GM65 answers a command.
//                          That proves power, both signal wires, and baud.
//   D2 (PB8, approach)     after a scan: on for 1 s if the data was all digits,
//                          3 quick blinks if it held anything else (garbage
//                          usually means the wrong baud rate).
//
// Steps:
//   1. Boot: all LEDs light for 0.3 s (firmware is alive).
//   2. Every second it asks the GM65 to read its config zone 0x0000 (read-only).
//      Wait for D1. If D1 never comes on: swapped TX/RX is the usual cause,
//      then 5V/GND, then a bad crimp.
//   3. Scan something. If the GM65 is in a mode where it scans by itself
//      (continuous / sensing), D2 lights. If not, press the Black Pill KEY
//      button (PA0): it sends the start-decoding command.
//
// The command bytes come from the GM65 serial protocol: 7E 00 <type> <len>
// <addr hi> <addr lo> <data> <crc hi> <crc lo>, type 07 = read, 08 = write,
// and CRC AB CD means "don't check the CRC". CHECK THEM AGAINST YOUR MANUAL.
// A good reply is 7 bytes starting 02 00 00.
//
// TIM2 free-runs at 1 MHz (HSI 16 MHz / 16), so one count = 1 us.

#define RCC_AHB1ENR   (*(volatile uint32_t *)0x40023830)
#define RCC_APB1ENR   (*(volatile uint32_t *)0x40023840)
#define RCC_APB2ENR   (*(volatile uint32_t *)0x40023844)

#define GPIOA_MODER   (*(volatile uint32_t *)0x40020000)
#define GPIOA_PUPDR   (*(volatile uint32_t *)0x4002000C)
#define GPIOA_IDR     (*(volatile uint32_t *)0x40020010)
#define GPIOA_AFRH    (*(volatile uint32_t *)0x40020024)
#define GPIOB_MODER   (*(volatile uint32_t *)0x40020400)
#define GPIOB_BSRR    (*(volatile uint32_t *)0x40020418)
#define GPIOC_MODER   (*(volatile uint32_t *)0x40020800)
#define GPIOC_ODR     (*(volatile uint32_t *)0x40020814)
#define GPIOC_BSRR    (*(volatile uint32_t *)0x40020818)

#define TIM2_CR1      (*(volatile uint32_t *)0x40000000)
#define TIM2_EGR      (*(volatile uint32_t *)0x40000014)
#define TIM2_CNT      (*(volatile uint32_t *)0x40000024)
#define TIM2_PSC      (*(volatile uint32_t *)0x40000028)
#define TIM2_ARR      (*(volatile uint32_t *)0x4000002C)

#define USART1_SR     (*(volatile uint32_t *)0x40011000)
#define USART1_DR     (*(volatile uint32_t *)0x40011004)
#define USART1_BRR    (*(volatile uint32_t *)0x40011008)
#define USART1_CR1    (*(volatile uint32_t *)0x4001100C)

#define SR_RXNE       (1u << 5)     // a byte is waiting in DR
#define SR_TC         (1u << 6)     // last byte has fully left the pin
#define SR_TXE        (1u << 7)     // DR is free for the next byte

#define TARGET_PIN    7             // PB7, D1
#define APPROACH_PIN  8             // PB8, D2
#define BOARD_LED_PIN 13            // PC13, LOW = on
#define KEY_PIN       0             // PA0, Black Pill KEY button, LOW = pressed

// 16 MHz / 9600 = 1666.67. BRR holds that number directly when OVER8 = 0
// (mantissa 104 in bits 15:4, fraction 3/16 in bits 3:0), rounded up to 1667.
#define BRR_9600      1667u

static const uint8_t CMD_READ_ZONE0[] = {0x7E, 0x00, 0x07, 0x01, 0x00, 0x00, 0x01, 0xAB, 0xCD};
static const uint8_t CMD_START_SCAN[] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, 0xAB, 0xCD};

static void mode_output(volatile uint32_t *moder, int pin) {
    *moder &= ~(3u << (pin * 2));
    *moder |=  (1u << (pin * 2));
}

static void d1(int on)    { GPIOB_BSRR = on ? (1u << TARGET_PIN)   : (1u << (TARGET_PIN + 16)); }
static void d2(int on)    { GPIOB_BSRR = on ? (1u << APPROACH_PIN) : (1u << (APPROACH_PIN + 16)); }
static void board(int on) { GPIOC_BSRR = on ? (1u << (BOARD_LED_PIN + 16)) : (1u << BOARD_LED_PIN); }

static void delay_us(uint32_t us) {
    uint32_t start = TIM2_CNT;
    while ((TIM2_CNT - start) < us) { }
}

static void uart_send(const uint8_t *buf, int n) {
    for (int i = 0; i < n; i++) {
        while (!(USART1_SR & SR_TXE)) { }
        USART1_DR = buf[i];
    }
    while (!(USART1_SR & SR_TC)) { }
}

// One received byte (0..255), or -1 if nothing arrives within timeout_us.
// Reading SR and then DR is also what clears an overrun error.
static int uart_getc(uint32_t timeout_us) {
    uint32_t t0 = TIM2_CNT;
    while ((TIM2_CNT - t0) < timeout_us) {
        if (USART1_SR & SR_RXNE) {
            GPIOC_ODR ^= (1u << BOARD_LED_PIN);         // PC13: a byte arrived
            return (int)(USART1_DR & 0xFF);
        }
    }
    return -1;
}

// Throw away anything already received, so a reply isn't mixed with old bytes.
static void uart_flush(void) {
    while (USART1_SR & SR_RXNE) { (void)USART1_DR; }
}

// Both commands used here are answered with a 7-byte frame: 02 00 00 01 <data> <crc> <crc>.
static int read_reply(void) {
    uint8_t r[7];
    int n = 0;
    while (n < 7) {
        int c = uart_getc(200000);                      // 200 ms per byte
        if (c < 0) break;
        r[n++] = (uint8_t)c;
    }
    return n == 7 && r[0] == 0x02 && r[1] == 0x00 && r[2] == 0x00;
}

static void blink_d2(int times) {
    for (int i = 0; i < times; i++) {
        d2(1); delay_us(120000);
        d2(0); delay_us(120000);
    }
}

int main(void) {
    RCC_AHB1ENR |= (1u << 0) | (1u << 1) | (1u << 2);   // GPIOA, GPIOB, GPIOC
    RCC_APB1ENR |= (1u << 0);                           // TIM2
    RCC_APB2ENR |= (1u << 4);                           // USART1

    mode_output(&GPIOB_MODER, TARGET_PIN);
    mode_output(&GPIOB_MODER, APPROACH_PIN);
    mode_output(&GPIOC_MODER, BOARD_LED_PIN);
    d1(0); d2(0); board(0);

    TIM2_PSC = 15;                                      // 16 MHz / 16 = 1 MHz
    TIM2_ARR = 0xFFFFFFFFu;
    TIM2_EGR = 1;                                       // load PSC now
    TIM2_CR1 = 1;                                       // start counting

    // PA9 = TX, PA10 = RX: alternate function mode (0b10), AF7 = USART1.
    GPIOA_MODER = (GPIOA_MODER & ~((3u << 18) | (3u << 20))) | (2u << 18) | (2u << 20);
    GPIOA_AFRH  = (GPIOA_AFRH & ~(0xFFu << 4)) | (7u << 4) | (7u << 8);
    // Pull-up on PA10 so the line idles high (not floating) with the GM65 unplugged.
    // Pull-up on PA0 for the KEY button (the board has no external one).
    GPIOA_PUPDR = (GPIOA_PUPDR & ~((3u << 20) | (3u << 0))) | (1u << 20) | (1u << 0);

    USART1_BRR = BRR_9600;
    USART1_CR1 = (1u << 13) | (1u << 3) | (1u << 2);    // UE, TE, RE

    // Step 1: LED self-test
    d1(1); d2(1); board(1); delay_us(300000);
    d1(0); d2(0); board(0); delay_us(300000);

    // Step 2: ask until the GM65 answers
    while (1) {
        uart_flush();
        uart_send(CMD_READ_ZONE0, sizeof CMD_READ_ZONE0);
        if (read_reply()) break;
        delay_us(1000000);
    }
    d1(1);

    // Step 3: collect scan data. A scan ends at CR/LF, or after 50 ms of silence.
    uint8_t line[48];
    int len = 0;
    int key_was_up = 1;

    while (1) {
        int key_up = (GPIOA_IDR >> KEY_PIN) & 1;
        if (!key_up && key_was_up) {
            uart_flush();
            uart_send(CMD_START_SCAN, sizeof CMD_START_SCAN);
            read_reply();                               // eat the 7-byte ack
        }
        key_was_up = key_up;

        int c = uart_getc(50000);
        int end = 0;
        if (c < 0) {
            end = (len > 0);                            // silence after data
        } else if (c == '\r' || c == '\n') {
            end = (len > 0);
        } else if (len < (int)sizeof line) {
            line[len++] = (uint8_t)c;
        }

        if (end) {
            int digits = 1;
            for (int i = 0; i < len; i++) {
                if (line[i] < '0' || line[i] > '9') digits = 0;
            }
            if (digits) { d2(1); delay_us(1000000); d2(0); }
            else        { blink_d2(3); }
            len = 0;
        }
    }
}
