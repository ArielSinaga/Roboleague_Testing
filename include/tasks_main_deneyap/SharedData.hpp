#pragma once

#include <stdint.h>



typedef struct controller_data {
    uint8_t dpad;
    uint8_t button;
    int16_t axisLX;
    int16_t axisLY;
    int16_t axisRX;
    int16_t axisRY;
    uint16_t leftTrig;
    uint16_t rightTrig;
} controller_data_t;

typedef struct MotorData {
    int16_t speed;
    int16_t angle;
} MotorData_t;

#define MAX_RFID_PAYLOAD 32 
struct RfidMessage {
    char payload[MAX_RFID_PAYLOAD]; 
    uint8_t length;
};


extern volatile controller_data_t CtlData;
extern volatile MotorData_t motorData;

extern volatile bool connection_active;
