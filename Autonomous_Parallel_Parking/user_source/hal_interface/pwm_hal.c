#include "pwm_hal.h"

TIM_HandleTypeDef tim_init1;
TIM_HandleTypeDef tim_init2;
TIM_HandleTypeDef tim_init3;
TIM_HandleTypeDef tim_init4;

TIM_OC_InitTypeDef tim2_conf1;
TIM_OC_InitTypeDef tim2_conf2;
TIM_OC_InitTypeDef tim2_conf3;
TIM_OC_InitTypeDef tim2_conf4;


void init_pwm(void)
{

// HAL_TIM_ACTIVE_CHANNEL_1        = 0x01U,    /*!< The active channel is 1     */
// HAL_TIM_ACTIVE_CHANNEL_2        = 0x02U,    /*!< The active channel is 2     */
// HAL_TIM_ACTIVE_CHANNEL_3        = 0x04U,    /*!< The active channel is 3     */   
// HAL_TIM_ACTIVE_CHANNEL_4        = 0x08U,    /*!< The active channel is 4     */
// HAL_TIM_ACTIVE_CHANNEL_CLEARED  = 0x00    /*!< All active channels cleared */    

	tim_init1.Instance = TIM2;
	tim_init1.Channel = HAL_TIM_ACTIVE_CHANNEL_4;
	
	tim_init1.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_init1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_init1.Init.Prescaler = 650U;
	tim_init1.Init.Period = 990U;
	tim_init1.Init.AutoReloadPreload = 0;	

	HAL_TIM_PWM_Init(&tim_init1);

	TIM_ClockConfigTypeDef tim_clk_init;
	tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	tim_clk_init.ClockPrescaler = TIM_CLOCKDIVISION_DIV1;

	tim2_conf1.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	tim2_conf1.OCMode = TIM_OCMODE_PWM1; // Timer PWM mode 2, if Counter > Pulse, we set the output pin LOW till end of cycle
	tim2_conf1.OCNIdleState = TIM_OCIDLESTATE_RESET;
	tim2_conf1.Pulse = 10; // our duty cycle setting point

	HAL_TIM_PWM_ConfigChannel(&tim_init1, &tim2_conf1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&tim_init1, TIM_CHANNEL_4);


	// configure channel 2
	tim_init2.Instance = TIM2;
	tim_init2.Channel = HAL_TIM_ACTIVE_CHANNEL_3;
	
	tim_init2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_init2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_init2.Init.Prescaler = 650U;
	tim_init2.Init.Period = 990U;
	tim_init2.Init.AutoReloadPreload = 0;	

	HAL_TIM_PWM_Init(&tim_init2);

	//TIM_ClockConfigTypeDef tim_clk_init;
	tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	tim_clk_init.ClockPrescaler = TIM_CLOCKDIVISION_DIV1;

	tim2_conf2.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	tim2_conf2.OCMode = TIM_OCMODE_PWM1; // Timer PWM mode 2, if Counter > Pulse, we set the output pin LOW till end of cycle
	tim2_conf2.OCNIdleState = TIM_OCIDLESTATE_RESET;
	tim2_conf2.Pulse = 600; // our duty cycle setting point

	HAL_TIM_PWM_ConfigChannel(&tim_init2, &tim2_conf2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&tim_init2, TIM_CHANNEL_3);

	// configure channel 3
	tim_init3.Instance = TIM2;
	tim_init3.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	
	tim_init3.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_init3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_init3.Init.Prescaler = 650U;
	tim_init3.Init.Period = 990U;
	tim_init3.Init.AutoReloadPreload = 0;	

	HAL_TIM_PWM_Init(&tim_init3);

	// TIM_ClockConfigTypeDef tim_clk_init;
	// tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	// tim_clk_init.ClockPrescaler = TIM_CLOCKDIVISION_DIV1;

	tim2_conf3.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	tim2_conf3.OCMode = TIM_OCMODE_PWM1; // Timer PWM mode 2, if Counter > Pulse, we set the output pin LOW till end of cycle
	tim2_conf3.OCNIdleState = TIM_OCIDLESTATE_RESET;
	tim2_conf3.Pulse = 10; // our duty cycle setting point

	HAL_TIM_PWM_ConfigChannel(&tim_init3, &tim2_conf3, TIM_CHANNEL_1);
	//HAL_TIM_PWM_Start(&tim_init3, TIM_CHANNEL_1);

	//HAL_TIM_PWM_ConfigChannel(&tim_init3, &tim2_conf3, TIM_CHANNEL_4);
	//HAL_TIM_PWM_Start(&tim_init3, TIM_CHANNEL_4);


	// configure channel 4
	tim_init4.Instance = TIM2;
	tim_init4.Channel = HAL_TIM_ACTIVE_CHANNEL_2;
	
	tim_init4.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_init4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_init4.Init.Prescaler = 650U;
	tim_init4.Init.Period = 990U;
	tim_init4.Init.AutoReloadPreload = 0;	

	HAL_TIM_PWM_Init(&tim_init4);

	// TIM_ClockConfigTypeDef tim_clk_init;
	// tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	// tim_clk_init.ClockPrescaler = TIM_CLOCKDIVISION_DIV1;

	tim2_conf4.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	tim2_conf4.OCMode = TIM_OCMODE_PWM1; // Timer PWM mode 2, if Counter > Pulse, we set the output pin LOW till end of cycle
	tim2_conf4.OCNIdleState = TIM_OCIDLESTATE_RESET;
	tim2_conf4.Pulse = 600; // our duty cycle setting point

	HAL_TIM_PWM_ConfigChannel(&tim_init4, &tim2_conf4, TIM_CHANNEL_2);
	//HAL_TIM_PWM_Start(&tim_init4, TIM_CHANNEL_2);
	//HAL_TIM_PWM_ConfigChannel(&tim_init4, &tim2_conf4, TIM_CHANNEL_3);
	//HAL_TIM_PWM_Start(&tim_init4, TIM_CHANNEL_3);

  start_channel(1);
  start_channel(2);
  start_channel(3);
  start_channel(4);
}



void configure_channel(uint32_t channel_no, uint32_t new_pulse)
{

// HAL_TIM_ACTIVE_CHANNEL_1        = 0x01U,    /*!< The active channel is 1     */
// HAL_TIM_ACTIVE_CHANNEL_2        = 0x02U,    /*!< The active channel is 2     */
// HAL_TIM_ACTIVE_CHANNEL_3        = 0x04U,    /*!< The active channel is 3     */   
// HAL_TIM_ACTIVE_CHANNEL_4        = 0x08U,    /*!< The active channel is 4     */
// HAL_TIM_ACTIVE_CHANNEL_CLEARED  = 0x00    /*!< All active channels cleared */    
	uint32_t Channel;
	TIM_HandleTypeDef *p_tim_init;

	TIM_OC_InitTypeDef *p_tim2_conf;

	switch (channel_no)
	{
		case 1:
			{   
				p_tim_init = &tim_init1;
				p_tim2_conf = &tim2_conf1;  
				Channel = TIM_CHANNEL_4;

				// HAL_TIM_PWM_ConfigChannel(p_tim_init, p_tim2_conf, TIM_CHANNEL_1);
				// p_tim2_conf->Pulse = new_pulse; // our duty cycle setting point
				// HAL_TIM_PWM_ConfigChannel(p_tim_init, p_tim2_conf, TIM_CHANNEL_4);
				//HAL_TIM_PWM_Start(&tim_init3, TIM_CHANNEL_4);
			}
			break;
		case 2:
			{   
				p_tim_init = &tim_init2;
				p_tim2_conf = &tim2_conf2;
				Channel = TIM_CHANNEL_3;  
				// HAL_TIM_PWM_ConfigChannel(p_tim_init, p_tim2_conf, TIM_CHANNEL_2);
				// HAL_TIM_PWM_ConfigChannel(p_tim_init, p_tim2_conf, TIM_CHANNEL_3);
			}
			break;

		case 3:
			{   
				p_tim_init = &tim_init3;
				p_tim2_conf = &tim2_conf3;  
				Channel = TIM_CHANNEL_1;
				// p_tim2_conf->Pulse = new_pulse; // our duty cycle setting point
				// HAL_TIM_PWM_ConfigChannel(p_tim_init, p_tim2_conf, TIM_CHANNEL_1);
				// HAL_TIM_PWM_ConfigChannel(p_tim_init, p_tim2_conf, TIM_CHANNEL_4);
			}
			break;
		case 4:
			{   
				p_tim_init = &tim_init4;
				p_tim2_conf = &tim2_conf4;  
				Channel = TIM_CHANNEL_2;
				// HAL_TIM_PWM_ConfigChannel(p_tim_init, p_tim2_conf, TIM_CHANNEL_2);
				// HAL_TIM_PWM_ConfigChannel(p_tim_init, p_tim2_conf, TIM_CHANNEL_3);
			}
			break;	
	}
	
	// TIM_ClockConfigTypeDef tim_clk_init;
	// tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	// tim_clk_init.ClockPrescaler = TIM_CLOCKDIVISION_DIV1;

	
	p_tim2_conf->Pulse = new_pulse; // our duty cycle setting point
	//stop_channel(channel_no);
	HAL_TIM_PWM_ConfigChannel(p_tim_init, p_tim2_conf, Channel);
	//start_channel(channel_no);
	HAL_TIM_PWM_Start(p_tim_init, Channel);
	 	

}

void start_channel(uint32_t channel_no)
{
	uint32_t Channel;
	TIM_HandleTypeDef *p_tim_init = NULL;
	switch (channel_no)
	{
		case 1:
			{   
				p_tim_init = &tim_init1;
				Channel = TIM_CHANNEL_4;
			}
			break;
		case 2:
			{   
				p_tim_init = &tim_init2;
				Channel = TIM_CHANNEL_3;  
			}
			break;

		case 3:
			{   
				p_tim_init = &tim_init3;
				Channel = TIM_CHANNEL_1;
			}
			break;
		case 4:
			{   
				p_tim_init = &tim_init4;
				Channel = TIM_CHANNEL_2;
			}
			break;	
	}
	//HAL_TIM_PWM_ConfigChannel(&tim_init4, &tim2_conf4, Channel);
	//HAL_TIM_PWM_Start(&tim_init4, TIM_CHANNEL_2);
	//HAL_TIM_PWM_ConfigChannel(&tim_init4, &tim2_conf4, TIM_CHANNEL_3);
	//HAL_TIM_PWM_Start(&tim_init4, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(p_tim_init, Channel);
}

void stop_channel(uint32_t channel_no)
{
	uint32_t Channel;
	TIM_HandleTypeDef *p_tim_init;

	TIM_OC_InitTypeDef *p_tim2_conf;

	switch (channel_no)
	{
		case 1:
			{   
				p_tim_init = &tim_init1;
				Channel = TIM_CHANNEL_4;
			}
			break;
		case 2:
			{   
				p_tim_init = &tim_init2;
				Channel = TIM_CHANNEL_3;  
			}
			break;

		case 3:
			{   
				p_tim_init = &tim_init3;
				Channel = TIM_CHANNEL_1;
			}
			break;
		case 4:
			{   
				p_tim_init = &tim_init4;
				Channel = TIM_CHANNEL_2;
			}
			break;	
	}
	HAL_TIM_PWM_Stop(p_tim_init, Channel);
}