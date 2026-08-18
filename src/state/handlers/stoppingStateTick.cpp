#include <types/SystemState.h>

extern void setSystemState(SystemState);

void stoppingStateTick() {
   setSystemState(SystemState::IDLE);
}