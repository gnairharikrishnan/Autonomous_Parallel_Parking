#include <stdio.h>
#include "sensor_timer.h"
#include "stm32f3xx_ll_exti.h"
#include "cpu.h"
#include "hal_common_includes.h" 
#include "mutex.h"
#include "uart.h"

uint8_t fmutex=0, rmutex=0, lmutex=0, mutex_flag=0;

TIM_HandleTypeDef s_TimerInstance;
TIM_HandleTypeDef s_FSensorTimerInstance;
TIM_HandleTypeDef s_RSensorTimerInstance;
TIM_HandleTypeDef s_LSensorTimerInstance;

float g_inches_f = -1.0;
float g_inches_r = -1.0;
float g_inches_l = -1.0;
#define  TIMER_COUNT_PRESCALER 19U
 volatile  unsigned long g_counter = 0;
 uint32_t flag = 1;


#define FALLING 0
#define RISING 1

volatile uint8_t PulseEnded = 1, Front_InterruptEdge = RISING, Right_InterruptEdge = RISING, Left_InterruptEdge = RISING, TimerOverflow = 0;
volatile uint32_t Forward_PulseTime = 0, Right_PulseTime = 0, Left_PulseTime = 0;
volatile uint32_t MSec = 0;

//Measures a HIGH pulse
int32_t pulse_in(GPIO_TypeDef* echo_port, uint32_t hexval ) 
{
  int32_t counter = 0;
  //int32_t timeout = 0;
  while((echo_port->IDR & hexval) == 0){      
    continue;
  }
  while((echo_port->IDR & hexval) == hexval) {
      counter++;
    }
  return counter;
} 

//GPIO initializations
void usensor_gpio_init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : PE11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PF6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PE10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // /*Configure GPIO pin : PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//Measures a HIGH pulse
void trigger_sensor(int sensor_no) 
{
  int duration = 100;
  float distance = 0;

  
  switch(sensor_no)
  {
    case sensor_front:
        // PA12
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
        cpu_sw_udelay(2U);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
        cpu_sw_udelay(10U);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);

        Front_InterruptEdge = RISING;
 
      break;
    case sensor_left:
      // PA11
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
      cpu_sw_udelay(2U);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
      cpu_sw_udelay(10U);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);

      Left_InterruptEdge = RISING;

      break;

    case sensor_right:
      // PE10
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
      cpu_sw_udelay(2U);
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
      cpu_sw_udelay(10U);
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);

      Right_InterruptEdge = RISING;

      break;
    break;
  }
}

void EXTI_ClearITPendingBit(uint32_t EXTI_Line)
{


  EXTI->PR = EXTI_Line;
}


uint8_t EXTI_GetITStatus(uint32_t EXTI_Line)
{
   uint8_t bitstatus = 0;
  
  if ((EXTI->PR & EXTI_Line) != (uint32_t)0x0000)
  {
    bitstatus = 1;
  }
  else
  {
    bitstatus = 0;
  }
  return bitstatus;
    
}


void EXTI9_5_IRQHandler(void) {

  
  // Front Sensor Stuff
  //If the interrupt has been triggered
  if(EXTI_GetITStatus(LL_EXTI_LINE_6) == 1){ 

    // Clear the pending status of the pin
    EXTI_ClearITPendingBit(LL_EXTI_LINE_6);
    if(Front_InterruptEdge == RISING){

      //Start the counter to time the length of the returned pulse!
      // TIM_Cmd(SR04_Timer, ENABLE);
       // Start the TIM time Base generation in interrupt mode

          HAL_TIM_Base_Start_IT(&s_FSensorTimerInstance);

      //Set the next edge to be falling. The 0 and 1 convention
      Front_InterruptEdge = FALLING;
    }
    else{ 
//On falling edge (Interrupt Edge will be 1 on falling edge)
      //Disable the timer to stop the count
      // TIM_Cmd(SR04_Timer, DISABLE);
      HAL_TIM_Base_Stop(&s_FSensorTimerInstance);
        //printf("In IRQ Handler C\n\r");



      //Set PulseTime to the length the timer was counting for
      Forward_PulseTime = TIM15->CNT;
      TIM15->CNT = 0;
      //Indicate that the SR04 returned pulse has ended.
      // PulseEnded = 1;
      if(acquire_mutex(&fmutex)) {
        g_inches_f = (TIMER_COUNT_PRESCALER + 1) * Forward_PulseTime / 2.0 * 1 / 72000000 * 343.0 * 100.0;
        release_mutex(&fmutex);
      }
    }
  }
}

void EXTI15_10_IRQHandler(void) {
   // Right Sensor Stuff
  //If the interrupt has been triggered

  if(EXTI_GetITStatus(LL_EXTI_LINE_11) == 1){ 

    // Clear the pending status of the pin
    EXTI_ClearITPendingBit(LL_EXTI_LINE_11);
    if(Right_InterruptEdge == RISING){

      //Start the counter to time the length of the returned pulse!
      // TIM_Cmd(SR04_Timer, ENABLE);
       /* Start the TIM time Base generation in interrupt mode */

          HAL_TIM_Base_Start_IT(&s_RSensorTimerInstance);

      //Set the next edge to be falling. The 0 and 1 convention
      Right_InterruptEdge = FALLING;
    }
    else{ 
//On falling edge (Interrupt Edge will be 1 on falling edge)
      //Disable the timer to stop the count
      // TIM_Cmd(SR04_Timer, DISABLE);
      HAL_TIM_Base_Stop(&s_RSensorTimerInstance);



      //Set PulseTime to the length the timer was counting for
      Right_PulseTime = TIM16->CNT;
      TIM16->CNT = 0;
      //Indicate that the SR04 returned pulse has ended.
      // PulseEnded = 1;
      if(acquire_mutex(&rmutex)) {
        g_inches_r = (TIMER_COUNT_PRESCALER + 1) * Right_PulseTime / 2.0 * 1 / 72000000 * 343.0 * 100.0;
        release_mutex(&rmutex);
      }


    }
  }
  if (EXTI_GetITStatus(LL_EXTI_LINE_13) == 1) {//Hari, Might have to make this if (as this is not conditional on the previous if not getting executed)
   
    // Clear the pending status of the pin
    EXTI_ClearITPendingBit(LL_EXTI_LINE_13);
    if(Left_InterruptEdge == RISING){

      //Start the counter to time the length of the returned pulse!
      // TIM_Cmd(SR04_Timer, ENABLE);
       /* Start the TIM time Base generation in interrupt mode */

          HAL_TIM_Base_Start_IT(&s_LSensorTimerInstance);

      //Set the next edge to be falling. The 0 and 1 convention
      Left_InterruptEdge = FALLING;
    }
    else{ 
//On falling edge (Interrupt Edge will be 1 on falling edge)
      //Disable the timer to stop the count
      // TIM_Cmd(SR04_Timer, DISABLE);
      HAL_TIM_Base_Stop(&s_LSensorTimerInstance);



      //Set PulseTime to the length the timer was counting for
      Left_PulseTime = TIM17->CNT;
      TIM17->CNT = 0;
      //Indicate that the SR04 returned pulse has ended.
      // PulseEnded = 1;
      if(acquire_mutex(&lmutex)) {
        g_inches_l = (TIMER_COUNT_PRESCALER + 1) * Left_PulseTime / 2.0 * 1 / 72000000 * 343.0 * 100.0;
        release_mutex(&lmutex);
      }

   }
  }

}

/*
  Interrupt initialization functions:
*/

/**
  * @brief  Sets up PE8 (blue LED) as an output, and PA0, PB2, and PB4 as inputs with
      Rising & Falling edge triggered interrupts, Pulled down, and low speed frequency.
      IRQ's for PA0, PB2, and PB4 are also enabled.
  * @retval None
  */
void SensorInterruptSetup(){

    //Interrupt for PF6 : Front sensor
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  
   //Interrupt for pe11 : right sensor
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  trigger_sensor(sensor_front);
  cpu_sw_udelay(10U);
  trigger_sensor(sensor_left);
  cpu_sw_udelay(10U);
  trigger_sensor(sensor_right);
  cpu_sw_udelay(10U);

}

void InitializeTimer()
{    
    RCC_ClkInitTypeDef    clkconfig;
    uint32_t              uwTimclock, uwAPB1Prescaler = 0U;
    uint32_t              uwPrescalerValue = 0U;
    uint32_t              pFLatency;
    s_FSensorTimerInstance.Instance = TIM15;
    s_RSensorTimerInstance.Instance = TIM16;
    s_LSensorTimerInstance.Instance = TIM17; 
    //TIM7 interrupt init for GPIO Pin PB4

    __HAL_RCC_TIM15_CLK_ENABLE();
    __HAL_RCC_TIM16_CLK_ENABLE();
    __HAL_RCC_TIM17_CLK_ENABLE();
      /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Get APB1 prescaler */
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;
          printf("Test POint B\n\r");
    /* Compute TIM6 clock */
    if (uwAPB1Prescaler == RCC_HCLK_DIV1) 
    {
      uwTimclock = HAL_RCC_GetPCLK1Freq();
    }
    else
    {
      uwTimclock = 2U*HAL_RCC_GetPCLK1Freq();
    }
          printf("Test POint C\n\r");

     // Compute the prescaler value to have TIM6 counter clock equal to 1MHz 
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

    //Front Timer initialization
    s_FSensorTimerInstance.Init.Prescaler = TIMER_COUNT_PRESCALER; // for 0.5 seconds //109/2 = 54.93 for 20 Hz
    s_FSensorTimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
    s_FSensorTimerInstance.Init.Period = 65535;
    s_FSensorTimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    s_FSensorTimerInstance.Init.RepetitionCounter = 0U;
    s_FSensorTimerInstance.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;    
    if(HAL_TIM_Base_Init(&s_FSensorTimerInstance) == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        //HAL_TIM_Base_Start_IT(&s_FSensorTimerInstance);
    }

    //Right Timer initialization
    s_RSensorTimerInstance.Init.Prescaler = TIMER_COUNT_PRESCALER; // for 0.5 seconds //109/2 = 54.93 for 20 Hz
    s_RSensorTimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
    s_RSensorTimerInstance.Init.Period = 65535;
    s_RSensorTimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    s_RSensorTimerInstance.Init.RepetitionCounter = 0U;
    s_RSensorTimerInstance.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;    
    if(HAL_TIM_Base_Init(&s_RSensorTimerInstance) == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        //HAL_TIM_Base_Start_IT(&s_FSensorTimerInstance);
    }

    //Left Timer initialization
    s_LSensorTimerInstance.Init.Prescaler = TIMER_COUNT_PRESCALER; // for 0.5 seconds //109/2 = 54.93 for 20 Hz
    s_LSensorTimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
    s_LSensorTimerInstance.Init.Period = 65535;
    s_LSensorTimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    s_LSensorTimerInstance.Init.RepetitionCounter = 0U;
    s_LSensorTimerInstance.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;    
    if(HAL_TIM_Base_Init(&s_LSensorTimerInstance) == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        //HAL_TIM_Base_Start_IT(&s_FSensorTimerInstance);
    }


    s_TimerInstance.Instance = TIM6;
    //TIM7 interrupt init for GPIO Pin PB4
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0U, 0U);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

    __HAL_RCC_TIM6_CLK_ENABLE();

      /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Get APB1 prescaler */
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;

    /* Compute TIM6 clock */
    if (uwAPB1Prescaler == RCC_HCLK_DIV1) 
    {
      uwTimclock = HAL_RCC_GetPCLK1Freq();
    }
    else
    {
      uwTimclock = 2U*HAL_RCC_GetPCLK1Freq();
    }

     // Compute the prescaler value to have TIM6 counter clock equal to 1MHz 
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

    s_TimerInstance.Init.Prescaler = 103; // for 0.5 seconds //109/2 = 54.93 for 20 Hz
    s_TimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
    s_TimerInstance.Init.Period = 65535U;
    s_TimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    s_TimerInstance.Init.RepetitionCounter = 0U;
    s_TimerInstance.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;    

    if(HAL_TIM_Base_Init(&s_TimerInstance) == HAL_OK)
    {
      /* Start the TIM time Base generation in interrupt mode */
      HAL_TIM_Base_Start_IT(&s_TimerInstance);
    }
}

void init_sensor()
{
    InitializeTimer();
    usensor_gpio_init();

    SensorInterruptSetup();

}


void print_all()
{
  printf("F: %f,\t R: %f,\t L: %f\n\r",g_inches_f, g_inches_r,g_inches_l );
}

float usensor_check_distance(int sensor_no)
{
  switch(sensor_no)
  {
    case sensor_front:  

      
      if(acquire_mutex(&fmutex)) {
        release_mutex(&fmutex);
        return g_inches_f;
      }
      else
        return -1;
      break;
    case sensor_left:
      if(acquire_mutex(&lmutex)) {
        release_mutex(&lmutex);
        return g_inches_l;
      }
      else
        return -1;
      break;
    case sensor_right:
      if(acquire_mutex(&rmutex)) {
        release_mutex(&rmutex);
        return g_inches_r;
      }
      else
        return -1;
      break;
  }

}