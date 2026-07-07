#include <tasks_main_deneyap/RFID.hpp>

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
QueueHandle_t rfidQueue = NULL;
SemaphoreHandle_t rfidMutex = NULL;
RfidMessage sharedRfidData;

bool readBlock(byte blockNumber, byte *outputBuffer) {
    MFRC522::StatusCode status = mfrc522.PCD_Authenticate(
        MFRC522::PICC_CMD_MF_AUTH_KEY_A, 
        blockNumber, 
        &key, 
        &(mfrc522.uid)
    );

    if (status != MFRC522::STATUS_OK) {
        return false;
    }
    
    // Ukuran buffer MIFARE harus minimal 18 byte (16 byte data + 2 byte CRC)
    byte size = 18;
    status = mfrc522.MIFARE_Read(blockNumber, outputBuffer, &size);
    
    if (status != MFRC522::STATUS_OK) {
        return false;
    }

    return true;
}

void initRFID() {
    // 1. Hardware Initialization
    SPI.begin();
    mfrc522.PCD_Init();
    
    byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
    if (version == 0x00 || version == 0xFF) {
        Serial.println("RFID Hardware: FAILED (Check SPI wiring)");
    } else {
        Serial.print("RFID Hardware: OK. Chip Version: 0x");
        Serial.println(version, HEX);
    }

    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
    
}

void RFID_Task(void *pvParameters) {
    byte rawBuffer[18];
    RfidMessage localMsg;
    initRFID();
    for (;;) {
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
            if (readBlock(4, rawBuffer)) {
                localMsg.length = 0;
                memset(localMsg.payload, 0, MAX_RFID_PAYLOAD);

                for (uint8_t i = 0; i < 16 && i < (MAX_RFID_PAYLOAD - 1); i++) {
                    if (rawBuffer[i] >= 32 && rawBuffer[i] <= 126) {
                        localMsg.payload[localMsg.length++] = (char)rawBuffer[i];
                    }
                }
                localMsg.payload[localMsg.length] = '\0';

                // Send to queue
                xQueueSend(rfidQueue, &localMsg, 0);
            }
            mfrc522.PICC_HaltA();
            mfrc522.PCD_StopCrypto1();
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
