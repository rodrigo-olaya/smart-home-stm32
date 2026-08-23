#include "exti.h"

void extiInit() {
    pRCC->APB2ENR |= (1 << SYSCFGEN);
    pSYSCFG->EXTICR[1] &= ~(0xF << SYSCFG_EXTI5_SHIFT);
    pEXTI->IMR |= (1 << ECHO_PIN);
    pEXTI->RTSR |= (1 << ECHO_PIN);
    pEXTI->FTSR |= (1 << ECHO_PIN);
}