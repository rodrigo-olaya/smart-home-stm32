#include <stdint.h>
#include "drivers/uart/uart.h"

void main() {
    uartInit();

    while (1) {
        uartSendByte('A');
        // TODO: Use hardware timer
        for (volatile int i = 0; i < 1000000; i++);
    }
}