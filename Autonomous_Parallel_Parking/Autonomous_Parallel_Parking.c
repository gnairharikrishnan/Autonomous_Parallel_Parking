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

// C standard include(s):
#include <math.h>

// Custom user APIs needed for hardware access specific to this board:
#include "cpu.h"
#include "interrupts.h"
#include "uart.h"
#include <stdio.h>
#include "motion.h"
#include "state_machine.h"
#include "sensor_timer.h"
#include "pwm_hal.h"

// Custom user APIs needed for generic algorithmic libraries that are hardware-independent:
#include "foo.h"



int main()
{
  /*
    Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
   */
  cpu_init();

  uart_debug_init();
  //GPIOInterruptSetup();
  init_sensor();
  state_init();

  //Set PE8, blue LED
  Pin_Set(GPIOE, GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
  Pin_Set(GPIOE, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);


  /*
    Initialize the PWM outputs by configuring the corresponding channels
   */
  init_pwm();

  /*
    Test a series of movements:
   */


  int i = 0;
  uint8_t next_dir;
  while(1)
  {
    /*
      Carry out a simple unit test of foo() declared in foo.h:
     */
    if(TEST_FOO(i, i+1) < 0)
    {
      /*
        If the above fails there is either a hardware, code or other undefined error.
        Now we're in an undefined state regarding processor core behavior...
       */
      while(1); // We probably have had a radiation hit or a major malfunction on the ALU of the processor...
    }
    else
    {
      // move_robot(STOP);

      next_dir = state_execute();
      move_robot(next_dir);
      //cpu_sw_delay(10U);

      //trigger_sensor(sensor_front) ;

      // cpu_sw_delay(10U);
      // if (g_inches_f < 20)
      //   move_back();
      // cpu_sw_delay(500U); 
      // turn_f_right();
      // cpu_sw_delay(500U); 
      // turn_f_left();
      // cpu_sw_delay(500U); 
      // brake();
      // cpu_sw_delay(500U); 
     
    }
  }

  return 0;
}
