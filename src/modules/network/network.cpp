#include <Arduino.h>
#include <WiFi.h>
#include <modules/storage/storage.h>
#include <modules/network/networkConfig.h>

const uint32_t DELAY_BETWEEN_WIFI_CONNECTION_CHECKS = 500;
const uint32_t WIFI_CONNECTION_FALLBACK_TIMEOUT = 5000;

bool wifiConnecting = false;
bool wifiConnected = false;
// bool wifiConnectionFailed = false;

uint32_t wifiConnectingLastCheckTimestamp = 0;
uint32_t wifiConnectingStartTimestamp = 0;

void connectToWifi(String ssid, String password) {
  Serial.println("Connecting to: " + ssid + " password:" + password);
  WiFi.begin(ssid, password);
  wifiConnectingStartTimestamp = millis();
  wifiConnecting = true;
}

void createAccessPoint() {
  String ssid = ap_ssid->get();
  String password = ap_password->get();
  if (ssid == "")
    ssid = "ESP32";
  Serial.print("Creating wifi access point ssid: ");
  Serial.print(ssid);
  Serial.print(", password: ");
  Serial.print(password);

  if (password == "") {
    WiFi.softAP(ssid);
  } else {
    WiFi.softAP(ssid, password);
  }

  Serial.println();
  Serial.print("access point IP: ");
  Serial.println(WiFi.softAPIP());
}

void networkTick() {
  // if (wifiConnectionFailed) {

  // }
  if (!wifiConnecting) {
    return;
  }
  uint32_t now = millis();
  if (now - wifiConnectingLastCheckTimestamp < DELAY_BETWEEN_WIFI_CONNECTION_CHECKS) {
    return;
  }
  if (now - wifiConnectingStartTimestamp > WIFI_CONNECTION_FALLBACK_TIMEOUT) {
    // wifiConnectionFailed = true;
    wifiConnecting = false;
    Serial.println("failed");
    WiFi.disconnect();
    createAccessPoint();
    return;
  }
  if (WiFi.status() != WL_CONNECTED) {
    wifiConnectingLastCheckTimestamp = now;
    Serial.print(".");
    return;
  }
  wifiConnecting = false;
  wifiConnected = true;
  Serial.print("\nConnected to wifi network: ");
  Serial.print(wifi_ssid->get());
  Serial.print(" ip: ");
  Serial.println(WiFi.localIP());
}

void initNetwork() {

    if (network_mode->get() == "wifi") {
      if (wifi_ssid->get() != "") {
        connectToWifi(wifi_ssid->get(), wifi_password->get());
        return;
      }
    }

    createAccessPoint();  
}