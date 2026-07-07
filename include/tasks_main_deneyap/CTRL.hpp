#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "CRSFforArduino.hpp"
#include <tasks_main_deneyap/SharedData.hpp>
#include <tasks_main_deneyap/TaskHandlers.hpp>
#include <tasks_main_deneyap/SemaQHandlers.hpp>
#include <string.h>
#include <tasks_main_deneyap/pins.hpp>

extern CRSFforArduino *crsf;

extern volatile unsigned long last_packet_timestamp;
extern volatile bool connection_active;

void initCRSF();

#define BIT_SA  (1 << 0)   // bit 0 — Switch A  (2-pos: off/on)
#define BIT_SB1 (1 << 1)   // bit 1 — Switch B  posisi atas  (3-pos)
#define BIT_SB2 (1 << 2)   // bit 2 — Switch B  posisi bawah (3-pos)
#define BIT_SC1 (1 << 3)   // bit 3 — Switch C  posisi atas  (3-pos)
#define BIT_SC2 (1 << 4)   // bit 4 — Switch C  posisi bawah (3-pos)
#define BIT_SD  (1 << 5)   // bit 5 — Switch D  (2-pos: off/on)

/**
 * @brief 
 */