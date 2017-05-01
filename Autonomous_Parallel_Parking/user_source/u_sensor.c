/*
  This is the main user software application
  In this case, we have below the code necessary to toggle a set of LEDs on a STM32 F3 Discovery PCB at 20 Hz.

  (c) Abhimanyu Ghosh, 2017
 */
#include "stm32f3xx_hal.h"
#include "cpu.h"
#include "board_led.h"
#include "foo.h"

long duration;
int distance;
int feet;
int inches;
uint32_t sysTickCounter = 0;
TIM_HandleTypeDef htim7;
int counter =0;
//GPIO initializations
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
//-----------------------------------------------------------TEST
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//----------------------------------------------------------
  /*Configure GPIO pins : PF9 PF6 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PE10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PD0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* TIM7 init function */
void MX_TIM7_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 72;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  // 1/20 second
  htim7.Init.Period = 50000;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

}

void TIM7_IRQHandler()
{
    HAL_TIM_IRQHandler(&htim7);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  counter++;
    //HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
}
//Delay 1us x n 
/*
void delay_us(uint32_t n) // Or n*72 clock cycles
{
  
  uint32_t i = 0U;
  uint32_t j = 0U;

  for(i = n; i > 0; --i) 
  {
    for(j = 12; j > 0; --j) //72
    {
      asm("nop");
    }
  }
}
*/
void delay_us(uint32_t n)
{
  counter = 0;
  HAL_NVIC_EnableIRQ(TIM7_IRQn);
  while(counter<20){}
  HAL_NVIC_DisableIRQ(TIM7_IRQn);
 
}

  //Measures a HIGH pulse
  long pulse_in(GPIO_TypeDef* echo_port, uint16_t echo) 
  {
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
    unsigned long counter = 0;
    while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11) == 0) {}
    while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11) == 1) {
        ++counter;
        asm("nop");
      }
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
    /*
    //Sensor#0 echo port
    if(echo_port == GPIOE) {
      //Stall and wait for echo PE11 to go high.
      while((echo_port->IDR & 0x0800) == 0) {} 
      //Time the pulse. Start counting clock cycles until the echoPin goes low.
      while((echo_port->IDR & 0x0800) == 1) {
        ++counter;
        asm("nop");
      }
    }
    //Sensor#1 echo port
    else if(echo_port == GPIOF) {    
      //Stall and wait for echo PF6 to go high.
      while((echo_port->IDR & 0x0040) == 0) {} 
      while((echo_port->IDR & 0x0800) == 1) {
        ++counter;
        asm("nop");
      }
    }
    //Sensor#2 echo port
    else if(echo_port == GPIOA) {
      //Stall and wait for echo PA13 to go high.
      while((echo_port->IDR & 0x2000) == 0) {} 
      while((echo_port->IDR & 0x0800) == 1) {
        ++counter;
        asm("nop");
      }
    }*/
  
    //Every 72 clock cycles yields 1 microseconds 
    return counter/72;
  }

  int get_sensor_dist(GPIO_TypeDef* trig_port, GPIO_TypeDef* echo_port, uint16_t trigger, uint16_t echo)
  {
    //Preparing the trigger pin.
    HAL_GPIO_WritePin(trig_port, trigger, GPIO_PIN_RESET);    
    delay_us(2);                                                
    //10us sensor required initiation pulse on the trigger pin. 
    HAL_GPIO_WritePin(trig_port, trigger, GPIO_PIN_SET);   
    delay_us(10);                                                   
    HAL_GPIO_WritePin(trig_port, trigger, GPIO_PIN_RESET);   
    //pulse_in returns the sound wave travel time in microseconds
    duration = pulse_in(echo_port, echo);
    distance = duration*.0133858/2;
    inches = distance%12;
    //Delay 50ms as per manufacture recomendation 
    delay_us(50000); 
    return inches;
  }


  
int main()
{
  /*
    Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
   */
  cpu_init();
  /*
    Initialize the GPIO (General-Purpose I/O) subsystem pins that are connected to the LEDs on the board:
   */
  board_led_init();

  //Initialize all GPIO for ultrasonic sensors.
  MX_GPIO_Init();

  //Initialize timer2
  MX_TIM7_Init();
  HAL_TIM_Base_Start(&htim7);
  HAL_NVIC_SetPriority(TIM7_IRQn, 3, 3);
  //InitializeTimer();
  int i = 0;

  /*
    In an infinite loop, keep toggling the LEDs in an alternating pattern:
   */
  while(1)
  {
      
      //put in port (GPIO_TypeDef *) GPIOE
      /*i = get_sensor_dist(GPIOE , GPIOE, GPIO_PIN_10, GPIO_PIN_11);
      if(i >= 2) {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
      }
      else {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
      }*/
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET); 
      delay_us(0);
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); 
      delay_us(0);
      /*
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);    //Clears the trigPin
      delay_us(2); //Delay usec
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET); 
      //10us Initiation pulse      
      delay_us(10);
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
      //Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPin);
      // Calculating the distance
      distance = duration*.0133858/2;
      inches = distance%12;
      //Delay 50ms as per manufacture recomendation 
      delay_us(50000); 
      
      board_led_on(LED1);
      board_led_off(LED2);

      delay_us(72000000U);  // Invoke a simple software busy-wait routine to delay approximately 50 milliseconds

      board_led_off(LED1);
      board_led_on(LED2);

      delay_us(72000000U);
      */
    }

  return 0;
}




// defines pins numbers
/*
const int trigPin = 9; 
const int echoPin = 10;

// defines variables
long duration;
int distance;
int feet;
int inches;

void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication
}

void loop() {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*.0133858/2;
// Prints the distance on the Serial Monitor
feet = distance/12;
inches = distance%12;
Serial.print("Distance: ");
Serial.print(feet); 
Serial.print(" ft ");
Serial.print(inches);
Serial.print(" in\n");
delay(500);
}*/
/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/

//-----------------------------timer initializations------------------
/*
static void MX_TIM7_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 72;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 50000;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}
*/