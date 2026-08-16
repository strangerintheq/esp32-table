#include <AccelStepper.h>
#include <MultiStepper.h>
#include <state.h>

#define MOTOR_INTERFACE_TYPE 1

const int stepPin1 = 14;
const int dirPin1 = 27;
const int stepPin2 = 26;
const int dirPin2 = 25;

AccelStepper thetaStepper(MOTOR_INTERFACE_TYPE, stepPin1, dirPin1);
AccelStepper rhoStepper(MOTOR_INTERFACE_TYPE, stepPin2, dirPin2);

MultiStepper steppers;

Point stepperMovingToPoint;
long moveStartTimestamp;

void initSteppers() {
  thetaStepper.setMaxSpeed(1000);
  rhoStepper.setMaxSpeed(1000);
  steppers.addStepper(thetaStepper);
  steppers.addStepper(rhoStepper);
}

void movePolar(long targetTheta, long targetRho) {
  long motorPositions[2];

  // 1. Мотор Theta всегда отвечает только за вращение
  motorPositions[0] = targetTheta;

  // 2. Мотор Rho делает свои целевые шаги + компенсирует вращение Theta
  // Формула: Физический_Rho + (Физический_Theta * Коэффициент)
  motorPositions[1] = targetRho + round(targetTheta * 1);

  // Передаем скорректированные шаги в MultiStepper
  steppers.moveTo(motorPositions);
  
  // MultiStepper сам пересчитает скорости моторов так, 
  // чтобы они начали и закончили движение строго одновременно!
  steppers.runSpeedToPosition(); 
}

void steppersTick() {
  long now = millis();
  if (stepperMovingToPoint.a == targetPoint.a && stepperMovingToPoint.r == targetPoint.r) {
     if (now - moveStartTimestamp > 1000) {
        steppersMoveFinished();
     }
    return;
  }
  stepperMovingToPoint.a = targetPoint.a;
  stepperMovingToPoint.r = targetPoint.r;
  moveStartTimestamp = millis();
  Serial.printf("got new point a:%s r:%s\n", String(targetPoint.a), String(targetPoint.r));
}