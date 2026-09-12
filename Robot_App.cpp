#include <Arduino.h>

#include "Robot_App.h"
#include "Ultrasonic_sensor.h"
#include "LCD_screen.h"
#include "Motor_control.h"
#include "Bluetooth.h"

// define modes of the robot
enum RobotMode
{
	AUTONOMOUS,
	//DABBLE,
	BLUETOOTH
};

// define states of the robot (like an FSM)
enum RobotState
{
	IDLE,
	DRIVE_FORWARD,
	DRIVE_BACKWARD,
	TURN_LEFT,
	TURN_RIGHT,
	FLIP
};

// define a variable holding the current mode and state
static RobotMode mode;
static RobotState prev_state, state, next_state;

static unsigned long idleStartTime;

static unsigned long turnStartTime;
static unsigned long turnDuration;

static int distFront, distRear;				// the measured distance from the ultrasonic sensors

void bluetooth_manual_control()
{
	if (!bluetooth_hasCommand())
		return;

	const char* cmd = bluetooth_getCommand();

	//Serial.println(cmd);

	if (strcmp(cmd, "F") == 0)					// forward
		motors_set(200, 200);
	else if (strcmp(cmd, "B") == 0)			// backward
		motors_set(-180, -180);
	else if (strcmp(cmd, "R") == 0)			// left
		motors_set(-150, 150);
	else if (strcmp(cmd, "L") == 0)			// right
		motors_set(150, -150);
	else if (strcmp(cmd, "S") == 0)			// stop
		motors_set(0, 0);
	else																// all other commands
		bluetooth_send("This is not a valid bluetooth command, retaining current state.");

	bluetooth_clearFlag();
}

void autonomous_update()
{
	// FSM implementation
	switch(state)
	{
		case IDLE :
		{
			motors_set(0, 0);			// stop the motors

			if (millis() - idleStartTime >= 3000)		// the wait is over
			{
				Serial.println("Timer done");

				if (next_state == TURN_RIGHT || next_state == TURN_LEFT || next_state == FLIP)
				{
					turnDuration = random(400, 1000);		// in ms
					turnStartTime = millis();						// we only need

					Serial.print("Turn time is ");
					Serial.print(turnDuration);
					Serial.println(" milliseconds");
				}

				state = next_state;
			}
			break;
		}

		case DRIVE_FORWARD :
		{
			motors_set(200, 200);
			
			if (distFront < 30)
			{
				state = IDLE;
				if (prev_state == TURN_RIGHT)
				{
					Serial.println("Switch to Turn left");
					next_state = TURN_LEFT;
				}
				else if (prev_state == TURN_LEFT)
				{
					Serial.println("Switch to Backward");
					next_state = DRIVE_BACKWARD;
				}
				else
				{
					Serial.println("Switch to Turn right");
					next_state = TURN_RIGHT;
				}
				
				idleStartTime = millis();
				prev_state = DRIVE_FORWARD;
			}
			break;		// to prevent it from immediately going into the next case
		}
		
		case DRIVE_BACKWARD :
		{
			motors_set(-140, -140);
			
			if (distRear < 20)
			{
				Serial.println("Switch to Flip");
				
				state = IDLE;
				next_state = FLIP;
				idleStartTime = millis();
				prev_state = DRIVE_BACKWARD;
			}
			break;
		}
		
		case TURN_LEFT :
		{
			motors_set(180, -180);		// turn left
			
			if (millis() - turnStartTime >= turnDuration)
			{
				Serial.println("Switch to Forward");
				
				state = IDLE;
				next_state = DRIVE_FORWARD;
				idleStartTime = millis();
				prev_state = TURN_LEFT;
			}
			break;
		}

		case TURN_RIGHT :
		{
			motors_set(-180, 180);		// turn right
			
			if (millis() - turnStartTime >= turnDuration)
			{
				Serial.println("Switch to Forward");
				
				state = IDLE;
				next_state = DRIVE_FORWARD;
				idleStartTime = millis();
				prev_state = TURN_RIGHT;
			}
			break;
		}

		case FLIP :
		{
			motors_set(180, -180);		// turn left
			
			if (millis() - turnStartTime >= 1000)			// turn around
			{
				Serial.println("Switch to Forward");
				
				state = IDLE;
				next_state = DRIVE_FORWARD;
				idleStartTime = millis();
				prev_state = FLIP;
			}
			break;
		}
	}
}

// let's initialize our general application
void robot_logic_init()
{
	// We'll use the serial monitor to view the sensor output
	Serial.begin(115200); 
	
	Serial.println("Begin in bluetooth mode");

	mode = BLUETOOTH;

	Serial.println("Begin in the idle state");

	prev_state = DRIVE_BACKWARD;
	state = IDLE;
	next_state = DRIVE_FORWARD;
	idleStartTime = millis();

	randomSeed(analogRead(A0));		// pseudo random seed
};

// And this will be the actual functionality
void robot_logic_update()
{
	//int distFront, distRear;

	// update distance data
	distFront = ultrasonic_get_front();
	distRear = ultrasonic_get_rear();
	
	// and write it to the LCD screen
	LCD_print(distFront, distRear);

	// now check if we're switching from bluetooth mode to autonomous mode or vice versa
	if (bluetooth_hasCommand())
	{
		const char* cmd = bluetooth_getCommand();				// store the string received via bluetooth

		//Serial.println(cmd);
		//Serial.println(strcmp(cmd, "AUTO"));

		if (strcmp(cmd, "AUTO") == 0 )
		{
			mode = AUTONOMOUS;
			Serial.println("Mode = AUTONOMOUS");
			bluetooth_clearFlag();
			bluetooth_send("Mode: Autonomous");
		}
		else if (strcmp(cmd, "BT") == 0)
		{
			mode = BLUETOOTH;
			Serial.println("Mode = BLUETOOTH");
			bluetooth_clearFlag();
			bluetooth_send("Mode: Bluetooth");
			motors_set(0, 0);				// make sure we stop the vehicle when switching to this mode
		}
	}

	// and finally execute the right function based on operating mode
	if (mode == BLUETOOTH)
	{
		bluetooth_manual_control();
	}
	else
	{
		autonomous_update();
	}
	
};