#pragma once 

#include <Arduino.h>

enum SystemSignal {
    NONE,
    START,
    PAUSE,
    STOP,
    HARDWARE_FAULT,
    REBOOT,
    CLEAR
};

inline String systemSignalName(SystemSignal signal) {
    switch (signal) {
        case SystemSignal::NONE: return "NONE";
        case SystemSignal::START: return "START";
        case SystemSignal::PAUSE: return "PAUSE";
        case SystemSignal::STOP: return "STOP";
        case SystemSignal::HARDWARE_FAULT: return "HARDWARE_FAULT";
        case SystemSignal::REBOOT: return "REBOOT";
        case SystemSignal::CLEAR: return "CLEAR";
    }
    return "!! UNKNOWN_SIGNAL !!";
}
