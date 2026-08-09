#ifndef SONAR_H
#define SONAR_H

#include "../registers.h"
#include "../gpio/gpio.h"

#define TRIG_PIN 4
#define ECHO_PIN 5

void sonarInit();

#endif