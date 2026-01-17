#ifndef MODE_TYPES_H
#define MODE_TYPES_H

#include <Arduino.h>

enum ModeDuration : uint8_t {
    DURATION_1_MIN  = 0,
    DURATION_3_MIN  = 1,
    DURATION_5_MIN  = 2,
    DURATION_10_MIN = 3
};

enum ModeType : uint8_t {
    MODE_SLOW   = 0,
    MODE_MEDIUM = 1,
    MODE_FAST   = 2
};

struct AutoMode {
    ModeDuration time;
    ModeType mode = MODE_SLOW;
    uint8_t speed = 80;
    bool directionRight = false;
};

struct ChaseMode {
    uint8_t speed = 0;
    bool directionRight = false;
};

#endif