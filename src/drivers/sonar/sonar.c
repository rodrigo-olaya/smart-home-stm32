#include "sonar.h"

void sonarInit(){
    pGPIOA->MODER &= ~(3 << (TRIG_PIN * 2));
    pGPIOA->MODER |= (MODER_GP_OUT << (TRIG_PIN * 2));
    pGPIOA->MODER &= ~(3 << (ECHO_PIN * 2));
}