#ifndef MOTION_H_
#define MOTION_H_   1

#include <stdint.h>
#include "hal_common_includes.h"

#define LEFT 0U
#define RIGHT 1U
#define FORWARD 3U
#define BACK 4U
#define RECENTER_RIGHT 5U
#define RECENTER_LEFT 6U
#define ROTATE 7U
#define STOP 8U
#define PARALLEL_PARK 9U
void move_robot(uint8_t next_dir);


#endif // MOTION_H_