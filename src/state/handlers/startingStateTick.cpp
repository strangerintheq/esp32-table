#include <types/SystemState.h>
#include <state/state.h>
#include <state/fsm.h>

void startingStateTick() {
    initState();
    setSystemState(SystemState::RUNNING);
}