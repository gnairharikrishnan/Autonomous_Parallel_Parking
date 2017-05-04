/***************************************************************************************************************
	
  Authors: Harikrishnan G Nair
           Denny Mathews
           John Vasquez
           Steven Pill
           Harish Puvvada

  Description: This is the file that contains the applicaiton source code. More coming up!!

*****************************************************************************************************************

      Copyright © 2017 Harikrishnan G Nair, Denny Mathews, John Vasquez, Steven Pill, Harish Puvvada @ NYU

 ****************************************************************************************************************/

#include "cpu.h"
#include "interrupts.h"
#include "uart.h"
#include <stdio.h>

uint32_t counter = 0;

int main()
{
  cpu_init();
  uart_debug_init();
  GPIOInterruptSetup();

  //Set PE8, blue LED
  Pin_Set(GPIOE, GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
  HAL_InitTick(1);

  while(1){

    printf("%d\n",counter);

    //Timer6 is set up to tick every 1ms, so 1000 counts is 1 second
    if (counter == 1000)
    {
      HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_8);
      counter = 0;
    }
  }

  printf("ERROR\n");
  return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  HAL_IncTick();
  counter++;
}



