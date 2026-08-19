#include <types/SystemState.h>
#include <state/fsm.h>

void pausingStateTick() {
    setSystemState(SystemState::PAUSED);
}
