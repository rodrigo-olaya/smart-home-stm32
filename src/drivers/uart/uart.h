#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "../registers.h"

#define UART_BUF_MAX 50

typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_RegDef_t;

typedef struct {
	uint8_t buffer[UART_BUF_MAX];
	uint8_t head;
	uint8_t tail;
} Circular_Buffer_t;

#define pUSART2 ((USART_RegDef_t *)0x40004400)
// #define pRCC    ((RCC_RegDef_t *)0x40023800)
#define pGPIOA  ((GPIO_RegDef_t *)0x40020000)

#define USART_CR1_RE 2
#define USART_CR1_TE 3
#define USART_CR1_UE 13

#define USART_SR_TC 6
#define USART_SR_TXE 7

#define RCC_AHB1ENR_GPIOA_EN 0
#define RCC_APB1ENR_USART2_EN 17

#define GPIO_AF7 7
#define GPIO_AF15 15

#define TX_PIN 2
#define RX_PIN 3

enum {
    MODER_INPUT = 0,
    MODER_GP_OUT = 1,
    MODER_ALT_FUNC = 2,
    MODER_ANALOG_MODE = 3
};

void uartInit();

int uartSendByte(uint8_t byteToSend);

int uartEnqueue(uint8_t byteToEnqueue);

uint8_t uartDequeue();

#endif