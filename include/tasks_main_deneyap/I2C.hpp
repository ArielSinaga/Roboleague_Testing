#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <tasks_main_deneyap/SharedData.hpp>
#include <tasks_main_deneyap/pins.hpp>
#include <tasks_main_deneyap/SemaQHandlers.hpp>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64




extern QueueHandle_t oledQueue;

void I2C_Sub_Task(void *pvParameters);
void I2C_OLED_Task(void *pvParameters);

