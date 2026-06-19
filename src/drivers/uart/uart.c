#include "uart.h"

void uartInit() {
    RCC_AHB1ENR |= 1;
    RCC_APB1ENR |= (1 << 17);

    GPIOA_MODER &= ~(3 << 6);
    GPIOA_MODER &= ~(3 << 4);

    GPIOA_MODER |= (2 << 6);
    GPIOA_MODER |= (2 << 4);

    GPIOA_AFRL &= ~(15 << 12);
    GPIOA_AFRL &= ~(15 << 8);

    GPIOA_AFRL |= (0b0111 << 12);
    GPIOA_AFRL |= (0b0111 << 8);

    USART2_CR1 |= (3 << 2); // Enable TE and RE
    USART2_BRR |= (1666 << 4) | 11; //9600 baud
    USART2_CR1 |= (1 << 13); // Enable UE
}