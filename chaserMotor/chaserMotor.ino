#include <Arduino.h>
#include "pinDefinitions.h"
#include "IrManager.h"
#include "ChaseManager.h"
#include "MotorManager.h"
#include <stdint.h>
IrManager irManager;
ChaseManager chaseManager;
MotorManager motorManager;

volatile long encoderCount = 0;



void setup() {
  Serial.begin(115200);
  irManager.irBegin();

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC_A), handleEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), handleEncoderB, CHANGE);
}



void loop() {

  uint16_t addr, cmd;
  if (irManager.receiveIR(addr, cmd)) {
    chaseManager.updateState(addr, cmd);
  }
  if (!chaseManager.isManualMode) {
    chaseManager.updateTimeState();
    motorManager.update(chaseManager.autoMode.speed, chaseManager.autoMode.directionRight);
  } else {
    motorManager.update(chaseManager.chaseMode.speed, chaseManager.chaseMode.directionRight);
  }
 

}

void  handleEncoderA() {
  if (digitalRead(ENC_A) == digitalRead(ENC_B)) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}
void  handleEncoderB() {
  if (digitalRead(ENC_A) != digitalRead(ENC_B)) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}
