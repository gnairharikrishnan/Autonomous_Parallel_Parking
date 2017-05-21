#include <stdio.h>
#include "sensor_timer.h"
#include "stm32f3xx_ll_exti.h"
#include "cpu.h"
#include "hal_common_includes.h"
#include "mutex.h" 

uint8_t acquire_mutex(uint8_t* flag){
	if(*flag == 0){ 
	*flag=1;
	return 1;}
	else{return 0;}
}

void release_mutex(uint8_t* flag){
	*flag=0;
}