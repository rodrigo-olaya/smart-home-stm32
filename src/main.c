#include <stdint.h>
#include "drivers/uart/uart.h"

void main() {
    uartInit();

    while (1) {
        uartSendByte('B');
        // TODO: Use hardware timer
        for (volatile int i = 0; i < 1000000; i++);
    }
}