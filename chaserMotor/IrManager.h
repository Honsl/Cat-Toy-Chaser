#ifndef IR_MANAGER_H
#define IR_MANAGER_H

#include <Arduino.h>
#include "pinDefinitions.h"

class IrManager {
public:
    bool receiveIR(uint16_t &address, uint16_t &command);
    void irBegin();
};

#endif