#include <Arduino.h>
#include <types/point.h>
#include <modules/network/network.h>
#include <modules/steppers/steppers.h>
#include <modules/server/server.h>
#include <modules/led/led.h>
#include <modules/storage/storage.h>
#include <state/state.h>

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n   ___ ___ ___ _______");
  Serial.println("  | __/ __| _ \\__ /_  )");
  Serial.println("  | _|\\__ \\  _/|_ \\/ / ");
  Serial.println("  |___|___/_| |___/___|\n\n");
  if (!initStorage()) 
    return;
  initLed();
  initSteppers();
  initNetworkConfiguration();
  initNetwork();
  startWebServer();
  initState();
  blink(5000);
}

void loop() {
  networkTick();
  ledTick();
  steppersTick();
  stateTick();
  

  // Point p = points[pointIndex];
  // movePolar(p.a, p.r);
  // pointIndex = (pointIndex + 1) % points.size();
}

