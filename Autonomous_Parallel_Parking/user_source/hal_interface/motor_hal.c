#include "motor_hal.h"


void test_motor_gpio(char value)
{
	switch(value)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
			break;
		default:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
			break;
	}
}