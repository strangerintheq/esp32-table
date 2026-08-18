#include <types/SystemSignal.h>
#include <types/SystemState.h>

SystemState nextState(SystemState state, SystemSignal signal) {

    if (signal == SystemSignal::HARDWARE_FAULT) {
        return SystemState::ERROR;
    }

    switch (state) {
        case SystemState::COMPLETED:
        case SystemState::ERROR:
            if (signal == SystemSignal::CLEAR)
                return SystemState::IDLE;
            break;

        case SystemState::IDLE:
        
            if (signal == SystemSignal::START)
                return SystemState::STARTING;
            break;    

        case SystemState::PAUSED:
            switch (signal) {
                case SystemSignal::START: 
                    return SystemState::UNPAUSING;
                case SystemSignal::STOP: 
                    return SystemState::STOPPING;
                default: 
                    break;
            }
            break;     

        case SystemState::RUNNING:
            switch (signal) {
                case SystemSignal::PAUSE: 
                    return SystemState::PAUSING;
                case SystemSignal::STOP: 
                    return SystemState::STOPPING;
                default: 
                    break;
            }
            break; 

         default: 
            break;         
    }

    return state;
}