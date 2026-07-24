#include "uart.h"

static const int clock = 16000000; // 16MH
static const int baud = 9600;
static const int baudMantissa = clock / (16 * baud);
static const int baudFraction = ((clock % (16 * baud)) * 16) / (16 * baud);  // = 2

Circular_Buffer_t uartTxBuffer;
Circular_Buffer_t uartRxBuffer;

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

    pUSART2->CR1 |= (1 << USART_CR1_RXNEIE);

    *NVIC_ISER1 |= (1 << ISER1_UART2_POSITION);
}

int uartTxEnqueue(uint8_t byteToEnqueue) {
    if ((uartTxBuffer.tail + 1) % UART_BUF_MAX != uartTxBuffer.head) {
        uartTxBuffer.buffer[uartTxBuffer.tail] = byteToEnqueue; 
        uartTxBuffer.tail = (uartTxBuffer.tail + 1) % UART_BUF_MAX;
        pUSART2->CR1 |= (1 << USART_CR1_TXEIE);
    }
    else {
        return -1;
    }
    return 0;
}

int16_t uartTxDequeue() {
    int16_t dequeued;
    if (uartTxBuffer.head != uartTxBuffer.tail) {
        dequeued = uartTxBuffer.buffer[uartTxBuffer.head];
        uartTxBuffer.head = (uartTxBuffer.head + 1) % UART_BUF_MAX;
    }
    else {
        pUSART2->CR1 &= ~(1 << USART_CR1_TXEIE);
        return -1;
    }
    return dequeued;
}

int uartRxEnqueue() {
    uint8_t data = (uint8_t)pUSART2->DR;

    if ((uartRxBuffer.tail + 1) % UART_BUF_MAX != uartRxBuffer.head) {
        uartRxBuffer.buffer[uartRxBuffer.tail] = data;
        uartRxBuffer.tail = (uartRxBuffer.tail + 1) % UART_BUF_MAX;
    }
    else {
        return -1;
    }

    return 0;
}

int16_t uartRxDequeue() {
    int16_t dequeued;
    if (uartRxBuffer.head != uartRxBuffer.tail) {
        dequeued = uartRxBuffer.buffer[uartRxBuffer.head];
        uartRxBuffer.head = (uartRxBuffer.head + 1) % UART_BUF_MAX;
    }
    else {
        return -1;
    }
    return dequeued;
}


void uartISR() {
    // assumption: the code we have for the ISR will call this for ANY flag
    if (pUSART2->SR & (1 << USART_SR_RXNE)){
        int enqueued = uartRxEnqueue();
    }
    if (pUSART2->SR & (1 << USART_SR_TXE)){
        int16_t dequeued = uartTxDequeue();
        if (dequeued != -1) {
            pUSART2->DR = (uint8_t)dequeued;
        }
        else {
            // buffer is empty
        }
    }
}