#include <tasks_main_deneyap/Zipper.hpp>

void Zipper_Task(void *pvParameters) {
    // kalian nanti bikin setup si stepper di sini, terus di loop-nya implementasi logika kontrol untuk zipper-nya
    for (;;) {
        // Implementasi logika kontrol untuk Zipper di sini
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}