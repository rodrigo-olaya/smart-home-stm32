#include <stdint.h>

// Black Pill V3.1: user LED on PC13, wired LED -> 3V3, so it is ACTIVE LOW
// (pin low = LED on).

#define RCC_AHB1ENR   (*(volatile uint32_t *)0x40023830)
#define GPIOC_MODER   (*(volatile uint32_t *)0x40020800)
#define GPIOC_BSRR    (*(volatile uint32_t *)0x40020818)

#define GPIOC_EN      2      // RCC_AHB1ENR bit for port C
#define LED_PIN       13

static void delay(volatile uint32_t n) {
    while (n--) { }
}

int main(void) {
    RCC_AHB1ENR |= (1u << GPIOC_EN);

    GPIOC_MODER &= ~(3u << (LED_PIN * 2));   // clear the two mode bits
    GPIOC_MODER |=  (1u << (LED_PIN * 2));   // 01 = general purpose output

    while (1) {
        GPIOC_BSRR = (1u << (LED_PIN + 16)); // reset bit -> pin low -> LED on
        delay(500000);
        GPIOC_BSRR = (1u << LED_PIN);        // set bit   -> pin high -> LED off
        delay(500000);
    }
}
