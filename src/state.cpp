#include <Arduino.h>
#include <modules/storage/storage.h>
#include <types/point.h>
#include <modules/server/ws.h>
#include <utils/JsonBuilder.h>


size_t pointsCount;
int32_t targetPointIndex;
Point targetPoint;
Point nextPoint;
bool isSequenceActive;
bool isTargetReached;

void readNextPoint() {
    size_t bytesRead = readBinary((uint8_t*)&nextPoint, sizeof(Point));
    if (bytesRead == sizeof(Point)) {
        Serial.printf("Point read successfully: a=%.2f, r=%.2f\n", nextPoint.a, nextPoint.r);
    } else {
        Serial.println("Failed to read the next point: end of file or error");
    }
}

void copyNextPointToCurrentPoint() {
    isTargetReached = false;
    targetPointIndex++;
    targetPoint = nextPoint;
    JsonBuilder<64> b;
    b.append("targetPointIndex", targetPointIndex);
    broadcast(b.c_str());
}

void initState() {
    pointsCount = fileSize("points.bin") / sizeof(Point);
    targetPointIndex = -1;
    
    if (pointsCount == 0) {
        Serial.println("Initialization aborted: points.bin is empty or missing");
        isSequenceActive = false;
        return;
    }

    startReadBinary("points.bin");
    isSequenceActive = true;

    // Phase 1: Load point 0 into nextPoint, then push it to targetPoint
    readNextPoint();
    copyNextPointToCurrentPoint();

    // Phase 2: Speculatively load point 1 into nextPoint if available
    if (pointsCount > 1) {
        readNextPoint();
    }
}

void stateTick() {
    //   Serial.println("isSequenceActive " + String(isSequenceActive));
    //    Serial.println("isTargetReached " + String(isTargetReached));
    if (!isSequenceActive || !isTargetReached) {
        return;
    }
        
    if (targetPointIndex < pointsCount - 1) {
        // Push pre-loaded point from pipeline to active hardware registers
        copyNextPointToCurrentPoint();
        
        // Fetch the next sequential point ahead of time into pipeline
        readNextPoint();
        
        Serial.printf("Advanced to point index: %u / %u\n", targetPointIndex, pointsCount);
    } else {
        Serial.println("All points from file have been processed");
        endReadBinary(); 
        isSequenceActive = false;
    }

}

int32_t getTargetPointIndex() {
    return targetPointIndex;
}

void steppersMoveFinished() {
    isTargetReached = true;
  
}