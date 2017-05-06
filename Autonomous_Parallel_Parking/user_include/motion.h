#ifndef MOTION_H_
#define MOTION_H_   1

#include <stdint.h>
#include "hal_common_includes.h"

#define LEFT 0U
#define RIGHT 1U
#define FORWARD 3U
#define BACK 4U
#define STOP 5U
void move_robot(uint8_t next_dir);

#endif // MOTION_H_