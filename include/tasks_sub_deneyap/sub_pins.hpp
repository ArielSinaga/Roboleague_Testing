#pragma once
#include <stdint.h>

#define MY_I2C_ADDR 0x08   

#define MOTOR_FL_IN1 25   // Front-Left
#define MOTOR_FL_IN2 26
#define MOTOR_FL_PWM 27

#define MOTOR_FR_IN1 32   // Front-Right
#define MOTOR_FR_IN2 33
#define MOTOR_FR_PWM 14

#define MOTOR_RL_IN1 4    // Rear-Left  -- GANTI sesuai skematik!
#define MOTOR_RL_IN2 16
#define MOTOR_RL_PWM 17

#define MOTOR_RR_IN1 18   // Rear-Right -- GANTI sesuai skematik!
#define MOTOR_RR_IN2 19
#define MOTOR_RR_PWM 21

#define PWM_FREQ_HZ   5000
#define PWM_RES_BITS  8      // 0-255
#define PWM_CH_FL     0
#define PWM_CH_FR     1
#define PWM_CH_RL     2
#define PWM_CH_RR     3

#define CMD_TIMEOUT_MS 300