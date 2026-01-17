#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

#include <Arduino.h>

#define IR_RECEIVE_PIN 14
#define IR_SEND_PIN 16
#define TONE_PIN 17
#define APPLICATION_PIN 18
#define ALTERNATIVE_IR_FEEDBACK_LED_PIN 19
#define _IR_TIMING_TEST_PIN 20

#define MOTOR_IN1 6  //IE pins 9/10
#define MOTOR_IN2 7
#define ENC_A 17
#define ENC_B 16

constexpr uint16_t manual_mode_ADDR = 0xF000;
constexpr uint16_t auto_mode_ADDR = 0xF111;

#endif