#pragma once

#include <types/SystemState.h>

void broadcaster_setSystemState(SystemState);

void broadcaster_setTargetPointIndex(uint16_t);

const char* broadcaster_getBroadcastMessage();
