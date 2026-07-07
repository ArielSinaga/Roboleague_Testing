#include <tasks_main_deneyap/I2C.hpp>
#include <tasks_main_deneyap/RFID.hpp>   
#include <tasks_main_deneyap/pins.hpp>   

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Timeout kalau sub-controller telat/gagal ACK berturut-turut (safety)
static uint8_t motorFailCount = 0;
static const uint8_t MAX_I2C_FAIL = 5;

// ── Helper: kirim struct MotorData_t mentah ke alamat I2C tujuan ───────
static bool sendMotorDataI2C(uint8_t i2cAddr, const MotorData_t &data) {
    bool ok = false;

    // Lindungi bus I2C, soalnya dipakai bareng OLED (I2C_OLED_Task)
    if (xSemaphoreTake(I2CBus_Mutex, pdMS_TO_TICKS(20)) == pdTRUE) {
        Wire.beginTransmission(i2cAddr);
        Wire.write((const uint8_t *)&data, sizeof(MotorData_t));
        uint8_t err = Wire.endTransmission();
        ok = (err == 0);
        xSemaphoreGive(I2CBus_Mutex);
    }

    return ok;
}

// ── Kirim command motor ke satu-satunya sub-controller roda ────────────
bool MotorSend(const MotorData_t &data) {
    bool ok = sendMotorDataI2C(I2C_ADDR_1, data);

    if (!ok) {
        motorFailCount++;
        if (motorFailCount >= MAX_I2C_FAIL) {
            Serial.println("I2C: Motor sub-controller tidak merespon (cek wiring/alamat)");
            motorFailCount = MAX_I2C_FAIL;   // clamp, biar counter gak overflow
        }
    } else {
        motorFailCount = 0;
    }

    return ok;
}

void I2C_Sub_Task(void *pvParameters) {
 
    controller_data_t CtlData_copy;
    MotorData_t       motorData_copy;

    for (;;) {
        // ── Ambil data controller terbaru secara atomik ─────────────────
        bool is_connected;
        if (xSemaphoreTake(ControllerData_Mutex, portMAX_DELAY) == pdTRUE) {
            memcpy((void *)&CtlData_copy, (const void *)&CtlData, sizeof(CtlData_copy));
            is_connected = connection_active;
            xSemaphoreGive(ControllerData_Mutex);
        }

        if (!is_connected) {
            // Safety: kalau controller putus, motor harus berhenti total
            motorData_copy.speed = 0;
            motorData_copy.angle = 0;
        } else {
            // axisLY = maju/mundur, axisRX = belok (arcade drive)
            // Deadzone kecil biar gimbal yang gak pas center gak bikin motor nyala sendiri
            motorData_copy.speed = (CtlData_copy.axisLY >= 10 || CtlData_copy.axisLY <= -10)
                                        ? CtlData_copy.axisLY
                                        : 0;

            motorData_copy.angle = (CtlData_copy.axisRX >= 10 || CtlData_copy.axisRX <= -10)
                                        ? CtlData_copy.axisRX
                                        : 0;
        }

        // ── Kirim ke 1 sub-controller (handle ke-4 motor roda) ──────────
        MotorSend(motorData_copy);

        // ── Simpan salinan ke shared data (buat OLED/monitoring) ────────
        if (xSemaphoreTake(MotorData_Mutex, portMAX_DELAY) == pdTRUE) {
            memcpy((void *)&motorData, (const void *)&motorData_copy, sizeof(motorData));
            xSemaphoreGive(MotorData_Mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(40));   // ~25 Hz, cukup untuk drive motor
    }
}

void I2C_OLED_Task(void *pvParameters){
    RfidMessage receivedMsg;

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 init failed");
        vTaskDelete(NULL);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    for (;;) {
        if (xQueueReceive(rfidQueue, &receivedMsg, pdMS_TO_TICKS(100)) == pdTRUE) {
            // Lindungi bus I2C, soalnya dipakai bareng pengiriman data motor
            if (xSemaphoreTake(I2CBus_Mutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                display.clearDisplay();

                display.setCursor(0, 0);
                display.println(receivedMsg.payload);

                display.setCursor(0, 16);
                display.println(receivedMsg.payload);

                display.display();  // push buffer to screen

                xSemaphoreGive(I2CBus_Mutex);
            }
        }
    }
}