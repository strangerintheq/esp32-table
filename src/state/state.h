#pragma once

#include <Arduino.h>
#include <types/Point.h>
#include <vector>

extern size_t pointsCount;
extern int32_t targetPointIndex;
extern Point targetPoint;

void initState();

void steppersMoveFinished();

void stateTick();

void stopStateTicks();
