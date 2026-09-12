#include <Arduino.h>

#include "Ultrasonic_sensor.h"

#define FILTER_SIZE 10				// find a suitable value for how many measurements to keep

static float frontBuffer[FILTER_SIZE];
static int frontIndex = 0;
static float rearBuffer[FILTER_SIZE];
static int rearIndex = 0;

// start with defining pin assignments for both sensors
static const int TRIG_PIN_FRONT = 44;
static const int ECHO_PIN_FRONT = 45;

static const int TRIG_PIN_REAR = 42;
static const int ECHO_PIN_REAR = 43;

// define the local distance variables
static int distance_front = 0;
static int distance_rear = 0;

// Anything over 400 cm (23200 us pulse) is "out of range"
static const unsigned int MAX_DIST = 23200;

// calculate median
int getMedian(float *buffer, int size)
{
	float temp[FILTER_SIZE];

	// Copy buffer
	for (int i = 0; i < size; i++)
	{
		temp[i] = buffer[i];
	}

	// Insertion sort
	for (int i = 1; i < size; i++)
	{
		float key = temp[i];
		int j = i - 1;

		while (j >= 0 && temp[j] > key) 
		{
			temp[j+1] = temp[j];
			j--;
		}
		temp[j+1] = key;
	}

	// Return median
	if (size % 2 == 0)
	{
		return (temp[size/2 - 1] + temp[size/2]) / 2.0;
	}
	else 
	{
		return temp[size/2];
	}
}

void ultrasonic_init()
{
	// trig pin sends the pulse, and echo receives it back
	pinMode(TRIG_PIN_FRONT, OUTPUT);
	digitalWrite(TRIG_PIN_FRONT, LOW);
	pinMode(ECHO_PIN_FRONT, INPUT);

	// make sure the sensor is idle after initialization
	pinMode(TRIG_PIN_REAR, OUTPUT);
	digitalWrite(TRIG_PIN_REAR, LOW);
	pinMode(ECHO_PIN_REAR, INPUT);
};

int ultrasonic_get_front()
{
	//int frontMedian = getMedian(frontBuffer, FILTER_SIZE);
	//return frontMedian;
	return distance_front;
};

int ultrasonic_get_rear()
{
	//int rearMedian = getMedian(rearBuffer, FILTER_SIZE);
	//return rearMedian;
	return distance_rear;
};

// this function needs to perform the measurement and update the distance values
void ultrasonic_update()
{
	// ToDo: find a way to translate the sensFront() and sensRear() functions
	// do we need the delay? Is the only alternative an interrupt?
	
	unsigned long t1, t2;
	unsigned long pulse_width;

	// send the trigger pulse front
	digitalWrite(TRIG_PIN_FRONT, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN_FRONT, LOW);

	// Wait for pulse on echo pin
	while ( digitalRead(ECHO_PIN_FRONT) == 0 );

	// Measure how long the echo pin was held high (pulse width)
	// Note: the micros() counter will overflow after ~70 min
	t1 = micros();
	while ( digitalRead(ECHO_PIN_FRONT) == 1);
	t2 = micros();
	pulse_width = t2 - t1;
	float front_measure = pulse_width / 58.0;
	distance_front = (int)front_measure;								// update global variable

	// add the new front meaurement to the buffer
	frontBuffer[frontIndex] = front_measure;
	frontIndex++;

	if (frontIndex >= FILTER_SIZE)
		frontIndex = 0;

	// send the trigger pulse rear
	digitalWrite(TRIG_PIN_REAR, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN_REAR, LOW);
	
	// Wait for pulse on echo pin
	while ( digitalRead(ECHO_PIN_REAR) == 0 );

	// Measure how long the echo pin was held high (pulse width)
	// Note: the micros() counter will overflow after ~70 min
	t1 = micros();
	while ( digitalRead(ECHO_PIN_REAR) == 1);
	t2 = micros();
	pulse_width = t2 - t1;
	float rear_measure = pulse_width / 58.0;
	distance_rear = (int)rear_measure;									// update global variable

	// add the new rear measurement to the buffer
	rearBuffer[rearIndex] = rear_measure;
	rearIndex++;

	if (rearIndex >= FILTER_SIZE)
		rearIndex = 0;
	
};