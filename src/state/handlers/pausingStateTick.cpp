#include <types/SystemState.h>

extern void setSystemState(SystemState);

void pausingStateTick() {
    setSystemState(SystemState::PAUSED);
}
