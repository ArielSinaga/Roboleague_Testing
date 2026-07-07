#include "tasks_main_deneyap/ARM.hpp"
#include "tasks_main_deneyap/CTRL.hpp"

Arm_data_t          myArm   = {90, 90, 90, 90, 90};
volatile Arm_data_t armData = {90, 90, 90, 90, 90};   // catatan: belum dipakai/ditulis di task manapun

Servo Servos[5];                          // di-attach() ke pin di setup(), main_deneyap.cpp
int   servoAngles[5] = {90, 90, 90, 90, 90};

const int idleServoAnglesUp[5]   = {90, 90, 90, 90, 90};
const int idleServoAnglesDown[5] = {90, 90, 90, 90, 90};

void ARM_Task(void *pvParameters) {
    controller_data_t CtlData_copy;
    bool              is_connected;
    static int        currentY = 90;   // posisi Y terakhir, persistent antar iterasi

    for (;;) {
        // ── Salin data bersama secara atomik (termasuk connection_active) ──────
        if (xSemaphoreTake(ControllerData_Mutex, portMAX_DELAY) == pdTRUE) {
            memcpy((void *)&CtlData_copy, (const void *)&CtlData, sizeof(CtlData_copy));
            is_connected = connection_active;   // baca di dalam mutex, bukan di luar
            xSemaphoreGive(ControllerData_Mutex);
        }

        if (is_connected) {
            // ── Ekstrak status sakelar menggunakan BIT_* dari CTRL.hpp ─────────
            bool sa_on = (CtlData_copy.button & BIT_SA);
            bool sb_up = (CtlData_copy.button & BIT_SB1);
            bool sb_dn = (CtlData_copy.button & BIT_SB2);
            bool sc_up = (CtlData_copy.button & BIT_SC1);
            bool sc_dn = (CtlData_copy.button & BIT_SC2);

            bool sc_active = (sc_up || sc_dn);   // SC sedang tidak di tengah
            bool sb_active = (sb_up || sb_dn);   // SB sedang tidak di tengah

            // ── SA saja = Pinch ───────────────────────────────────────────────
            // Tidak aktif saat SC dipakai agar tidak bentrok saat atur Y
            if (sa_on && !sc_active) {
                myArm.pinch = 150;   // gripper menutup
            } else {
                myArm.pinch = 90;    // gripper membuka
            }

            // ── SC aktif = kendali X, Y, Z ────────────────────────────────────
            if (sc_active) {
                // SC + Right Gimbal Y = sendi X
                // Axis range CTRL: -512..512  →  servo: 0..180°
                myArm.x = (uint8_t)map(CtlData_copy.axisRY, -512, 512, 0, 180);

                // SC + Left Gimbal Y = sendi Z
                myArm.z = (uint8_t)map(CtlData_copy.axisLY, -512, 512, 0, 180);

                // SC + SB = Y  (incremental, bukan langsung dari gimbal)
                if (sb_up) {
                    currentY += 2;
                } else if (sb_dn) {
                    currentY -= 2;
                }
                currentY  = constrain(currentY, 0, 180);
                myArm.y   = (uint8_t)currentY;
            }

            // ── SB saja (SC tidak aktif) = Rotasi ─────────────────────────────
            if (sb_active && !sc_active) {
                myArm.rotasi = (uint8_t)map(CtlData_copy.axisRX, -512, 512, 0, 180);
            }

            Servos[0].write(myArm.rotasi);
            Servos[1].write(myArm.x);
            Servos[2].write(myArm.y);
            Servos[3].write(myArm.z);
            Servos[4].write(myArm.pinch);
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}