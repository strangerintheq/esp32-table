#include <Arduino.h>
#include <modules/storage/storage.h>
#include <types/point.h>
#include <vector>

size_t pointsCount = 0;

std::vector<Point> pts;

void initState() {
    pts.clear(); 
    pointsCount = readBinaryFile("points.bin", sizeof(Point), [](const uint8_t* data) {
        // приводим указатель на байты к указателю на структуру Point
        const Point* p = reinterpret_cast<const Point*>(data);
        // Разыменовываем указатель (*p) и кладем копию объекта в вектор pts
        pts.push_back(*p);
    }) / sizeof(Point);
}
