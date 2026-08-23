#ifndef SONAR_H
#define SONAR_H

#include "../registers.h"
#include "../gpio/gpio.h"
#include "../timers/timer.h"

#define TRIG_PIN 4
#define ECHO_PIN 5

extern volatile uint32_t preSoundStart;
extern volatile uint32_t soundStart;
extern volatile uint32_t elapsedTime;

void sonarInit();

uint16_t readSonar();

#endif