#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <tasks_main_deneyap/SharedData.hpp>

#include "pins.hpp"
#include "SemaQHandlers.hpp"
#include "SharedData.hpp"

typedef struct arm_data{
    uint8_t pinch;  
    uint8_t rotasi;
    uint8_t x;      
    uint8_t y;     
    uint8_t z;     
} Arm_data_t;

extern volatile Arm_data_t armData;

extern Arm_data_t myArm;


extern Servo Servos[5];
extern int servoAngles[5];
extern const int idleServoAnglesUp[5];
extern const int idleServoAnglesDown[5];
