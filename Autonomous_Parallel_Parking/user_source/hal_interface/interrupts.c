#include "interrupts.h"
#include "hal_common_includes.h"
int count = 0;

/*
	Interrupt IRQ's
*/

/**
* @brief IRQ for PA0 (user button) - use this for any testing that can be done with the push button.
*/
void EXTI0_IRQHandler(void){
	//User button interrupt
	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
* @brief IRQ for PB2 - to be used for left encoder
*/
void EXTI2_TSC_IRQHandler(void){
	//Pin PB2 IRQ. encoder left counter can go here
	
	//Clear interrupt flag
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

/**
* @brief IRQ for PB4 - to be used for right encoder
*/
void EXTI4_IRQHandler(void){
	//Pin PB4 IRQ.  Encoder right counter can go here
	
	//Clear interrupt flag
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

/*
	Interrupt initialization functions:
*/

/**
  * @brief  Sets up PE8 (blue LED) as an output, and PA0, PB2, and PB4 as inputs with
			Rising & Falling edge triggered interrupts, Pulled down, and low speed frequency.
			IRQ's for PA0, PB2, and PB4 are also enabled.
  * @retval None
  */
void GPIOInterruptSetup(){
	GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
	
	//   //Set GPIO PE8 (blue LED) as an output
	// Pin_Set(GPIOE, GPIO_PIN_8,GPIO_MODE_OUTPUT_PP,GPIO_PULLDOWN,GPIO_SPEED_FREQ_LOW);
	
	// //Set GPIO PA0 (user button) as input and  enable rising_falling edge triggered interrupt
	// Pin_Set(GPIOA, GPIO_PIN_0,GPIO_MODE_IT_RISING ,GPIO_PULLDOWN,GPIO_SPEED_FREQ_LOW);
	
	
	// //Set GPIO PB2 & PB4 as inputs and  enable rising_falling edge triggered interrupts
 //    Pin_Set(GPIOB, GPIO_PIN_2 | GPIO_PIN_4,GPIO_MODE_IT_RISING,GPIO_PULLDOWN,GPIO_SPEED_FREQ_LOW);
 
	
	 //EXTI interrupt init for GPIO Pin PA0
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  
	 //EXTI interrupt init for GPIO Pin PB2
  HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);

  
	 //EXTI interrupt init for GPIO Pin PB4
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  

}
