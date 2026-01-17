#ifndef CHASE_MANAGER_H
#define CHASE_MANAGER_H

#include <Arduino.h>
#include "pinDefinitions.h"
#include "ModeTypes.h"

class ChaseManager {
public:
    AutoMode autoMode;
    ChaseMode chaseMode;

    uint32_t autoStartTime = 0;
    uint32_t lastSpeedUpdate = 0;
    uint32_t nextSpeedInterval = 0;
    uint32_t lastDirectionChange = 0;
    uint32_t nextDirectionInterval = 0;

    bool isManualMode = true;

    void updateState(uint16_t address, uint16_t command);
    void updateTimeState();
};

#endif
