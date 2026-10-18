#include <stdint.h>
#include "drivers/uart/uart.h"
#include "drivers/timers/timer.h"
#include "drivers/gpio/gpio.h"
#include "drivers/sonar/sonar.h"
#include "drivers/exti/exti.h"
#include "drivers/pwm/pwm.h"
#include "heartbeatHandler/heartbeatHandler.h"

void main() {
    gpioInit();
    uartInit();
    initTimers();
    sonarInit();
    extiInit();
    pwmInit();

    uint32_t startTick = tick;
    triggerSonar();

    uint16_t permilleTestValues[6] = {0, 30, 100, 250, 500, 1000};
    int i = 0;

    lastPwmTrigger = tick;
    
    while (1) {
        // uartSendByte('B');
        // uint32_t start = tick;
        // while (tick - start < 1000);

        // int16_t received;
        // while ((received = uartRxDequeue()) != -1) {
        //     uartTxEnqueue((uint8_t)received);
        // }
        if (tick - lastSonarTrigger >= 60) {
            handleSonar();
        }
        
        if (tick - lastHeartbeat >= 1000) {
            handleHeartbeat();
        }

        if (tick - lastPwmTrigger > 500) {
            
            pwmSetDuty(2, permilleTestValues[i]);
            pwmSetDuty(3, permilleTestValues[i]);
            i++;
            if (i > 5) i = 0;
            lastPwmTrigger = tick;
        }
        // while (tick - start < 1000);
    }
}