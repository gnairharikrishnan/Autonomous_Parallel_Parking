#ifndef MOTOR_HAL_H_
#define MOTOR_HAL_H_	1

#include "hal_common_includes.h"

#define TIMER_PERIOD_1KHZ	6545

typedef enum {
	motor1,
	motor2
} motor_id;

void motor_init(void);
void test_motor_gpio(char value);

#endif