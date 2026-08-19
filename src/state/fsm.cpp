#include <Arduino.h>
#include <state/fsm.h>
#include "handlers/handlers.h"
#include "logic/nextState.h"
#include <modules/server/ws.h>
#include <modules/server/broadcaster.h>

SystemState currentState = SystemState::INITIALIZING;

QueueHandle_t signalQueue = nullptr;

void setSystemState(SystemState state) {
    Serial.println("setSystemState: " + systemStateName(currentState) + " -> " + systemStateName(state));
    currentState = state;
    broadcaster_setSystemState(state);
}

void initFsm() {
    signalQueue = xQueueCreate(20, sizeof(SystemSignal));
    if (signalQueue == nullptr) {
        Serial.println("Critical Error: Failed to create FSM Signals Queue");
    }
}

void fsmTick() {
    if (signalQueue == nullptr) {
        return;
    }
    SystemSignal receivedSignal = SystemSignal::NONE;
    if (signalQueue != nullptr && xQueueReceive(signalQueue, &receivedSignal, 0) == pdPASS) {
        if (receivedSignal != SystemSignal::NONE) {
            Serial.println("Processing signal: " + systemSignalName(receivedSignal));  
            setSystemState(nextState(currentState, receivedSignal));
        }
    }
    
    runHandlerTick(currentState);
}

void sendSignal(SystemSignal signal) {
    if (signalQueue == nullptr) 
        return;
    Serial.println("Sending signal: " + systemSignalName(signal));    
    // xQueueSend pushes item to the back of the queue. 
    // Delay parameter is set to 0 (non-blocking) because web callbacks must execute instantly.
    if (xQueueSend(signalQueue, &signal, 0) != pdPASS) {
        Serial.println("Warning: FSM Signals Queue is full, dropping signal");
    }
}

