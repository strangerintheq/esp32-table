#include <ESPAsyncWebServer.h>
#include <utils/JsonBuilder.h>
#include <modules/temp/temp.h>
#include <state/state.h>
#include "broadcaster.h"

AsyncWebSocket ws("/ws"); 

uint32_t lastBroadcastTimestamp = 0;

void onWsEvent(
    AsyncWebSocket *server, 
    AsyncWebSocketClient *client, 
    AwsEventType type, 
    void *arg, 
    uint8_t *data, 
    size_t len
) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        client->text(broadcaster_getBroadcastMessage());
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            data[len] = 0; // Null-terminate the string
            Serial.printf("Received message: %s\n", (char*)data);
        }
    }
}

void initWs(AsyncWebServer* server) {
     ws.onEvent(onWsEvent);
     server->addHandler(&ws);
}

void wsTick() {
    uint32_t now = millis();
    if (now - lastBroadcastTimestamp < 1000)
        return;
    lastBroadcastTimestamp = now;    
    ws.textAll(broadcaster_getBroadcastMessage());
    ws.cleanupClients();
}