#include <types/SystemState.h>

extern void setSystemState(SystemState);

void initializingStateTick() {
    setSystemState(SystemState::IDLE);
}
