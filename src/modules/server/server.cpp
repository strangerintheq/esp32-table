#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <modules/temp/temp.h>
#include <types/point.h>
#include <modules/storage/storage.h>
#include <modules/network/networkConfig.h>
#include <state/state.h>
#include <utils/JsonBuilder.h>
#include "ws.h"
#include <ArduinoJson.h>
#include <state/fsm.h>

AsyncWebServer server(80);

const char* single_page_routes[] = {
    "/network",
    "/generator",
};

const SystemSignal signal_routes[] = {
    SystemSignal::START,
    SystemSignal::STOP,
    SystemSignal::PAUSE,
    SystemSignal::REBOOT,
    SystemSignal::CLEAR,
};

void sendOk(AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"result\":\"ok\"}"); 
}

void startWebServer() {

  for (const char* route : single_page_routes) {
    server.on(route, HTTP_GET, [](AsyncWebServerRequest *request) { 
        request->send(LittleFS, "/site/index.html", "text/html"); 
    });
  }

  server.serveStatic("/", LittleFS, "/site/")
    .setDefaultFile("/index.html")
    .setCacheControl("public, max-age=31536000, immutable");

  for (const SystemSignal sig : signal_routes) {
    String route = "/api/signal/" + systemSignalName(sig);
    route.toLowerCase();
    server.on(route.c_str(), HTTP_POST, [sig](AsyncWebServerRequest *request) { 
        sendSignal(sig);
        sendOk(request);
    });
  }

  server.on("/api/network/get", HTTP_POST, [](AsyncWebServerRequest *request) { 
      JsonBuilder<512> b;
      b.append("network_mode", network_mode->get());
      b.append("wifi_ssid", wifi_ssid->get());
      b.append("wifi_password", wifi_password->get());
      b.append("ap_ssid", ap_ssid->get());
      b.append("ap_password", ap_password->get());
      request->send(200, "application/json", b.c_str());
  });


  server.on("api/network/set", HTTP_POST, 
    [](AsyncWebServerRequest *request) {
        sendOk(request);
    },
    NULL, 
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, data, len);
        if (!error) {
            if (doc["network_mode"].is<String>())  
                network_mode->write(doc["network_mode"].as<String>());
            if (doc["wifi_ssid"].is<String>())     
                wifi_ssid->write(doc["wifi_ssid"].as<String>());
            if (doc["wifi_password"].is<String>()) 
                wifi_password->write(doc["wifi_password"].as<String>());
            if (doc["ap_ssid"].is<String>())       
                ap_ssid->write(doc["ap_ssid"].as<String>());
            if (doc["ap_password"].is<String>())   
                ap_password->write(doc["ap_password"].as<String>());
            ESP.restart();
        }
    }
  );

  String pointsBin = "/points.bin";

  server.on("/api/upload", HTTP_POST, [pointsBin](AsyncWebServerRequest *request) {
      endWrite(pointsBin);
      //initState(); 
      sendOk(request);
  }, NULL, [pointsBin](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      if (index == 0) {
          // TODO: STOP JOB
          startWrite(pointsBin);
      }
      writeBytes(pointsBin, data, len);
  });

  server.on("/api/points", HTTP_POST, [](AsyncWebServerRequest *request) {
     request->send(LittleFS, "/points.bin", "application/octet-stream");
  });

  initWs(&server);

  server.begin();
}