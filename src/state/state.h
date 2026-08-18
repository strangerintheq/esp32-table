#pragma once

#include <Arduino.h>
#include <types/point.h>
#include <vector>

extern size_t pointsCount;
extern int32_t targetPointIndex;
extern Point targetPoint;

void initState();

void steppersMoveFinished();

void stateTick();
