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

uint16_t readSonar() {
    uint32_t start = pTIM3->CNT;

    pGPIOA->BSRR = (1 << TRIG_PIN);
    while (elapsedUs(start) < 10);

    pGPIOA->BSRR = (1 << (TRIG_PIN + 16));

    uint32_t preSoundStart = pTIM3->CNT;

    while (~(pGPIOA->IDR & (1 << ECHO_PIN)) && elapsedUs(preSoundStart) < 100);

    uint32_t soundStart = pTIM3->CNT;

    while (pGPIOA->IDR & (1 << ECHO_PIN) && elapsedUs(soundStart) < 60000);

    uint32_t elapsedTime = elapsedUs(soundStart);

    uint32_t out = processSonarData(elapsedTime);

    return out;
}