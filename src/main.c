#include <stdint.h>
#include "uart/uart.h"

#define RCC_AHB1ENR  (*(volatile uint32_t *)0x40023830)
#define GPIOD_MODER  (*(volatile uint32_t *)0x40020C00)
#define GPIOD_ODR    (*(volatile uint32_t *)0x40020C14)

int main() {
    uartInit();

    while (1) {
        uartSendByte('A');
        for (volatile int i = 0; i < 1000000; i++);
    }

    return 0;
}