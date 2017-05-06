#ifndef INTERRUPTS
#define INTERRUPTS	1

#include "pin_setup.h"
#include "hal_common_includes.h"

//Interrupt initialization functions
void GPIOInterruptSetup();


//Interrupt IRQ's
void EXTI0_IRQHandler(void);
void EXTI2_TSC_IRQHandler(void);
void EXTI4_IRQHandler(void);

#endif