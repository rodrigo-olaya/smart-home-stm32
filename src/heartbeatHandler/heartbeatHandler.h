#ifndef HEARTBEAT_HANDLER_H
#define HEARTBEAT_HANDLER_H

#include "../drivers/timers/timer.h"
#include "../drivers/uart/uart.h"

extern volatile uint32_t lastHeartbeat;

void handleHeartbeat();

#endif