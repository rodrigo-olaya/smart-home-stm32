#include <stdint.h>

#define RCC_AHB1ENR  (*(volatile uint32_t *)0x40023830)
#define GPIOD_MODER  (*(volatile uint32_t *)0x40020C00)
#define GPIOD_ODR    (*(volatile uint32_t *)0x40020C14)

int main() {

    RCC_AHB1ENR |= (1 << 3);

    GPIOD_MODER &= ~(3 << 24);
    GPIOD_MODER |= (1 << 24);

    while (1) {
        GPIOD_ODR ^= (1 << 12);
        for (volatile int i = 0; i < 500000; i++);
    }

    return 0;
}