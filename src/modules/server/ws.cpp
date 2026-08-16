#include <ESPAsyncWebServer.h>
#include <utils/JsonBuilder.h>
#include <modules/temp/temp.h>
#include <state.h>

AsyncWebSocket ws("/ws"); 

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
        JsonBuilder<128> b;
        b.append("status", "connected");
        b.append("targetPointIndex", targetPointIndex);
        client->text(b.c_str());
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

void broadcast(String data) {
    ws.cleanupClients();
    ws.textAll(data.c_str());
}

void broadcastStatus() {
    // Clean up disconnected clients from memory automatically
    ws.cleanupClients();
    JsonBuilder<256> b;
    b.append("uptime", (millis() / 1000));
    b.append("temp", getTemp(), 2);
    b.append("freeHeap", ESP.getFreeHeap());
    broadcast(b.c_str());
}

void initWs(AsyncWebServer* server) {
     ws.onEvent(onWsEvent);
     server->addHandler(&ws);
}

