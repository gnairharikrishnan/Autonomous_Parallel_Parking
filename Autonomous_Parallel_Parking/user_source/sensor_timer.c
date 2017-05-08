#include "sensor_timer.h"
#include "cpu.h"
#include "hal_common_includes.h" 
#include "uart.h"


TIM_HandleTypeDef s_TimerInstance;
float g_inches_f = 0;
float g_inches_r = 0;
float g_inches_l = 0;
 volatile  unsigned long g_counter = 0;

// void TIM6_DAC_IRQHandler(void)
// {
//     trigger_sensor(sensor_front);    
//     // HAL_NVIC_ClearPendingIRQ(TIM7_IRQn);
// }

//Measures a HIGH pulse
int32_t pulse_in(GPIO_TypeDef* echo_port, uint32_t hexval ) 
{
  int32_t counter = 0;
  while((echo_port->IDR & hexval) == 0) {continue;} 
  //Time the pulse. Start counting clock cycles until the echoPin goes low.

  while((echo_port->IDR & hexval) == hexval) {
      counter++;
      // HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, flag);
      // flag = flag ? 0 : 1;
      asm("nop");
    }
  return counter;
} 

  //   //Every 72 clock cycles yields 1 microseconds 
  //   return counter/72;
  // }


  // int get_sensor_dist(GPIO_TypeDef* trig_port, GPIO_TypeDef* echo_port, uint16_t trigger)
  // {
  //   //Preparing the trigger pin.
  //   HAL_GPIO_WritePin(trig_port, trigger, GPIO_PIN_RESET);    
  //   delay_us(2);                                                
  //   //10us sensor required initiation pulse on the trigger pin. 
  //   HAL_GPIO_WritePin(trig_port, trigger, GPIO_PIN_SET);   
  //   delay_us(10);                                                   
  //   HAL_GPIO_WritePin(trig_port, trigger, GPIO_PIN_RESET);   
  //   //pulse_in returns the sound wave travel time in microseconds
  //   duration = pulse_in(echo_port);
  //   distance = duration*.0133858/2;
  //   inches = distance%12;
  //   //Delay 50ms as per manufacture recomendation 
  //   delay_us(50000); 
  //   return inches;
  // }

uint32_t flag = 1;

//Measures a HIGH pulse
void trigger_sensor(int sensor_no) 
{
  int duration = 0;
  float distance = 0;
  char buffer[20];

  
  switch(sensor_no)
  {
    case sensor_front:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
        cpu_sw_udelay(2U);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
        cpu_sw_udelay(10U);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);

       
        // Poll PF6
        duration = pulse_in(GPIOF, 0x0040);
        //distance = duration * 0.0133858/2.0;
        distance = duration * 1 / 72000000.0 * 340.0 / 2.0;
        g_inches_f = distance * 1000; // distance % 12U;
        //memset(buffer,'0', sizeof(buffer));
        //sprintf (buffer, "%d: %f \n\r", duration, g_inches_f);
        //uart_send_data_blocking(UART_PORT1, (uint8_t*)buffer, sizeof(buffer));

        //  if (duration < 2) {
        //   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
        // } else {
        //   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
        // }
        // flag = flag ? 0 : 1;
        //Delay 50ms as per manufacture recomendation 
        //delay_us(50000); 
      break;
    case sensor_left:
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
      cpu_sw_udelay(2U);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
      cpu_sw_udelay(10U);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
      // if (flag) {
      //   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
      //   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
      // } else {
      //   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
      //   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
      // }
      // flag = flag ? 0 : 1;
      // Poll PF9
      duration = pulse_in(GPIOF, 0x0200);
      distance = duration * 1 / 72000000.0 * 340.0 / 2.0;
      g_inches_l = distance * 1000; // distance % 12U;
      break;

    case sensor_right:
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
      cpu_sw_udelay(2U);
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
      cpu_sw_udelay(10U);
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
      // if (flag) {
      //     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
      //     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
      //   } else {
      //     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
      //     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
      //   }
      //   flag = flag ? 0 : 1;
        
       // Poll PE11
      duration = pulse_in(GPIOE, 0x0800);
      distance = duration * 1 / 72000000.0 * 340.0 / 2.0;
      g_inches_r = distance * 1000; // distance % 12U;
      break;
    break;
  }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  trigger_sensor(sensor_front);
  //       cpu_sw_delay(10U);

  trigger_sensor(sensor_left);
        // cpu_sw_delay(10U);

  trigger_sensor(sensor_right);
        // cpu_sw_delay(10U);

}

//GPIO initializations
void usensor_gpio_init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  // /*Configure GPIO pin Output Level */
  // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  // /*Configure GPIO pin Output Level */
  // HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);

  // /*Configure GPIO pin Output Level */
  // HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pins : PF9 PF6 */
  GPIO_InitStruct.Pin = GPIO_PIN_9| GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PE10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void InitializeTimer()
{    
    RCC_ClkInitTypeDef    clkconfig;
    uint32_t              uwTimclock, uwAPB1Prescaler = 0U;
    uint32_t              uwPrescalerValue = 0U;
    uint32_t              pFLatency;

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

    s_TimerInstance.Init.Prescaler = 53; // for 0.5 seconds //109/2 = 54.93 for 20 Hz
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
  usensor_gpio_init();
  InitializeTimer();

}

float usensor_check_distance(int sensor_no)
{
  switch(sensor_no)
  {
    case sensor_front:
      return g_inches_f;
      break;
    case sensor_left:
      return g_inches_l;
      break;
    case sensor_right:
      return g_inches_r;
      break;
  }
  return 0.0;
}