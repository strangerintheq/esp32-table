#include <Arduino.h>
#include <modules/storage/storage.h>
#include <types/point.h>
#include <vector>
#include <LittleFS.h>

size_t pointsCount = 0;

std::vector<Point> pts;

void restorePoints1() {
    pts.clear(); 
    pointsCount = readBinaryFile("points.bin", sizeof(Point), [](const uint8_t* data) {
        Point* p;
        memcpy(&p, data, sizeof(Point));
        pts.push_back(*p);
        Serial.println(p->a);
    }) / sizeof(Point);
}

void restorePoints() {
  File file = LittleFS.open("/points.bin", "r");
  if (file) {
      pointsCount = file.size() / sizeof(Point); 
      Serial.println("points.bin: " + String(pointsCount));
      pts.clear(); 
      pts.reserve(pointsCount); 
      for (size_t i = 0; i < pointsCount; i++) {
          Point p;
          file.read((uint8_t*)&p, sizeof(Point));
          pts.push_back(p);
      }
      file.close();
  }
}