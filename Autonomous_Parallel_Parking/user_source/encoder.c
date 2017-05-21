#include "encoder.h"

unsigned int g_leftEncoderCount = 0;
unsigned int g_rightEncoderCount = 0;


/*
	Interrupt initialization functions:
*/

/**
  * @brief  Sets up PB2, and PB4 as inputs with
			Rising & Falling edge triggered interrupts, Pulled down, and low speed frequency.
			IRQ's for PB2 (right encoder), and PB4 (left encoder) are also enabled.
  * @retval None
  */
void encoders_init(){
	//GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  //__HAL_RCC_GPIOE_CLK_ENABLE();
	
	//   //Set GPIO PE8 (blue LED) as an output
	// Pin_Set(GPIOE, GPIO_PIN_8,GPIO_MODE_OUTPUT_PP,GPIO_PULLDOWN,GPIO_SPEED_FREQ_LOW);
	
	 //Set GPIO PA0 (user button) as input and  enable rising_falling edge triggered interrupt
	 Pin_Set(GPIOA, GPIO_PIN_0,GPIO_MODE_IT_RISING ,GPIO_PULLDOWN,GPIO_SPEED_FREQ_LOW);
	
	
	// //Set GPIO PB2 & PB4 as inputs and  enable rising_falling edge triggered interrupts
     Pin_Set(GPIOB, GPIO_PIN_2 | GPIO_PIN_4,GPIO_MODE_IT_RISING,GPIO_PULLDOWN,GPIO_SPEED_FREQ_LOW);
 
	
	/*
	 //EXTI interrupt init for GPIO Pin PA0
  	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	*/
  
	 //EXTI interrupt init for GPIO Pin PB2, RIGHT Encoder
	 //Only using these encoders for turning, so don't enable them until we are doing a turn
  HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 3, 0);

	 //EXTI interrupt init for GPIO Pin PB4, LEFT Encoder
  HAL_NVIC_SetPriority(EXTI4_IRQn, 3, 0);

  
}

void encoders_enable(){
	//Reset encoder counts before enable the IRQ 
	encoders_reset();
	HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

void encoders_disable(){
	encoders_reset();
	HAL_NVIC_DisableIRQ(EXTI2_TSC_IRQn);
	HAL_NVIC_DisableIRQ(EXTI4_IRQn);
}

void encoders_reset(){
	g_leftEncoderCount = 0;
	g_rightEncoderCount = 0;
}
