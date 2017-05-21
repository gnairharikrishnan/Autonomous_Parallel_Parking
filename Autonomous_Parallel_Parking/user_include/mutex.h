#ifndef MUTEX_H_
#define MUTEX_H_   1



#include <stdio.h>
#include "sensor_timer.h"
#include "stm32f3xx_ll_exti.h"
#include "cpu.h"
#include "hal_common_includes.h"




uint8_t acquire_mutex(uint8_t *flag);
void release_mutex(uint8_t *flag);

#endif // MUTEX_H_