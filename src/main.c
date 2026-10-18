#include <stdint.h>
#include "drivers/uart/uart.h"
#include "drivers/timers/timer.h"
#include "drivers/gpio/gpio.h"
#include "drivers/sonar/sonar.h"
#include "drivers/exti/exti.h"
#include "drivers/pwm/pwm.h"
#include "heartbeatHandler/heartbeatHandler.h"
#include "positionHandler/positionHandler.h"

void main() {
    gpioInit();
    uartInit();
    initTimers();
    sonarInit();
    extiInit();
    pwmInit();

    uint32_t startTick = tick;
    triggerSonar();

    while (1) {
        // uartSendByte('B');
        // uint32_t start = tick;
        // while (tick - start < 1000);

        // int16_t received;
        // while ((received = uartRxDequeue()) != -1) {
        //     uartTxEnqueue((uint8_t)received);
        // }
        if (tick - lastSonarTrigger >= 60) {
            handlePosition(handleSonar());
        }

        if (tick - lastHeartbeat >= 1000) {
            handleHeartbeat();
        }
        // while (tick - start < 1000);
    }
}