#include "stdint.h"
#include "registers.h"
#include "sonar.h"

typedef struct {
    volatile uint32_t IMR;    // 0x00 - Interrupt mask register
    volatile uint32_t EMR;    // 0x04 - Event mask register
    volatile uint32_t RTSR;   // 0x08 - Rising trigger selection
    volatile uint32_t FTSR;   // 0x0C - Falling trigger selection
    volatile uint32_t SWIER;  // 0x10 - Software interrupt event register
    volatile uint32_t PR;     // 0x14 - Pending register
} EXTI_RegDef_t;

#define pEXTI ((EXTI_RegDef_t*)0x40013C00)

typedef struct {
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];  // 0x08-0x14, 4 registers, each covering 4 EXTI lines
    volatile uint32_t reserved[2];
    volatile uint32_t CMPCR;
} SYSCFG_RegDef_t;

#define pSYSCFG ((SYSCFG_RegDef_t*)0x40013800)

#define SYSCFGEN 14
#define SYSCFG_EXTI5_SHIFT 4

void extiInit();