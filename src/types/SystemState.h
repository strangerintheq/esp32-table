#pragma once

#include <Arduino.h>

enum SystemState {
    INITIALIZING,
    IDLE,
    STARTING,
    RUNNING,
    PAUSING,  
    PAUSED,
    UNPAUSING,
    STOPPING,
    COMPLETED,
    ERROR,
    REBOOTING
};

inline String systemStateName(SystemState state){
    switch (state){
        case SystemState::INITIALIZING: return "INITIALIZING";
        case SystemState::IDLE: return "IDLE";
        case SystemState::STARTING: return "STARTING";
        case SystemState::RUNNING: return "RUNNING";
        case SystemState::PAUSING: return "PAUSING";
        case SystemState::PAUSED: return "PAUSED";
        case SystemState::UNPAUSING: return "UNPAUSING";
        case SystemState::STOPPING: return "STOPPING";
        case SystemState::COMPLETED: return "COMPLETED";
        case SystemState::ERROR: return "ERROR";
        case SystemState::REBOOTING: return "REBOOTING";
    }
    return "!! UNKNOWN_STATE!!";
}
