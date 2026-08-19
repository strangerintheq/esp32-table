#include <types/SystemState.h>
#include <state/fsm.h>

void initializingStateTick() {
    setSystemState(SystemState::IDLE);
}
