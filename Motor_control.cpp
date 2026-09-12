#include <Arduino.h>

#include "Motor_control.h"

// first define the variables holding the pin assignments
// (static here means that the defines variables are local to this file)
static const int enA = 11;
static const int enB = 12;

static const int dir1 = 30;
static const int dir2 = 31;
static const int dir3 = 32;
static const int dir4 = 33;

// and make variables for motor speed and direction
static int target_left = 0;
static int target_right = 0;

void motors_init()
{	// next step is to initialize the GPIOs being used
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	
	pinMode(dir1, OUTPUT);
	pinMode(dir2, OUTPUT);
	pinMode(dir3, OUTPUT);
	pinMode(dir4, OUTPUT);	
};

void motors_set(int left, int right)
{
	// update the local variables
	target_left = left;
	target_right = right;
};

void motors_update()
{
	// set the left motor direction
	if (target_left > 0) {
		digitalWrite(dir1, HIGH);
		digitalWrite(dir2, LOW);
	}
	else if (target_left < 0) {
		digitalWrite(dir1, LOW);
		digitalWrite(dir2, HIGH);
	}
	else {
		digitalWrite(dir1, LOW);
		digitalWrite(dir2, LOW);
	}
	
	// set the right motor direction
	if (target_right > 0) {
		digitalWrite(dir3, HIGH);
		digitalWrite(dir4, LOW);
	}
	else if (target_right < 0) {
		digitalWrite(dir3, LOW);
		digitalWrite(dir4, HIGH);
	}
	else {
		digitalWrite(dir3, LOW);
		digitalWrite(dir4, LOW);
	}
	
	// now set the correct speeds
	analogWrite(enA, abs(target_left));
	analogWrite(enB, abs(target_right));
};