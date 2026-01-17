#include "ChaseManager.h"



static const uint8_t SPEED_NONE = 0;
static const uint8_t SPEED_SLOW = 80;
static const uint8_t SPEED_MEDIUM = 127;
static const uint8_t SPEED_FAST = 255;

void ChaseManager::updateState(uint16_t address, uint16_t command) {
  switch (address) {
    case auto_mode_ADDR:
      autoMode.time = static_cast<ModeDuration>(command & 0b00000011);
      autoMode.mode = static_cast<ModeType>((command >> 2) & 0b00000011);
      isManualMode = false;
      Serial.print("Time");
      Serial.println(autoMode.time);
      Serial.print("Mode");
      Serial.println(autoMode.mode);
      autoStartTime = millis();
      lastSpeedUpdate = millis();
      lastDirectionChange = millis();
      nextDirectionInterval = random(2000, 20000);
      nextSpeedInterval = random(500, 20000);

      break;
    case manual_mode_ADDR:{
      uint8_t speed = map(command & 0x7F, 0, 100, 0, 255);  // lower 7 bits
      if (speed > 0 && speed < 80) {
        speed = 80;
      }
      chaseMode.speed = speed;
      chaseMode.directionRight = command >> 7;  // MSB
      //Serial.print("speed");
      //Serial.println(chaseMode.speed);
     // Serial.print("Direction");
     // Serial.println(chaseMode.directionRight);
      isManualMode = true;
      break;
    }
    default:
      break;
  }
}


void ChaseManager::updateTimeState() {
  bool updateMotor = false;
  uint32_t now = millis();

  // 1. STOP WHEN TIME EXPIRES
  uint32_t durationMs = 0;

  switch (autoMode.time) {
    case DURATION_1_MIN: durationMs = 1UL * 60UL * 1000UL; break;
    case DURATION_3_MIN: durationMs = 3UL * 60UL * 1000UL; break;
    case DURATION_5_MIN: durationMs = 5UL * 60UL * 1000UL; break;
    case DURATION_10_MIN: durationMs = 10UL * 60UL * 1000UL; break;
  }

  if (now - autoStartTime >= durationMs) {
    autoMode.speed = 0;  // stop motor
    return;
  }

  // 2. RANDOM DIRECTION CHANGES
  if (now - lastDirectionChange > nextDirectionInterval) {  // 2–20 seconds
    autoMode.directionRight = !autoMode.directionRight;
    lastDirectionChange = now;
    nextDirectionInterval = random(2000, 20000);
    updateMotor = true;
  }

  // 3. SPEED MODULATION BASED ON MODE
  if (now - lastSpeedUpdate > nextSpeedInterval) {  // update every 0.5s to 20 seconds
    lastSpeedUpdate = now;
    nextSpeedInterval = random(500, 20000);
    updateMotor = true;
    switch (autoMode.mode) {

      case MODE_SLOW:
        autoMode.speed = random(0, SPEED_SLOW);
        break;

      case MODE_MEDIUM:
        autoMode.speed = random(0, SPEED_MEDIUM);
        break;

      case MODE_FAST:
        autoMode.speed = random(0, SPEED_FAST);
        break;
    }
  }
}