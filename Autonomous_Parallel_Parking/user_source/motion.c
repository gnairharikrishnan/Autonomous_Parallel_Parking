
#include "cpu.h"
#include "hal_common_includes.h" 
#include "pwm_hal.h"
#include "motor_hal.h"
#include "motion.h"
#include <stdio.h>


#define HIGH      3150U
#define LOW       0U
#define MEDIUM    0U

extern float g_inches_f;


void move_forward()
{
  configure_channel(1, LOW);
  configure_channel(2, HIGH);
  configure_channel(3, LOW);
  configure_channel(4, HIGH + 05U);
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

void turn_f_left_slow()
{
  configure_channel(1, LOW);
  configure_channel(2, HIGH - 50);
  configure_channel(3, LOW);
  configure_channel(4, MEDIUM);
}

void turn_f_right_slow()
{
  configure_channel(1, LOW);
  configure_channel(2, MEDIUM);
  configure_channel(3, LOW);
  configure_channel(4, HIGH - 50);
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
      cpu_sw_delay(60U);

			break;
		case RIGHT:
      brake();
      cpu_sw_delay(10U);
      turn_f_left();
      cpu_sw_delay(60U);

			break;
		case FORWARD:
      //brake();
      //cpu_sw_delay(10U);
			move_forward();
      cpu_sw_delay(05U);

			break;
		case BACK:
			move_back();
      cpu_sw_delay(10U);
			break;
    case RECENTER_RIGHT:
      brake();
      cpu_sw_delay(01U);
      turn_f_right();
      cpu_sw_delay(01U);
      move_forward();
      cpu_sw_delay(01U);
      break;

    case RECENTER_LEFT:
      brake();
      cpu_sw_delay(01U);
      turn_f_left();
      cpu_sw_delay(01U);
      move_forward();
      cpu_sw_delay(01U);
      break;

		case STOP:
			brake();
			break;

	}
}	