#include <types/SystemState.h>
#include <state/state.h>

extern void setSystemState(SystemState);

void stoppingStateTick() {
   stopStateTicks();
   setSystemState(SystemState::IDLE);
}