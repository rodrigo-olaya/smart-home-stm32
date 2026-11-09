#include <stdint.h>

// Carrier board test, polling only (no interrupts).
//   PA4 = TRIG (out)   PA5 = ECHO (in, divided to 3.3 V by R3/R4)
//   PB7 = D1 target LED   PB8 = D2 approach LED   (pin HIGH = LED on)
//   PC13 = Black Pill LED (pin LOW = LED on)
//
// On boot: D1, then D2, then PC13 each light for 0.4 s (checks the LEDs).
// Then it loops on the sonar:
//   closer than 30 cm   -> D1 (target) on
//   30 to 80 cm         -> D2 (approach) on
//   farther / no echo   -> both off
//   PC13 is on whenever an echo was received.
// TIM2 free-runs at 1 MHz (HSI 16 MHz / 16), so one count = 1 us.

#define RCC_AHB1ENR   (*(volatile uint32_t *)0x40023830)
#define RCC_APB1ENR   (*(volatile uint32_t *)0x40023840)

#define GPIOA_MODER   (*(volatile uint32_t *)0x40020000)
#define GPIOA_IDR     (*(volatile uint32_t *)0x40020010)
#define GPIOA_BSRR    (*(volatile uint32_t *)0x40020018)
#define GPIOB_MODER   (*(volatile uint32_t *)0x40020400)
#define GPIOB_BSRR    (*(volatile uint32_t *)0x40020418)
#define GPIOC_MODER   (*(volatile uint32_t *)0x40020800)
#define GPIOC_BSRR    (*(volatile uint32_t *)0x40020818)

#define TIM2_CR1      (*(volatile uint32_t *)0x40000000)
#define TIM2_EGR      (*(volatile uint32_t *)0x40000014)
#define TIM2_CNT      (*(volatile uint32_t *)0x40000024)
#define TIM2_PSC      (*(volatile uint32_t *)0x40000028)
#define TIM2_ARR      (*(volatile uint32_t *)0x4000002C)

#define TRIG_PIN      4
#define ECHO_PIN      5
#define TARGET_PIN    7     // PB7, D1
#define APPROACH_PIN  8     // PB8, D2
#define BOARD_LED_PIN 13    // PC13
#define TARGET_CM     30
#define APPROACH_CM   80
#define ECHO_TIMEOUT_US 30000u

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

// Echo pulse width in us, or 0 if no echo came back in time.
static uint32_t measure_us(void) {
    GPIOA_BSRR = (1u << TRIG_PIN);              // 10 us TRIG pulse
    delay_us(10);
    GPIOA_BSRR = (1u << (TRIG_PIN + 16));

    uint32_t t0 = TIM2_CNT;
    while (!(GPIOA_IDR & (1u << ECHO_PIN))) {
        if ((TIM2_CNT - t0) > ECHO_TIMEOUT_US) return 0;
    }
    uint32_t rise = TIM2_CNT;
    while (GPIOA_IDR & (1u << ECHO_PIN)) {
        if ((TIM2_CNT - rise) > ECHO_TIMEOUT_US) return 0;
    }
    return TIM2_CNT - rise;
}

int main(void) {
    RCC_AHB1ENR |= (1u << 0) | (1u << 1) | (1u << 2);   // GPIOA, GPIOB, GPIOC
    RCC_APB1ENR |= (1u << 0);                           // TIM2

    mode_output(&GPIOA_MODER, TRIG_PIN);                // ECHO stays input
    mode_output(&GPIOB_MODER, TARGET_PIN);
    mode_output(&GPIOB_MODER, APPROACH_PIN);
    mode_output(&GPIOC_MODER, BOARD_LED_PIN);
    d1(0); d2(0); board(0);

    TIM2_PSC = 15;                                      // 16 MHz / 16 = 1 MHz
    TIM2_ARR = 0xFFFFFFFFu;                             // 32-bit on the F4
    TIM2_EGR = 1;                                       // load PSC now
    TIM2_CR1 = 1;                                       // start counting

    // LED self-test
    d1(1);    delay_us(400000); d1(0);
    d2(1);    delay_us(400000); d2(0);
    board(1); delay_us(400000); board(0);

    while (1) {
        uint32_t us = measure_us();
        uint32_t cm = us / 58;                          // ~58 us per cm round trip
        board(us != 0);
        d1(us != 0 && cm < TARGET_CM);
        d2(us != 0 && cm >= TARGET_CM && cm < APPROACH_CM);
        delay_us(60000);                                // >60 ms between pings
    }
}
