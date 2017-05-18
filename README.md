# Autonomous Parallel and Perpendicular parking Vehicle using STM32F3 Development Board

UART and PWM code acquired from  Abhi's GIT repo: https://github.com/Aghosh993/EL6483_EmbeddedSystems

### YouTube Links:

Perpendicular Parking: https://youtu.be/CBa3i3oduog
Parallel Parking: https://youtu.be/PyT3DjwcbR4


The following is the readme for the Autonomous Parking Project 

**Project Members**

Steven Pill	sp4207
John Vasquez    jv1065
Denny Mathew	dm4028
Harikrishnan G  hgn212
Harish P        hp1047


Details of Implementation

**Hardware Details**
	Board : STM32F3
	Sensor : HCSR04 Ultransonic Sensor(x3)
	Interface PCB 
	DC Motors (x2)
	Optical Encoder (x2)

**Components Used / Programmed**
	
* Motor
* PWM
* UltraSonic Sensors
* Encoders
* Interrupts

**Overall Design**

**Ultrasonic Sensors:**

The ultrasonic sensors need a trigger of 10us and after the trigger, the sonic waves will be sent by the hardware and the echo pin will be driven high proportional to the distance sensed.
	
Trigger : A 10us delay is implemented by a software delay. A software delay consists of counting in a software loop till a counter reaches the limit specified. Since the clock is 72 MHz we are counting till 72 for a microsecond delay. Done 10 times gives the 10us delay.

Echo : The echo was measured using two methods.
	
Method 1 : Employ polling the echo pin after the trigger is sent.  Though we were able to get correct distances the time spent on polling especially on open areas was messing the system. Hence we switched to an interrupt based approach with separate timers for each sensors to count the echo duration. 
	
Method 2 : The trigger pins were registered as external interrupt sources with Rising and Falling edge detection. The timer is started on the rising edge and stopped at the falling edge. The value of the corresponding timer's CNT register serves as the distance sensed in this case. The duration is converted to distance using the formula 

Distance = SpeedOfSound*(1/72000000))*(duration/2.0);

**Encoders:**
	
The Encoders were implemented by having a counter for the left and right wheel.  Whenever we were interested in turning a certain angle or moving a certain distance, we would enable the encoders.  In other words, we'd enable the GPIO Pins that we set up as pin change interrups and connected to the output of our encoder.  This way, we could count how many encoder ticks we were getting as we move.

**Motor And PWM:** 

Each DC motor has 2 channels . Depending on the high and low values across the two motors they rotate forward, back, stop etc.
PWM is generated using the timer 2 of the STM32 board. The timer 2 has a PwM configuration mode that can be tapped into to produce a PWM signal. The duty cycle of the signal determines the output velocity of the motor where a 100% duty cycle gives the full velocity of the motor. The PWM code was based on the demo code provided by Abhi. The GPIOs of the motors were configured in Alternate Function Mode (TIM2 mode) to get the individual channel output accross to the four GPIOS. The GPIOs drive the pair of motors.

**Mutexes:**

We implemented mutexes in our code base to account for random corruption of sensed distances that were updated in Interrupt Context. Accessing the sensed distances was, however, done as part of the main code which led us to believe synchronization issues can corrupt our sensed distances. We wrote a custom mutex API to acquire and release a mutex and synchronize the access of the global variables.

UART :
	The code for UART was taken directly from Abhis UART demo code and it was used for print based debugging.
	
Algorithmic Approach

	Perpendicular Parking
		
		The foundation of our algorithm was a state machine with the following states:
			STATE_IDLE,
			STATE_FOLLOW_WALL
		
			Each state was transitioned between by a call to "state_execute," which based on the current state and sensor values would return the move the robot
		should make next (discussed in more detail down below).  These moves were:
			LEFT,
			RIGHT,
			FORWARD,
			BACK,
			RECENTER_RIGHT,
			RECENTER_LEFT,
			STOP,
			PULL_IN_PARK
			
			Where we had a switch statement and would handle how each move should be made.
		
		Each state had the following purpose:
			STATE_IDLE - 
				Wait in this state until a call to find the next move of the robot is made.
				
			STATE_FOLLOW_WALL - 
				* Trigger left, front, and right ultrasonic sensors and wait for their echo response.  
				* Calculate the distance from each sensor.
				** Determine which move the robot should make.
				
					For determining what move the robot should make, we mostly used the front and right ultrasonic sensors.  We declared a threshold value for our 
				front sensor.  
					In the case that our front sensor received a value beneath this threshold, our robot would make a 90 degree turn to the left.
				We determined what a "90 degree turn" was by using rising edge triggered interrupts with our encoders.  In order to not have continuous, rapid 
				GPIO Pin change interrupts firing throughout our code, we made sure to enable these encoder pin-change interrupts when we wanted to do a turn,
				and similarly disdable them when we were done making our turn.
					Our robot was always right-aligned; therefore, in the case that the right sensor value went beneath or over two set threshold values, we
				made sure to make the robot move in such a way where it would run parallel to its right wall as best as possible.
					In the case where our right sensor value was suddenly a high value (again, a threshold we had set through trial and error), we knew we were
				now in the position to make a right turn.  However, in order to determine whether we should be turning right to find the parking spot, or turning
				right INTO a parking spot, we checked the left sensor as well.  If the left sensor distance was not that large and the right distance was
				greater than the width of the car, we decided that we should just be doing a right turn.  On the other hand, if the left sensor distance was large
				and the right sensor distance was, again, greater than the width of the car, then the next move should be to park.  When we get to this point,
				we know we are in the position where we should be pulling in to a parking spot.  This move uses the encoders to pull up a fixed amount of distance, 
				turn right 90 degrees, and then pull in to the parking spot and lock the code in an infinite loop as our stop "state".
				
					
	
	Parallel Parking
		
		For the Parallel Parking algorithm, we used mostly the same logic as above with the exception of the following:
			The possible moves to get from our 'state_execute' function were now:
				LEFT,
				RIGHT,
				FORWARD,
				BACK,
				RECENTER_RIGHT,
				RECENTER_LEFT,
				STOP,
				PARALLEL_PARK
			
			With the difference being "PULL_IN_PARK" was now instead "PARALLEL_PARK".
			The logic for our PARALLEL_PARK move was as follows:
				Once we are in PARALLEL_PARK, we know that we are where are parking spot should be.  However, we need to go beside the next car so that 
				we could back up into our parking spot.  Therefore, we do continuous FORWARD, "RECENTER_LEFT" and RECENTER_RIGHT" moves (to maintain parallel
				to the parking spot) until our right sensor distance suddenly drops below our threshold.  Then, we use our encoders to turn our car 45 degrees
				towards the parking spot, back up into the spot, then turn another 45 degrees in the opposite direction we initially did in order to straighten
				our vehicle.
