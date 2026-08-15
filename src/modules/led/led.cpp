#include <Arduino.h>

static const int ledPin = 2;
const int ledChannel = 0;   // ШИМ канал (от 0 до 15)
const int freq = 5000;      // Частота ШИМ (5 кГц)
const int resolution = 8;   // Разрешение 8 бит (значения от 0 до 255)

static uint32_t blinkStartTimestamp = 0;
static uint32_t blinkEndTimestamp = 0;

void turnOffLed() {
    ledcWrite(ledChannel, 0);
}

void turnOnLed() {
    ledcWrite(ledChannel, 255);
}

void initLed() {
    ledcSetup(ledChannel, freq, resolution); // Настраиваем ШИМ-канал
    ledcAttachPin(ledPin, ledChannel);  // Привязываем физический пин к ШИМ-каналу
    turnOffLed();
}

void blink(uint32_t delayMillis) {
    blinkStartTimestamp = millis();
    blinkEndTimestamp = blinkStartTimestamp + delayMillis;
}

void ledTick() {
    uint32_t now = millis();    
    if (now > blinkStartTimestamp && now < blinkEndTimestamp) {
        uint32_t t = now - blinkStartTimestamp;
        float s = ((float) t) / 314.6;
        float b = (1.0 - cosf(s)) * 127.5;
        ledcWrite(ledChannel, (uint32_t) b);
    } else {
        turnOffLed();
    }
}
