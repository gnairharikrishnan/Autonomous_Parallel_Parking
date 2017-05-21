#include "pin_setup.h"


/**
  * @brief  Set a pin's status based on params
  * @param  GPIOx: The port that the pin to be set is on
  * @param  pins: The pins on the selected port to set.  This can be a single pin or multiple pins.
					to set multiple pins, just pass in the pins as something like: GPIO_PIN_0 | GPIO_PIN_1 ...
  * @param  mode: The mode to set the pin as.  For a list of valid modes, see "stm32f3xx_hal_gpio.h"
  * @param  pull: How you want the pin to be pull. For a list of valid pull modes, see "stm32f3xx_hal_gpio.h"  
  * @param  speed: GPIO output maximum frequency. For a list of valid speeds, see "stm32f3xx_hal_gpio.h"
  * @retval None
  */
void Pin_Set(GPIO_TypeDef* GPIOx, uint32_t pins, uint32_t mode, uint32_t pull, uint32_t speed)
{
	static GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = pins;
	GPIO_InitStruct.Mode = mode;
	GPIO_InitStruct.Pull = pull;
	GPIO_InitStruct.Speed = speed;

	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
