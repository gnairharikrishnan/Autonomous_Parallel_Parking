// #ifndef U_SENSOR_H
// #define U_SENSOR_H	1

// #include "stm32f3xx_hal.h"
// #include "hal_common_includes.h"




// typedef enum {
// 	sensor_front,
// 	sensor_left,
// 	sensor_right
// } motor_id;




// //Initialize ultrasound sensor gpio as needed for the board we are using.
// void usensor_gpio_init(void);

// //Simple software delay microseconds.
// void delay_us(uint32_t n);

// //Measures a pulse in microseconds.
// long pulse_in(GPIO_TypeDef* echo_port);

// //Returns the distance from object.
// int get_sensor_dist(GPIO_TypeDef* trig_port, GPIO_TypeDef* echo_port, uint16_t trigger);

// #endif