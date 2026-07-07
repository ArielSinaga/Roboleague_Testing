#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

extern SemaphoreHandle_t ControllerData_Mutex;
extern SemaphoreHandle_t MotorData_Mutex;

extern SemaphoreHandle_t I2CBus_Mutex;
