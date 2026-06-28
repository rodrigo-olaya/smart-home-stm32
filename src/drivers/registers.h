#ifndef REGISTERS_H
#define REGISTERS_H

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

#define pRCC ((RCC_RegDef_t*)0x40023800)

#define NVIC_ISER0 ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1 ((volatile uint32_t*)0xE000E104)

#endif