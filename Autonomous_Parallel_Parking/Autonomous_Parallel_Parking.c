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
#include "pwm_hal.h"
#include "motor_hal.h"

// Custom user APIs needed for generic algorithmic libraries that are hardware-independent:
#include "foo.h"

#define HIGH      5000
#define LOW       10
#define MEDIUM    2500

void move_forward()
{
  configure_channel(1, LOW);
  configure_channel(2, HIGH);
  configure_channel(3, LOW);
  configure_channel(4, HIGH);
}

void move_back()
{
  configure_channel(1, HIGH);
  configure_channel(2, LOW);
  configure_channel(3, HIGH);
  configure_channel(4, LOW);
}

void turn_f_left()
{
  configure_channel(1, LOW);
  configure_channel(2, HIGH);
  configure_channel(3, LOW);
  configure_channel(4, MEDIUM);
}

void turn_f_right()
{
  configure_channel(1, LOW);
  configure_channel(2, MEDIUM);
  configure_channel(3, LOW);
  configure_channel(4, HIGH);
}

void brake()
{
  configure_channel(1, LOW);
  configure_channel(2, LOW);
  configure_channel(3, LOW);
  configure_channel(4, LOW);
}


int main()
{
  /*
    Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
   */
  cpu_init();

  /*
    Initialize the PWM outputs by configuring the corresponding channels
   */
  init_pwm();

  /*
    Test a series of movements:
   */
  start_channel(1);
  start_channel(2);
  start_channel(3);
  start_channel(4);

  int i = 0;
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
      move_forward();
      cpu_sw_delay(500U);
      move_back();
      cpu_sw_delay(500U); 
      turn_f_right();
      cpu_sw_delay(500U); 
      turn_f_left();
      cpu_sw_delay(500U); 
      brake();
      cpu_sw_delay(500U); 
     
    }
  }

  return 0;
}
