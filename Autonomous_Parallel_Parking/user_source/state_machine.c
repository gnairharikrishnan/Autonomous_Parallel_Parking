
/*
 Code to setup the states and define its operation

 */
#include "stm32f3xx_hal.h"
#include "sensor_timer.h"
#include "motion.h"
#include "state_machine.h"
#include <stdio.h>


int g_current_state;
#define TURN_FORWARD_DISTANCE_THRESH 18
#define DIP_RISE_THRESH				 100
#define REF_DIST_FROM_RGT_WALL 10

typedef uint8_t bool;
#define TRUE 1U
#define FALSE 0U


bool g_ldip = FALSE;
bool g_lrise = FALSE;
bool g_rdip = FALSE;
bool g_rrise = FALSE;
float g_old_ld, g_old_rd;

uint32_t g_exit_counter = 0;

int8_t g_parking_dir = LEFT;
uint8_t calculate_next_dominant_turn(float ld, float rd)
{
	if( ld > rd )
		return LEFT;
	else
		return RIGHT;
}

bool check_parking_condition()
{
	if(g_ldip && g_lrise) {
		g_parking_dir = LEFT;
		return TRUE;
	}
	if(g_rdip && g_rrise) {
		g_parking_dir = RIGHT;
		return TRUE;
	}
	return FALSE;
}

void update_parking_params(float ld, float rd)
{
	if (g_ldip == TRUE)	{
		if (g_exit_counter == 1000U) {
			// check if we donot get into false states
			g_ldip = FALSE;
			return;
		}
		if ((g_old_ld - ld) > DIP_RISE_THRESH) {
			g_lrise = TRUE;
		}
		else
			g_exit_counter++;
	}
	else if (g_rdip == TRUE) {
		if (g_exit_counter == 1000U) {
			// check if we donot get into false states
			g_rdip = FALSE;
			return;
		}
		if ((g_old_rd - rd) > DIP_RISE_THRESH) {
			g_lrise = TRUE;
		}
		else
			g_exit_counter++;
	}
	else {
		// FIRST CONDITION hasnt occured
		if ((ld - g_old_ld) > DIP_RISE_THRESH) {
			g_ldip = TRUE;
			g_rdip = FALSE;
		}
		else {
			g_old_ld = ld;
			g_exit_counter = 0;
		}
		// check right now
		if ((rd - g_old_rd) > DIP_RISE_THRESH) {
			g_rdip = TRUE;
			g_ldip = FALSE;
		}
		else {
			g_old_rd = rd;
			g_exit_counter = 0;
		}
	}
}



// #ifdef CURR_MODE_PERPENDICULAR

void state_init(void)
{
	g_current_state = STATE_IDLE;
}

void state_next(int next_state)
{
	g_current_state = next_state;
}

uint8_t state_execute(void)
{
	float ld, rd, fd;
	bool to_park = FALSE;
	uint8_t next_dir = FORWARD;
	switch(g_current_state)
	{
		case STATE_IDLE:
			state_next(STATE_FOLLOW_WALL);
		break;

		case STATE_FOLLOW_WALL:
			ld = usensor_check_distance(sensor_left);
			rd = usensor_check_distance(sensor_right);
			fd = usensor_check_distance(sensor_front);
			// first check for parking condition occurence
			//update_parking_params(ld, rd);
			//to_park = check_parking_condition();
			//if (to_park)
			//{
			//	// park state next and break from here
			//	state_next(STATE_PARK);

			//}
			
			if (fd < TURN_FORWARD_DISTANCE_THRESH)
				next_dir = LEFT; //calculate_next_dominant_turn(ld, rd);
			else if(rd > REF_DIST_FROM_RGT_WALL)
				next_dir = RECENTER_RIGHT; //Move towards the wall 
			else if(rd < (REF_DIST_FROM_RGT_WALL - 5))
					next_dir = RECENTER_LEFT;
            else
				next_dir = FORWARD;
		break;

		case STATE_PARK:
		break;

		case STATE_STOP:
		break;
	}
	return next_dir;
}

// #endif //CURR_MODE_PERPENDICULAR