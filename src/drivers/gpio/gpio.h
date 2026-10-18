#ifndef GPIO_H
#define GPIO_H

#include "../registers.h"

#define pGPIOA  ((GPIO_RegDef_t *)0x40020000)
#define pGPIOB  ((GPIO_RegDef_t *)0x40020400)

#define RCC_AHB1ENR_GPIOA_EN 0
#define RCC_AHB1ENR_GPIOB_EN 1

#define GPIO_AF2 2

enum {
    MODER_INPUT = 0,
    MODER_GP_OUT = 1,
    MODER_ALT_FUNC = 2,
    MODER_ANALOG_MODE = 3
};

void gpioInit();

#endif