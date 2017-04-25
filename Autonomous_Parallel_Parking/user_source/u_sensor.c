/*
  This is the main user software application
  In this case, we have below the code necessary to set up and control usensor.

 */
#include "stm32f3xx_hal.h"
#include "u_sensor"
long duration;
int distance;
int inches;

//GPIO initializations
void usensor_gpio_init(void)
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

//Delay 1us x n 
void delay_us(uint32_t n) // Or n*72 clock cycles
{
  
  uint32_t i = 0U;
  uint32_t j = 0U;

  for(i = n; i > 0; --i)
  {
    for(j = 12; j > 0; --j)
    {
      asm("nop");
    }
  }
}

/*
void cpu_sw_delay(uint32_t n)
{
  uint32_t i = 0U;
  uint32_t j = 0U;

  for(i = 0U; i < n; ++i)
  {
    for(j = 0U; j < 72; ++j) //72 CPU ticks per microsecond
    {
      asm("nop");
    }
  }
}

*/

  //Measures a HIGH pulse
  long pulse_in(GPIO_TypeDef* echo_port) 
  {
    unsigned long counter = 0;
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
    }

    //Every 72 clock cycles yields 1 microseconds 
    return counter/72;
  }


  int get_sensor_dist(GPIO_TypeDef* trig_port, GPIO_TypeDef* echo_port, uint16_t trigger)
  {
    //Preparing the trigger pin.
    HAL_GPIO_WritePin(trig_port, trigger, GPIO_PIN_RESET);    
    delay_us(2);                                                
    //10us sensor required initiation pulse on the trigger pin. 
    HAL_GPIO_WritePin(trig_port, trigger, GPIO_PIN_SET);   
    delay_us(10);                                                   
    HAL_GPIO_WritePin(trig_port, trigger, GPIO_PIN_RESET);   
    //pulse_in returns the sound wave travel time in microseconds
    duration = pulse_in(echo_port);
    distance = duration*.0133858/2;
    inches = distance%12;
    //Delay 50ms as per manufacture recomendation 
    delay_us(50000); 
    return inches;
  }

  
  

// Arduino example 
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