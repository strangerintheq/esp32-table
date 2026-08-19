#pragma once

#include <types/SystemState.h>
#include <types/SystemSignal.h>

void initFsm();

void fsmTick();

void sendSignal(SystemSignal);

void setSystemState(SystemState);

