#include <stdint.h>
#include "drivers/uart/uart.h"
#include "drivers/timers/timer.h"

void main() {
    uartInit();
    initTimers();

    while (1) {
        uartSendByte('B');
        uint32_t start = tick;
        while (tick - start < 1000);
    }
}