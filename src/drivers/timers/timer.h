#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "../registers.h"

#define pRCC ((RCC_RegDef_t*)0x40023800)
#define NVIC_ISER0 ((volatile uint32_t*)0xE000E100)

#define NVIC_TIM2_POSITION 28
#define TIM2_EN 0

void initTimers();

#endif