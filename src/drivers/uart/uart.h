#ifndef UART_H
#define UART_H

#include <stdint.h>

typedef struct {
    volatile uint32_t MODER;		// Address offset: 0x00
	volatile uint32_t OTYPER;		// Address offset: 0x04
	volatile uint32_t OSPEEDR;		// Address offset: 0x08
	volatile uint32_t PUPDR;		// Address offset: 0x0C
	volatile uint32_t IDR;			// Address offset: 0x10
	volatile uint32_t ODR;			// Address offset: 0x14
	volatile uint32_t BSRR;			// Address offset: 0x18
	volatile uint32_t LCKR;			// Address offset: 0x1C
	volatile uint32_t AFR[2];		// Address offset: 0x20
} GPIO_RegDef_t;

typedef struct
{
	volatile uint32_t CR;            // Address offset: 0x00
	volatile uint32_t PLLCFGR;      // Address offset: 0x04
	volatile uint32_t CFGR;         // Address offset: 0x08
	volatile uint32_t CIR;          // Address offset: 0x0C
	volatile uint32_t AHB1RSTR;     // Address offset: 0x10
	volatile uint32_t AHB2RSTR;     // Address offset: 0x14
	volatile uint32_t AHB3RSTR;     // Address offset: 0x18
	uint32_t          RESERVED0;    // Reserved, 0x1C
	volatile uint32_t APB1RSTR;     // Address offset: 0x20
	volatile uint32_t APB2RSTR;     // Address offset: 0x24
	uint32_t          RESERVED1[2]; // Reserved, 0x28-0x2C
	volatile uint32_t AHB1ENR;      // Address offset: 0x30
	volatile uint32_t AHB2ENR;      // Address offset: 0x34
	volatile uint32_t AHB3ENR;      // Address offset: 0x38
	uint32_t          RESERVED2;    // Reserved, 0x3C
	volatile uint32_t APB1ENR;      // Address offset: 0x40
	volatile uint32_t APB2ENR;      // Address offset: 0x44
	uint32_t          RESERVED3[2]; // Reserved, 0x48-0x4C
	volatile uint32_t AHB1LPENR;    // Address offset: 0x50
	volatile uint32_t AHB2LPENR;    // Address offset: 0x54
	volatile uint32_t AHB3LPENR;    // Address offset: 0x58
	uint32_t          RESERVED4;    // Reserved, 0x5C
	volatile uint32_t APB1LPENR;    // Address offset: 0x60
	volatile uint32_t APB2LPENR;    // Address offset: 0x64
	uint32_t          RESERVED5[2]; // Reserved, 0x68-0x6C
	volatile uint32_t BDCR;         // Address offset: 0x70
	volatile uint32_t CSR;          // Address offset: 0x74
	uint32_t          RESERVED6[2]; // Reserved, 0x78-0x7C
	volatile uint32_t SSCGR;        // Address offset: 0x80
	volatile uint32_t PLLI2SCFGR;   // Address offset: 0x84
	volatile uint32_t PLLSAICFGR;   // Address offset: 0x88
	volatile uint32_t DCKCFGR;      // Address offset: 0x8C
	volatile uint32_t CKGATENR;     // Address offset: 0x90
	volatile uint32_t DCKCFGR2;     // Address offset: 0x94

} RCC_RegDef_t;

typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR2;
    volatile uint32_t GTPR;
} USART_RegDef_t;

USART_RegDef_t *pUSART2 = (USART_RegDef_t *)0x40004400;
RCC_RegDef_t *pRCC = (RCC_RegDef_t *)0x40023800;
GPIO_RegDef_t *pGPIOA = (GPIO_RegDef_t *)0x40020000;

#define USART_CR1_RE 2
#define USART_CR1_TE 3
#define USART_CR1_UE 13

#define USART_SR_TC 6
#define USART_SR_TXE 7

#define RCC_AHB1ENR_GPIOA_EN 0
#define RCC_APB1ENR_USART2_EN 17

#define GPIO_AF7 7
#define GPIO_AF15 11

#define TX_PIN 2
#define RX_PIN 3

enum {
    MODER_INPUT = 0,
    MODER_GP_OUT = 1,
    MODER_ALT_FUNC = 2,
    MODER_ANALOG_MODE = 3
};

void uartInit();

int uartSendByte();

#endif