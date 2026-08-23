#include "heartbeatHandler.h"

volatile uint32_t lastHeartbeat;

void handleHeartbeat() {
    uartTxEnqueue(0xAA);
    uartTxEnqueue(0x01);
    uartTxEnqueue('H');
    uartTxEnqueue('B');
    uartTxEnqueue('H');
    uartTxEnqueue('B');
    lastHeartbeat = tick;
}   