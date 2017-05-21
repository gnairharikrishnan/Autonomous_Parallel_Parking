#ifndef ENCODER_H
#define ENCODER_H	1

#include "pin_setup.h"
#include "hal_common_includes.h"



extern unsigned int g_leftEncoderCount;
extern unsigned int g_rightEncoderCount;

/**
  * @brief  Sets up PB2 as an input for the LEFT encoder, and PB4 as input for RIGHT encoder with
			Rising & Falling edge triggered interrupts, Pulled down, and low speed frequency.
			IRQ's for PB2 (left encoder), and PB4 (right encoder) are also enabled.
  * @retval None
  */
void encoders_init();

//Enable interrupts for left and right encoders and disable the counts for the interrupts
void encoders_enable();

//Disable interrupts for left and right encoders
void encoders_disable();

//zero out left and right encoder count
void encoders_reset();
#endif