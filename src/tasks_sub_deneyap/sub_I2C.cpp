#include "tasks_sub_deneyap/sub_I2C.hpp"

volatile MotorData_t   lastCmd    = {0, 0};
volatile unsigned long lastRecvMs = 0;

static void onI2CReceive(int numBytes) {
    if (numBytes != sizeof(MotorData_t)) {
        while (Wire.available()) Wire.read();
        return;
    }
    MotorData_t incoming;
    Wire.readBytes((uint8_t *)&incoming, sizeof(MotorData_t));
    if (xSemaphoreTake(MotorCmd_Mutex, 0) == pdTRUE) {
        memcpy((void *)&lastCmd, &incoming, sizeof(MotorData_t));
        lastRecvMs = millis();
        xSemaphoreGive(MotorCmd_Mutex);
    }
}

void initI2CSlave() {
    Wire.begin(MY_I2C_ADDR);
    Wire.onReceive(onI2CReceive);
    Serial.print("I2C slave siap. addr: 0x");
    Serial.println(MY_I2C_ADDR, HEX);
}

void I2C_Task(void *pvParameters) {
    initI2CSlave();
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}