#include "tasks_sub_deneyap/Motor.hpp"

// ── Set 1 motor: driver 2-pin arah (IN1/IN2) + 1 pin PWM ───────────────
static void setMotor(int in1, int in2, int pwmChannel, int value) {
    value = constrain(value, -255, 255);

    if (value > 0) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    } else if (value < 0) {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    } else {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
    }

    ledcWrite(pwmChannel, abs(value));
}

void stopMotors() {
    setMotor(MOTOR_FL_IN1, MOTOR_FL_IN2, PWM_CH_FL, 0);
    setMotor(MOTOR_FR_IN1, MOTOR_FR_IN2, PWM_CH_FR, 0);
    setMotor(MOTOR_RL_IN1, MOTOR_RL_IN2, PWM_CH_RL, 0);
    setMotor(MOTOR_RR_IN1, MOTOR_RR_IN2, PWM_CH_RR, 0);
}

void setupMotorPins() {
    pinMode(MOTOR_FL_IN1, OUTPUT);
    pinMode(MOTOR_FL_IN2, OUTPUT);
    pinMode(MOTOR_FR_IN1, OUTPUT);
    pinMode(MOTOR_FR_IN2, OUTPUT);
    pinMode(MOTOR_RL_IN1, OUTPUT);
    pinMode(MOTOR_RL_IN2, OUTPUT);
    pinMode(MOTOR_RR_IN1, OUTPUT);
    pinMode(MOTOR_RR_IN2, OUTPUT);

    ledcSetup(PWM_CH_FL, PWM_FREQ_HZ, PWM_RES_BITS);
    ledcAttachPin(MOTOR_FL_PWM, PWM_CH_FL);

    ledcSetup(PWM_CH_FR, PWM_FREQ_HZ, PWM_RES_BITS);
    ledcAttachPin(MOTOR_FR_PWM, PWM_CH_FR);

    ledcSetup(PWM_CH_RL, PWM_FREQ_HZ, PWM_RES_BITS);
    ledcAttachPin(MOTOR_RL_PWM, PWM_CH_RL);

    ledcSetup(PWM_CH_RR, PWM_FREQ_HZ, PWM_RES_BITS);
    ledcAttachPin(MOTOR_RR_PWM, PWM_CH_RR);

    stopMotors();  
}

void Motor_Task(void *pvParameters) {
    MotorData_t   cmd;
    unsigned long recvAge;
    bool          haveData;

    for (;;) {
        if (xSemaphoreTake(MotorCmd_Mutex, portMAX_DELAY) == pdTRUE) {
            memcpy(&cmd, (const void *)&lastCmd, sizeof(MotorData_t));
            recvAge  = millis() - lastRecvMs;
            haveData = (lastRecvMs != 0);
            xSemaphoreGive(MotorCmd_Mutex);
        }

        if (!haveData || recvAge > CMD_TIMEOUT_MS) {
            stopMotors();
        } else {
            int leftRaw  = constrain(cmd.speed + cmd.angle, -512, 512);
            int rightRaw = constrain(cmd.speed - cmd.angle, -512, 512);

            int leftPWM  = map(leftRaw,  -512, 512, -255, 255);
            int rightPWM = map(rightRaw, -512, 512, -255, 255);

            setMotor(MOTOR_FL_IN1, MOTOR_FL_IN2, PWM_CH_FL, leftPWM);
            setMotor(MOTOR_RL_IN1, MOTOR_RL_IN2, PWM_CH_RL, leftPWM);
            setMotor(MOTOR_FR_IN1, MOTOR_FR_IN2, PWM_CH_FR, rightPWM);
            setMotor(MOTOR_RR_IN1, MOTOR_RR_IN2, PWM_CH_RR, rightPWM);
        }

        vTaskDelay(pdMS_TO_TICKS(20));   
    }
}