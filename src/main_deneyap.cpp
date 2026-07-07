#include <Arduino.h>
#include <Wire.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <tasks_main_deneyap/SemaQHandlers.hpp>
#include <tasks_main_deneyap/TaskHandlers.hpp>
#include <tasks_main_deneyap/SharedData.hpp>

#include <tasks_main_deneyap/I2C.cpp>
#include <tasks_main_deneyap/CTRL.cpp>
#include <tasks_main_deneyap/RFID.cpp>
#include <tasks_main_deneyap/ARM.cpp>
#include <tasks_main_deneyap/Zipper.cpp>

TaskHandle_t CTRL_task_Handler       = NULL;
TaskHandle_t I2C_Sub_Task_Handler    = NULL;
TaskHandle_t I2C_OLED_Task_Handler   = NULL;
TaskHandle_t RFID_Task_Handler       = NULL;
TaskHandle_t ARM_Task_Handler        = NULL;
TaskHandle_t Zipper_Task_Handler     = NULL;

SemaphoreHandle_t I2CBus_Mutex = NULL;

void setup(){
    Serial.begin(115200);
    Wire.begin();

    rfidMutex = xSemaphoreCreateMutex();
    rfidQueue = xQueueCreate(10, sizeof(RfidMessage));

    ControllerData_Mutex = xSemaphoreCreateMutex();
    MotorData_Mutex      = xSemaphoreCreateMutex();
    I2CBus_Mutex          = xSemaphoreCreateMutex();

    for (int i = 0; i < 5; i++) {
        Servos[i].attach(servoPins[i]);
        Servos[i].write(servoAngles[i]);   
    }

    initCRSF();   

    Serial.println("Ready.");
    delay(1000);

    xTaskCreatePinnedToCore(I2C_Sub_Task,   "I2C_Sub_Task",   4096, NULL, 4, &I2C_Sub_Task_Handler,  0);
    xTaskCreatePinnedToCore(I2C_OLED_Task,  "I2C_OLED_Task",  4096, NULL, 5, &I2C_OLED_Task_Handler, 0);
    xTaskCreatePinnedToCore(RFID_Task,      "RFID_Task",      2048, NULL, 3, &RFID_Task_Handler,     0);
    xTaskCreatePinnedToCore(ARM_Task,       "ARM_Task",       2048, NULL, 6, &ARM_Task_Handler,      1);
    xTaskCreatePinnedToCore(CTRL_Task,      "CTRL_Task",      2048, NULL, 7, &CTRL_task_Handler,     1);
    xTaskCreatePinnedToCore(Zipper_Task,    "Zipper_Task",    2048, NULL, 2, &Zipper_Task_Handler,   1);

    if (CTRL_task_Handler == NULL || I2C_Sub_Task_Handler == NULL ||
        I2C_OLED_Task_Handler == NULL || RFID_Task_Handler == NULL ||
        ARM_Task_Handler == NULL || Zipper_Task_Handler == NULL) {
        Serial.println("ERROR: Ada task yang gagal dibuat! Cek ukuran stack/heap.");
    }
}

void loop(){}