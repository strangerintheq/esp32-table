#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <modules/temp/temp.h>
#include <types/point.h>
#include <modules/storage/storage.h>
#include <modules/network/networkConfig.h>
#include <state.h>
#include <utils/JsonBuilder.h>
#include "ws.h"

AsyncWebServer server(80);


void startWebServer() {

  server.serveStatic("/", LittleFS, "/site/").setDefaultFile("index.html");

  server.on("/toggle", HTTP_POST, [](AsyncWebServerRequest *request) { 
    request->send(200, "text/plain", "ok"); 
  });

  server.on("/network/get", HTTP_POST, [](AsyncWebServerRequest *request) { 
      JsonBuilder<512> b;
      b.append("network_mode", network_mode->get());
      b.append("wifi_ssid", wifi_ssid->get());
      b.append("wifi_password", wifi_password->get());
      b.append("ap_ssid", ap_ssid->get());
      b.append("ap_password", ap_password->get());
      request->send(200, "application/json", b.c_str());
  });

  String pointsBin = "/points.bin";

  server.on("/upload", HTTP_POST, [pointsBin](AsyncWebServerRequest *request) {
      endWrite(pointsBin);
      initState();
      request->send(200, "application/json", "{\"result\":\"saved_to_flash\"}");
  }, NULL, [pointsBin](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      if (index == 0) {
          startWrite(pointsBin);
      }
      writeBytes(pointsBin, data, len);
  });

  // server.on("/status", HTTP_POST, [](AsyncWebServerRequest *request) {
  //   // JsonBuilder<512> b;
  //   // b.append("uptime", millis() );
  //   // b.append("temp", getTemp());
  //   // b.append("pointsCount", pointsCount);
  //   // b.append("heapSize", ESP.getHeapSize());
  //   // b.append("freeHeap", ESP.getFreeHeap());
  //   request->send(200, "application/json", "{}");
  // });

  server.on("/points", HTTP_POST, [](AsyncWebServerRequest *request) {
     request->send(LittleFS, "/points.bin", "application/octet-stream");
  });


  initWs(&server);

  server.begin();
}