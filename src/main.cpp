#include <Arduino.h>
#include <types/point.h>
#include <modules/network/network.h>
#include <modules/steppers/steppers.h>
#include <modules/server/server.h>
#include <modules/led/led.h>
#include <modules/storage/storage.h>
//#include <state.h>

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n ESP32 starting...");
  if (!initStorage()) 
    return;
  initLed();
  initSteppers();
  initNetworkConfiguration();
  initNetwork();
  startWebServer();
  blink(5000);
}

void loop() {
  networkTick();
  ledTick();
  steppersTick();

  // Point p = points[pointIndex];
  // movePolar(p.a, p.r);
  // pointIndex = (pointIndex + 1) % points.size();
}

