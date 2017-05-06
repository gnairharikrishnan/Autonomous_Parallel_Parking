
#include "cpu.h"
#include "hal_common_includes.h" 
#include "pwm_hal.h"
#include "motor_hal.h"
#include "motion.h"
#include <stdio.h>


#define HIGH      3200U
#define LOW       0U
#define MEDIUM    0U

extern float g_inches_f;


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

void move_robot(uint8_t next_dir)
{
	switch(next_dir)
	{
		case LEFT:
      brake();
      cpu_sw_delay(10U);
			turn_f_left();
      cpu_sw_delay(20U);

			break;
		case RIGHT:
      brake();
      cpu_sw_delay(10U);
      turn_f_left();
      cpu_sw_delay(20U);

			break;
		case FORWARD:
      //brake();
      //cpu_sw_delay(10U);
			move_forward();
      cpu_sw_delay(10U);

			break;
		case BACK:
			move_back();
			break;
		case STOP:
			brake();
			break;

	}
}	