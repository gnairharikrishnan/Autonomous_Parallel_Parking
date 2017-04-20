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

int main()
{
  cpu_init();
  GPIOInterruptSetup();
  return 0;
}



