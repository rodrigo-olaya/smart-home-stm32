#include <stdint.h>

#define USART2_BASE 0x40004400

#define USART2_SR (*(volatile uint32_t *)(USART2_BASE))
#define USART2_DR (*(volatile uint32_t *)(USART2_BASE + 0x04))
#define USART2_BRR (*(volatile uint32_t *)(USART2_BASE + 0x08))
#define USART2_CR1 (*(volatile uint32_t *)(USART2_BASE + 0x0C))

#define RCC_BASE 0x40023800
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x40))

#define GPIOA_BASE 0x40020000
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE))
#define GPIOA_AFRL (*(volatile uint32_t *)(GPIOA_BASE + 0x20))

void uartInit();