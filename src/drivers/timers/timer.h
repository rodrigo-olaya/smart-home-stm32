#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "../registers.h"

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t TIM2_OR;
} TIM_RegDef_t;


#define NVIC_ISER0 ((volatile uint32_t*)0xE000E100)

#define NVIC_TIM2_POSITION 28
#define TIM2_EN 0

#define pTIM2 ((TIM_RegDef_t*)0x40000000)

#define TIM2_ARR 1000
#define TIM2_PSC 15
#define TIM2_UIE 0
#define TIM2_CEN 0
#define TIM2_UIF 0

extern volatile uint32_t tick;

void initTimers();

void tim2Handler();

#endif