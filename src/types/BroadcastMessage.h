#pragma once
#include "SystemState.h"

struct BroadcastMessage {
    SystemState systemState;
    uint32_t targetPointIndex;
    uint8_t temp;
};
