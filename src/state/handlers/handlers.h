#pragma once

#include <types/SystemState.h>

void initializingStateTick();
void pausingStateTick();
void runningStateTick();
void startingStateTick();
void stoppingStateTick();
void unpausingStateTick();
void rebootingStateTick();

inline void runHandlerTick(SystemState currentState) {
     switch (currentState) {
        case SystemState::INITIALIZING:
            return initializingStateTick();
        case SystemState::STARTING:
            return startingStateTick();
        case SystemState::RUNNING:
            return runningStateTick();
        case SystemState::PAUSING:
            return pausingStateTick();
        case SystemState::STOPPING:
            return stoppingStateTick();
        case SystemState::UNPAUSING:
            return unpausingStateTick();
        case SystemState::REBOOTING:
            return rebootingStateTick();
        default:
            break;    
    }
}