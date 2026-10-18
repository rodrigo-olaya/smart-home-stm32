#ifndef POSITION_HANDLER_H
#define POSITION_HANDLER_H

#include <stdint.h>
#include "../drivers/timers/timer.h"
#include "../drivers/uart/uart.h"
#include "../drivers/pwm/pwm.h"

void positionInit(void);
void handlePosition(uint16_t distanceCm);
void updatePositionLeds(void);

#endif
