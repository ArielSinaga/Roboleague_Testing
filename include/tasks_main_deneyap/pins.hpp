#pragma once
#include <stdint.h>

// Controller
#define CRSF_RX_PIN 16 
#define CRSF_TX_PIN 17 

// I2C (sub)
#define I2C_ADDR_1 0x01
#define I2C_ADDR_2 0x08

// RFID
#define RST_PIN 9
#define SS_PIN 10

const uint8_t servoPins[5] = {26, 22, 21, 14, 32};