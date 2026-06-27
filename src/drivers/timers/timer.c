#include "timer.h"

void initTimers() {
    pRCC->APB1ENR |= (0x01 << TIM2_EN);
    *NVIC_ISER0 |= (0x01 << NVIC_TIM2_POSITION);
}