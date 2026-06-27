#include "timer.h"

volatile uint32_t tick;


void initTimers() {
    pRCC->APB1ENR |= (0x01 << TIM2_EN);
    *NVIC_ISER0 |= (0x01 << NVIC_TIM2_POSITION);

    pTIM2->ARR = TIM2_ARR;
    pTIM2->PSC = TIM2_PSC;

    pTIM2->DIER |= (0x01 << TIM2_UIE);
    pTIM2->CR1 |= (0x01 << TIM2_CEN);
}

void tim2Handler() {
    pTIM2->SR &= ~(0x01 << TIM2_UIF);
    tick++;
}