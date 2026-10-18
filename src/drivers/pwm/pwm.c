#include "pwm.h"

void pwmInit(void) {
    pRCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOB_EN);
    pRCC->APB1ENR |= (1 << TIM4_EN);

    pGPIOB->MODER &= ~(3 << (PWM_TARGET_PIN * 2));
    pGPIOB->MODER |= (MODER_ALT_FUNC << (PWM_TARGET_PIN * 2));

    pGPIOB->MODER &= ~(3 << (PWM_APPROACH_PIN * 2));
    pGPIOB->MODER |= (MODER_ALT_FUNC << (PWM_APPROACH_PIN * 2));

    pGPIOB->AFR[0] &= ~(0b1111 << PWM_AFRL7);
    pGPIOB->AFR[0] |= (GPIO_AF2 << PWM_AFRL7);

    pGPIOB->AFR[1] &= ~(0b1111 << PWM_AFRH8);
    pGPIOB->AFR[1] |= (GPIO_AF2 << PWM_AFRH8);

    pTIM4->ARR = PWM_ARR;
    pTIM4->PSC = PWM_PSC;

    pTIM4->CCMR1 &= ~(0b111 << PWM_OC2M);
    pTIM4->CCMR1 |= (0b110 << PWM_OC2M);

    pTIM4->CCMR1 &= ~(1 << PWM_OC2PE);
    pTIM4->CCMR1 |= (1 << PWM_OC2PE);

    pTIM4->CCMR2 &= ~(0b111 << PWM_OC3M);
    pTIM4->CCMR2 |= (0b110 << PWM_OC3M);
    
    pTIM4->CCMR2 &= ~(1 << PWM_OC3PE);
    pTIM4->CCMR2 |= (1 << PWM_OC3PE);
    
    pTIM4->CCER &= ~(1 << PWM_CC2E);
    pTIM4->CCER |= (1 << PWM_CC2E);
    
    pTIM4->CCER &= ~(1 << PWM_CC3E);
    pTIM4->CCER |= (1 << PWM_CC3E);
    
    pTIM4->CR1 &= ~(1 << PWM_ARPE);
    pTIM4->CR1 |=(1 << PWM_ARPE);
    
    pTIM4->EGR = (1 << PWM_UG);

    pTIM4->CR1 &= ~(1 << PWM_CEN);
    pTIM4->CR1 |= (1 << PWM_CEN);
}