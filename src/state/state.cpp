#include <Arduino.h>
#include <modules/storage/storage.h>
#include <types/Point.h>
#include <utils/JsonBuilder.h>
#include <types/SystemState.h>
#include <modules/server/broadcaster.h>

size_t pointsCount;
int32_t targetPointIndex;
Point targetPoint;
Point nextPoint;
bool isTargetReached;

extern void setSystemState(SystemState state);

void readNextPoint() {
    size_t bytesRead = readBinary((uint8_t*)&nextPoint, sizeof(Point));
    if (bytesRead == sizeof(Point)) {
        Serial.printf("Point read successfully: a=%.2f, r=%.2f\n", nextPoint.a, nextPoint.r);
    } else {
        Serial.println("Failed to read the next point: end of file or error");
    }
}

void updateTargetPointIndex(int32_t i) {
    targetPointIndex = i;
    broadcaster_setTargetPointIndex(i);
}

void copyNextPointToCurrentPoint() {
    isTargetReached = false;
    targetPoint = nextPoint;
    updateTargetPointIndex(targetPointIndex + 1);
}

void initState() {
    pointsCount = fileSize("points.bin") / sizeof(Point);
    targetPointIndex = -1;
    if (pointsCount == 0) {
        Serial.println("Initialization aborted: points.bin is empty or missing");
        return;
    }
    startReadBinary("points.bin");
    readNextPoint();
    copyNextPointToCurrentPoint();
    if (pointsCount > 1) {
        readNextPoint();
    }
}

void stopStateTicks() {
     endReadBinary(); 
     updateTargetPointIndex(0);
}

void stateTick() {
    if (!isTargetReached) {
        return;
    }
    if (targetPointIndex < pointsCount - 1) {
        copyNextPointToCurrentPoint();
        readNextPoint();
    } else {
        setSystemState(SystemState::COMPLETED);
        stopStateTicks();
    }
}

void steppersMoveFinished() {
    isTargetReached = true;
}

