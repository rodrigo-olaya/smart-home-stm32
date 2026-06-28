#include "uart.h"

static const int clock = 16000000; // 16MH
static const int baud = 9600;
static const int baudMantissa = clock / (16 * baud);
static const int baudFraction = ((clock % (16 * baud)) * 16) / (16 * baud);  // = 2

Circular_Buffer_t uartBuffer;

void uartInit() {
    pRCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOA_EN);
    pRCC->APB1ENR |= (1 << RCC_APB1ENR_USART2_EN);

    pGPIOA->MODER &= ~(3 << (TX_PIN * 2));
    pGPIOA->MODER &= ~(3 << (RX_PIN * 2));

    pGPIOA->MODER |= (MODER_ALT_FUNC << (TX_PIN * 2));
    pGPIOA->MODER |= (MODER_ALT_FUNC << (RX_PIN * 2));

    pGPIOA->AFR[0] &= ~(GPIO_AF15 << (TX_PIN * 4));
    pGPIOA->AFR[0] &= ~(GPIO_AF15 << (RX_PIN * 4));

    pGPIOA->AFR[0] |= (GPIO_AF7 << (TX_PIN * 4));
    pGPIOA->AFR[0] |= (GPIO_AF7 << (RX_PIN * 4));

    pUSART2->CR1 |= (1 << USART_CR1_RE); // Enable RE
    pUSART2->CR1 |= (1 << USART_CR1_TE); // Enable TE
    pUSART2->BRR |= (baudMantissa << 4) | baudFraction; //9600 baud
    pUSART2->CR1 |= (1 << USART_CR1_UE); // Enable UE
}

int uartSendByte(uint8_t byteToSend) {

    int timeout = 100000;
    while (!(pUSART2->SR & (1 << USART_SR_TXE))) {
        if (--timeout == 0) return -1;
    }

    timeout = 100000;
    pUSART2->DR = byteToSend;
    while (!(pUSART2->SR & (1 << USART_SR_TC))) {
        if (--timeout == 0) return -1;
    }

    return 0;
}

int uartEnqueue(uint8_t byteToEnqueue) {
    if ((uartBuffer.tail + 1) % UART_BUF_MAX != uartBuffer.head) {
        uartBuffer.buffer[uartBuffer.tail] = byteToEnqueue; 
        uartBuffer.tail = (uartBuffer.tail + 1) % UART_BUF_MAX;
    }
    else {
        return -1;
    }
    return 0;
}