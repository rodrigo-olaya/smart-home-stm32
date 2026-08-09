#include "gpio.h"

void gpioInit() {
    pRCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOA_EN);
}