#pragma once

#include <SPI.h>
#include <MFRC522.h>
#include "SharedData.hpp"
#include "pins.hpp"


// DECLARATIONS ONLY
extern MFRC522 mfrc522;
extern MFRC522::MIFARE_Key key;
extern QueueHandle_t rfidQueue;
extern SemaphoreHandle_t rfidMutex;
extern RfidMessage sharedRfidData;

bool readBlock(byte blockNumber, byte *outputBuffer);
void RFID_Task(void *pvParameters);
void initRFID();
