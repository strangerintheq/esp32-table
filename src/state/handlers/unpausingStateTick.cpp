#include <types/SystemState.h>
#include <state/fsm.h>

void unpausingStateTick() {
   setSystemState(SystemState::RUNNING);
}