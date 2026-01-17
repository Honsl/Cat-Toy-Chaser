#ifndef MOTOR_MANAGER_H
#define MOTOR_MANAGER_H

#include <Arduino.h>
#include "pinDefinitions.h"
#include <cstdlib>
class MotorManager {
public:
    void update(uint8_t speed, bool directionRight);

private:
    bool lastDirectionRight = false;
    uint8_t currentSpeed = 0;
    uint32_t lastRampUpdate = 0;
    bool toyPinned = false;
    uint32_t toyPinnedEndTime = 0;


    void applyDirection(bool directionRight);
    void rampToSpeed(uint8_t targetSpeed, bool directionRight);
    void checkStall(uint8_t pwm);

};

#endif