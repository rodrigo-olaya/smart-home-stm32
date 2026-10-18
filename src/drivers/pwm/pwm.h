#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "../gpio/gpio.h"
#include "../timers/timer.h"

#define PWM_APPROACH_PIN  8    // PB8 → TIM4_CH3
#define PWM_TARGET_PIN    7    // PB7 → TIM4_CH2
#define PWM_PSC           15   // 16 MHz / 16 = 1 MHz
#define PWM_ARR           999  // 1 MHz / 1000 = 1 kHz, duty = CCR / 1000

#define PWM_AFRL7 28
#define PWM_AFRH8 0

#define PWM_OC2M 12
#define PWM_OC2PE 11
#define PWM_OC3M 4
#define PWM_OC3PE 3
#define PWM_CC2E 4
#define PWM_CC3E 8

#define PWM_ARPE 7
#define PWM_CEN 0

#define PWM_UG 0

typedef enum {
    PWM_CH_TARGET = 2,
    PWM_CH_APPROACH = 3
} pwm_channel_t;

void pwmInit(void);
void pwmSetDuty(pwm_channel_t ch, uint16_t permille);

#endif
