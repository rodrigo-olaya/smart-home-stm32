#include "sonar.h"

void sonarInit(){
    pGPIOA->MODER &= ~(3 << (TRIG_PIN * 2));
    pGPIOA->MODER |= (MODER_GP_OUT << (TRIG_PIN * 2));
    pGPIOA->MODER &= ~(3 << (ECHO_PIN * 2));
}

uint32_t processSonarData(uint32_t time) {
    if (time > 38000) {
        return 0;
    }
    else {
        return time / 58;
    }
}

void triggerSonar() {
    uint32_t start = pTIM3->CNT;

    pGPIOA->BSRR = (1 << TRIG_PIN);
    while (elapsedUs(start) < 10);

    pGPIOA->BSRR = (1 << (TRIG_PIN + 16));

    preSoundStart = pTIM3->CNT;
}

void handleEcho(uint8_t dir) {
    if (dir == 1) {
        soundStart = pTIM3->CNT;
    }
    else {
        elapsedTime = elapsedUs(soundStart);
    }
}

uint16_t readSonar() {
    // while (!(pGPIOA->IDR & (1 << ECHO_PIN)) && elapsedUs(preSoundStart) < 10000);

    // soundStart = pTIM3->CNT;

    // while (pGPIOA->IDR & (1 << ECHO_PIN) && elapsedUs(soundStart) < 60000);

    // elapsedTime = elapsedUs(soundStart);

    uint32_t out = processSonarData(elapsedTime);

    return out;
}