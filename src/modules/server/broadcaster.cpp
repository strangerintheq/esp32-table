#include <types/SystemState.h>
#include <utils/JsonBuilder.h>
#include <types/BroadcastMessage.h>
#include <modules/temp/temp.h>

JsonBuilder<256> jsonBuilder;
BroadcastMessage broadcastMessage;

void broadcaster_setSystemState(SystemState state) {
    broadcastMessage.systemState = state;
}

void broadcaster_setTargetPointIndex(uint16_t index) {
    broadcastMessage.targetPointIndex = index;
}

const char* broadcaster_getBroadcastMessage() {
    jsonBuilder.clear();
    jsonBuilder.append("targetPointIndex", broadcastMessage.targetPointIndex);
    jsonBuilder.append("temp", getTemp());
    jsonBuilder.append("systemState", systemStateName(broadcastMessage.systemState));
    return jsonBuilder.c_str();
}
