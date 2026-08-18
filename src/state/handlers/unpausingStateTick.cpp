#include <types/SystemState.h>

extern void setSystemState(SystemState);

void unpausingStateTick() {
   setSystemState(SystemState::RUNNING);
}