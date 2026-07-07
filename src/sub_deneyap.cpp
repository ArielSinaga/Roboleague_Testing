#include <Arduino.h>
#include <Wire.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <tasks_sub_deneyap/sub_SemaQHandlers.hpp>
#include <tasks_sub_deneyap/sub_TaskHandlers.hpp>
#include <tasks_sub_deneyap/sub_SharedData.hpp>
#include <tasks_sub_deneyap/sub_pins.hpp>
#include <tasks_sub_deneyap/sub_I2C.cpp>
#include <tasks_sub_deneyap/Motor.cpp>

SemaphoreHandle_t MotorCmd_Mutex   = NULL;
TaskHandle_t      Motor_Task_Handler = NULL;
TaskHandle_t      I2C_Task_Handler   = NULL;

void setup() {
    Serial.begin(115200);

    MotorCmd_Mutex = xSemaphoreCreateMutex();

    setupMotorPins();   

    xTaskCreatePinnedToCore(Motor_Task, "Motor_Task", 2048, NULL, 3, &Motor_Task_Handler, 1);
    xTaskCreatePinnedToCore(I2C_Task, "I2C_Task", 2048, NULL, 1, &I2C_Task_Handler, 1);
    
    if (Motor_Task_Handler == NULL) {
        Serial.println("ERROR: Motor_Task gagal dibuat! Cek ukuran stack/heap.");
    }
    if (I2C_Task_Handler == NULL) {
        Serial.println("ERROR: I2C_Task gagal dibuat! Cek ukuran stack/heap.");
    }

    Serial.println("Sub-controller ready.");
}

void loop() {}