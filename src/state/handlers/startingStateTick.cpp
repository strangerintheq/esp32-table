#include <types/SystemState.h>
#include <state/state.h>

extern void setSystemState(SystemState);

void startingStateTick() {
    initState();
    setSystemState(SystemState::RUNNING);
}