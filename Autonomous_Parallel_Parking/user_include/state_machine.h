#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_	1

#include "hal_common_includes.h"

#define TIMER_PERIOD_1KHZ	6545
// uncomment for parallel parking
#define CURR_MODE_PERPENDICULAR

typedef enum {
	STATE_IDLE,
	STATE_FOLLOW_WALL
} states;

void state_init(void);

uint8_t	 state_execute(void);


#endif // STATE_MACHINE_H_