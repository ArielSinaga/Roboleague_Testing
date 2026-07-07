#pragma once
#include <stdint.h>

typedef struct MotorData {
    int16_t speed;   // -512..512, maju/mundur
    int16_t angle;   // -512..512, belok kiri/kanan
} MotorData_t;

extern volatile MotorData_t   lastCmd;
extern volatile unsigned long lastRecvMs;