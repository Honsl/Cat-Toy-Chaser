#include "MotorManager.h"

extern volatile long encoderCount;  // read from .ino

static long lastEncoderCount = 0;
static uint8_t stallCounter = 0;
bool isChangingSpeed = false;
uint32_t rampFinishedTime = 0;
bool pendingDirectionChange = false;
bool newDirection = true;

void MotorManager::update(uint8_t speed, bool directionRight) {

  if (toyPinned) speed = 0;
  if (directionRight != lastDirectionRight && !pendingDirectionChange) {
    newDirection = directionRight;
    pendingDirectionChange = true;
  }

  if (pendingDirectionChange) {
    if (currentSpeed > 0) {
     // isChangingSpeed = true;
     // stallCounter = 0;
     // rampToSpeed(0, lastDirectionRight);
     speed =0;
     directionRight = lastDirectionRight;
     // return;
    } else {
      lastDirectionRight = newDirection;
      pendingDirectionChange = false;
    }
  }

  if (speed != currentSpeed) {
    isChangingSpeed = true;
    stallCounter = 0;
    rampToSpeed(speed, directionRight);
  } else {
    if (isChangingSpeed) {
      rampFinishedTime = millis();
    }  // mark end of ramp
    isChangingSpeed = false;
  }
  checkStall(speed);
}
// void MotorManager::applyDirection(bool directionRight) {
//   if (directionRight) {
//     // Right direction: IN1 = LOW, IN2 = PWM
//     digitalWrite(MOTOR_IN1, LOW);

//   } else {
//     // Left direction: IN2 = LOW, IN1 = PWM
//     digitalWrite(MOTOR_IN2, LOW);
//   }
// }
void MotorManager::rampToSpeed(uint8_t targetSpeed, bool directionRight) {
  uint32_t now = millis();
  if (now - lastRampUpdate < 10) return;
  lastRampUpdate = now;

  if (currentSpeed < targetSpeed) currentSpeed++;
  else if (currentSpeed > targetSpeed) currentSpeed--;

  if (directionRight) {
    analogWrite(MOTOR_IN2, currentSpeed); 
    analogWrite(MOTOR_IN1, 0);             
  } else {
    analogWrite(MOTOR_IN1, currentSpeed);  
    analogWrite(MOTOR_IN2, 0);             
  }
}

static uint32_t lastCheck = 0;
void MotorManager::checkStall(uint8_t pwm) {

  uint32_t now = millis();
  if (now - lastCheck < 20) return;
  lastCheck = now;


  long current = encoderCount;
  long delta = abs(current - lastEncoderCount);
  if (isChangingSpeed) {
    Serial.print("changing speed");
    stallCounter = 0;
    lastEncoderCount = current;
    return;
  }
  if (millis() - rampFinishedTime < 1000) {
    lastEncoderCount = current;
    stallCounter = 0;
    return;
  }



  Serial.print("Delta");
  Serial.println(delta);

  int minDiff = 0;
  int speedPct = map(currentSpeed, 0, 255, 0, 100);
  Serial.print("Speed");
  Serial.println(speedPct);
  if (speedPct < 32) {
    minDiff = 20;
  } else if (speedPct < 45) {
    minDiff = 40;
  } else if (speedPct < 50) {
    minDiff = 45;
  } else if (speedPct < 70) {
    minDiff = 80;
  } else if (speedPct < 90) {
    minDiff = 90;
  } else if (speedPct < 100) {
    minDiff = 140;
  }
  if (delta < minDiff && delta != 0) {
    stallCounter++;
  } else {
    stallCounter = 0;
  }
  lastEncoderCount = current;
  if (stallCounter > 5) {
    Serial.println("stalled");
    // Stall detected → pause
    toyPinned = true;
    toyPinnedEndTime = millis() + 5000;
  
    stallCounter = 0;
  }
  if (toyPinnedEndTime < now) {
    toyPinned = false;
  }
}
