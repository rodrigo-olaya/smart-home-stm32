#include "positionHandler.h"

#define TARGET_MIN_CM 25
#define TARGET_MAX_CM 35

// A single dropped echo shouldn't blank the LEDs; this many in a row means nothing is there
#define MAX_MISSED_READINGS 3

typedef struct {
    uint16_t maxDistanceCm;   // row applies up to and including this distance
    uint16_t brightness;      // permille
} approachLevel_t;

static const approachLevel_t approachLevels[] = {
    {  10,  250 },
    {  24,  500 },
    {  50,  500 },
    {  70,  250 },
    { 100,  100 },
    { 150,   30 },
};

#define APPROACH_LEVEL_COUNT (sizeof(approachLevels) / sizeof(approachLevels[0]))

static uint8_t missedReadings;

void positionInit(void) {

}

void handlePosition(uint16_t distanceCm) {
    if (distanceCm == 0) {
        if (missedReadings < MAX_MISSED_READINGS) {
            missedReadings++;
            return;
        }
        pwmSetDuty(PWM_CH_APPROACH, 0);
        pwmSetDuty(PWM_CH_TARGET, 0);
        return;
    }

    missedReadings = 0;

    if (distanceCm >= TARGET_MIN_CM && distanceCm <= TARGET_MAX_CM) {
        pwmSetDuty(PWM_CH_APPROACH, 0);
        pwmSetDuty(PWM_CH_TARGET, 1000);
        return;
    }

    pwmSetDuty(PWM_CH_TARGET, 0);

    for (uint8_t i = 0; i < APPROACH_LEVEL_COUNT; i++) {
        if (distanceCm <= approachLevels[i].maxDistanceCm) {
            pwmSetDuty(PWM_CH_APPROACH, approachLevels[i].brightness);
            return;
        }
    }

    pwmSetDuty(PWM_CH_APPROACH, 0);
}
