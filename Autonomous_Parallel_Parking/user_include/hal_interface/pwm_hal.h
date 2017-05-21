#ifndef PWM_HAL_H_
#define PWM_HAL_H_	1

#include "hal_common_includes.h"

#define TIMER_PERIOD_1KHZ	6545



void init_pwm(void);
void configure_channel(uint32_t channel_no, uint32_t new_pulse);

void start_channel(uint32_t channel_no);

void stop_channel(uint32_t channel_no);


#endif