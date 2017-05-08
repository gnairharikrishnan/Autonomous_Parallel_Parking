#ifndef SENSOR_TIMER_H_
#define SENSOR_TIMER_H_   1

#include <stdint.h>
#include "hal_common_includes.h"

void InitializeTimer();
typedef enum {
  sensor_front,
  sensor_left,
  sensor_right
} sensor_id;

			
float usensor_check_distance(int sensor_id);

// //Initialize ultrasound sensor gpio as needed for the board we are using.
// void usensor_gpio_init(void);

// //Simple software delay microseconds.
// void delay_us(uint32_t n);

// //Measures a pulse in microseconds.
// long pulse_in(GPIO_TypeDef* echo_port, uint32_t hexval);


void trigger_sensor(int sensor_no);

void init_sensor();
// HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority);
#endif