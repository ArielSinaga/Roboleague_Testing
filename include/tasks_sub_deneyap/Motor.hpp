#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <tasks_sub_deneyap/sub_pins.hpp>
#include <tasks_sub_deneyap/sub_SharedData.hpp>
#include <tasks_sub_deneyap/sub_SemaQHandlers.hpp>

void setupMotorPins();

void stopMotors();

void Motor_Task(void *pvParameters);