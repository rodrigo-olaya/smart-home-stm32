#include "exti.h"

void extiInit() {
    pRCC->APB2ENR |= (1 << SYSCFGEN);
    pSYSCFG->EXTICR[1] &= ~(0xF << SYSCFG_EXTI5_SHIFT);
    pEXTI->IMR |= (1 << ECHO_PIN);
    pEXTI->RTSR |= (1 << ECHO_PIN);
    pEXTI->FTSR |= (1 << ECHO_PIN);

    *NVIC_ISER0 |= (0x01 << NVIC_EXTI5_POS);
}

void exti_9_5_ISR() {
    if (pGPIOA->IDR & (1 << ECHO_PIN)) {
        handleEcho(1);
    }
    else {
        handleEcho(0);
    }
    pEXTI->PR = (1 << ECHO_PIN);
}