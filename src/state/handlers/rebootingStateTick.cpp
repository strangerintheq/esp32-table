#include <Arduino.h>

uint32_t rebootStartTimestamp = 0;

void rebootingStateTick() {
    uint32_t now = millis();
    if (rebootStartTimestamp == 0)
        rebootStartTimestamp = now;
    if (now - rebootStartTimestamp < 500)
        return;
    ESP.restart();        
}