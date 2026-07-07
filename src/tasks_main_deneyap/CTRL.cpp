#include "tasks_main_deneyap/CTRL.hpp"


SemaphoreHandle_t ControllerData_Mutex = NULL;   // di-create di setup(), main_deneyap.cpp
SemaphoreHandle_t MotorData_Mutex      = NULL;   // di-create di setup(), main_deneyap.cpp

volatile bool          connection_active     = false;
volatile unsigned long last_packet_timestamp = 0;
volatile controller_data_t CtlData           = {0};
volatile MotorData_t       motorData         = {0};

CRSFforArduino *crsf = nullptr;

static HardwareSerial ELRS_Serial(2);

void initCRSF() {
    crsf = new CRSFforArduino(&ELRS_Serial, CRSF_RX_PIN, CRSF_TX_PIN);

    if (!crsf->begin()) {
        Serial.println("CRSF gagal init! Cek wiring RX/TX & baudrate receiver.");
        delete crsf;
        crsf = nullptr;   // biar CTRL_Task tetap aman (skip blok crsf) kalau init gagal
        return;
    }

    Serial.println("CRSF init OK. Menunggu bind ke RadioMaster...");
}

void CTRL_Task(void *pvParameters) {
    const unsigned long CONNECTION_TIMEOUT_MS = 500;

    for (;;) {
        if (crsf != nullptr) {
            crsf->update();
        }

        unsigned long current_time = millis();

        if (connection_active &&
            (current_time - last_packet_timestamp > CONNECTION_TIMEOUT_MS)) {

            connection_active = false;
            Serial.println("Connection lost. Entering safe mode.");

            if (xSemaphoreTake(ControllerData_Mutex, portMAX_DELAY) == pdTRUE) {
                memset((void *)&CtlData, 0, sizeof(CtlData));
                xSemaphoreGive(ControllerData_Mutex);
            }
        }

        if (crsf != nullptr) {
            if (xSemaphoreTake(ControllerData_Mutex, pdMS_TO_TICKS(10)) == pdTRUE) {

                last_packet_timestamp = current_time;

                if (!connection_active) {
                    connection_active = true;
                    Serial.println("Koneksi CRSF terhubung.");
                }

                CtlData.axisRX = (int16_t)map(crsf->rcToUs(crsf->getChannel(1)), 1000, 2000, -512, 512);
                CtlData.axisRY = (int16_t)map(crsf->rcToUs(crsf->getChannel(2)), 1000, 2000, -512, 512);
                CtlData.axisLY = (int16_t)map(crsf->rcToUs(crsf->getChannel(3)), 1000, 2000, -512, 512);
                CtlData.axisLX = (int16_t)map(crsf->rcToUs(crsf->getChannel(4)), 1000, 2000, -512, 512);

                uint16_t tempButton = 0;

                // SA — ch5, 2-posisi
                if (crsf->rcToUs(crsf->getChannel(5)) > 1500)
                    tempButton |= BIT_SA;

                // SB — ch6, 3-posisi (>1700 = atas, 1300-1700 = tengah, <1300 = bawah)
                {
                    uint16_t SBRaw = crsf->rcToUs(crsf->getChannel(6));
                    if      (SBRaw > 1700) tempButton |= BIT_SB1;  // atas
                    else if (SBRaw > 1300) tempButton |= BIT_SB2;  // tengah→bawah
                }

                // SC — ch7, 3-posisi
                {
                    uint16_t SCRaw = crsf->rcToUs(crsf->getChannel(7));
                    if      (SCRaw > 1700) tempButton |= BIT_SC1;  // atas
                    else if (SCRaw > 1300) tempButton |= BIT_SC2;  // tengah→bawah
                }

                // SD — ch8, 2-posisi
                if (crsf->rcToUs(crsf->getChannel(8)) > 1500)
                    tempButton |= BIT_SD;

                CtlData.button = tempButton;

                xSemaphoreGive(ControllerData_Mutex);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    if (data_len == sizeof(controller_data_t)) {
        if (xSemaphoreTake(ControllerData_Mutex, 0) == pdTRUE) {
            memcpy((void *)&CtlData, data, sizeof(CtlData));
            last_packet_timestamp = millis();
            connection_active     = true;
            xSemaphoreGive(ControllerData_Mutex);
        }
    }
}

void onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t linkStatistics) {
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 1000) {
        lastPrint = millis();
        Serial.printf("RSSI: %d | LQI: %d | SNR: %d\n",
                      linkStatistics.rssi, linkStatistics.lqi, linkStatistics.snr);
    }
}