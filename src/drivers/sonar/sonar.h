#ifndef SONAR_H
#define SONAR_H

#include "../registers.h"
#include "../gpio/gpio.h"
#include "../timers/timer.h"

#define TRIG_PIN 4
#define ECHO_PIN 5

void sonarInit();

uint16_t readSonar();

#endif