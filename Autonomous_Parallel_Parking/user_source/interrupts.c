#include "interrupts.h"
#include "hal_common_includes.h"
#include "encoder.h"
#include <stdio.h>

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
* @brief IRQ for PB2 - to be used for right encoder
*/
void EXTI2_TSC_IRQHandler(void){

	HAL_NVIC_DisableIRQ(EXTI2_TSC_IRQn);
	    //print statement helps add a useful delay for our encoders
	printf("\n\r");
	++g_rightEncoderCount;
	HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);

	//Clear interrupt flag
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

/**
* @brief IRQ for PB4 - to be used for left encoder
*/
void EXTI4_IRQHandler(void){

	HAL_NVIC_DisableIRQ(EXTI4_IRQn);
	    //print statement helps add a useful delay for our encoders
	printf("\n\r");
	++g_leftEncoderCount;
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

