#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

extern TaskHandle_t CTRL_task_Handler;

extern TaskHandle_t I2C_Sub_Task_Handler;
extern TaskHandle_t I2C_OLED_Task_Handler;

extern TaskHandle_t RFID_Task_Handler;

extern TaskHandle_t ARM_Task_Handler;
extern TaskHandle_t Zipper_Task_Handler;




