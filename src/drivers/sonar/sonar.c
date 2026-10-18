#include "sonar.h"

volatile uint32_t preSoundStart;
volatile uint32_t soundStart;
volatile uint32_t elapsedTime;
volatile uint8_t ready;
volatile uint8_t waitingForFallingEdge;
volatile uint32_t lastSonarTrigger;

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

    lastSonarTrigger = tick;
}

uint16_t handleSonar() {
    uint16_t distance = readSonar();

    if (distance >= 10 && distance <= 60) {
        uartTxEnqueue(0xAA);
        uartTxEnqueue(0x02);
        uartTxEnqueue(0x00);
        uartTxEnqueue(0x00);
        uartTxEnqueue(0x00);
        uartTxEnqueue(0x01);
    }
    else {
        uartTxEnqueue(0xAA);
        uartTxEnqueue(0x02);
        uartTxEnqueue(0x00);
        uartTxEnqueue(0x00);
        uartTxEnqueue(0x00);
        uartTxEnqueue(0x00);
    }

    triggerSonar();
}

void handleEcho(uint8_t dir) {
    if (dir == 1) {
        soundStart = pTIM3->CNT;
        waitingForFallingEdge = 1;
    }
    else if (dir == 0 && waitingForFallingEdge == 1) {
        elapsedTime = elapsedUs(soundStart);
        ready = 1;
        waitingForFallingEdge = 0;
    }
}

uint16_t readSonar() {
    // while (!(pGPIOA->IDR & (1 << ECHO_PIN)) && elapsedUs(preSoundStart) < 10000);

    // soundStart = pTIM3->CNT;

    // while (pGPIOA->IDR & (1 << ECHO_PIN) && elapsedUs(soundStart) < 60000);

    // elapsedTime = elapsedUs(soundStart);

    if (!(ready == 1)) return 0;

    uint32_t out = processSonarData(elapsedTime);

    ready = 0;

    return out;
}