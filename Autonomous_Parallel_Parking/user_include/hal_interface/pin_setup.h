#ifndef PIN_SETUP
#define PIN_SETUP	1

#include "stm32f3xx_hal_gpio.h"


void Pin_Set(GPIO_TypeDef* GPIOx, uint32_t pins, uint32_t mode, uint32_t pull, uint32_t speed);

#endif