#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <modules/temp/temp.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <types/point.h>
#include <vector>
#include <modules/storage/storage.h>
#include <modules/network/networkConfig.h>
#include <state.h>

AsyncWebServer server(80);

void startWebServer() {

  server.serveStatic("/", LittleFS, "/site/").setDefaultFile("index.html");

  server.on("/toggle", HTTP_POST, [](AsyncWebServerRequest *request) { 
    request->send(200, "text/plain", "ok"); 
  });

  server.on("/network/get", HTTP_POST, [](AsyncWebServerRequest *request) { 
    JsonDocument doc; 
    doc["network_mode"] = network_mode->get();
    doc["wifi_ssid"] = wifi_ssid->get();
    doc["wifi_password"] = wifi_password->get();
    doc["ap_ssid"] = ap_ssid->get();
    doc["ap_password"] = ap_password->get();
    String jsonResponse;
    serializeJson(doc, jsonResponse); 
    request->send(200, "appication/json", jsonResponse);
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

  server.on("/status", HTTP_POST, [](AsyncWebServerRequest *request) {
    JsonDocument doc; 
    doc["uptime"] = millis() / 1000;
    doc["temp"] = getTemp();
    doc["pointsCount"] = pointsCount;
    String jsonResponse;
    serializeJson(doc, jsonResponse); 
    request->send(200, "appication/json", jsonResponse);
  });

  server.on("/points", HTTP_POST, [](AsyncWebServerRequest *request) {
    JsonDocument doc; 
    JsonArray jsonArray = doc["points"].to<JsonArray>();
    for (const auto& p : pts) {
        jsonArray.add(p.a);
        jsonArray.add(p.r);
    }
    String jsonResponse;
    serializeJson(doc, jsonResponse); 
    request->send(200, "appication/json", jsonResponse);
  });

  server.begin();
}