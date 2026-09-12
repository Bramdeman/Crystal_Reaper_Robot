#pragma once

// this function will setup the ultrasonic distance sensors
void ultrasonic_init();

// use this function to read the latest distance measurement
int ultrasonic_get_front();
int ultrasonic_get_rear();

// calling this function will trigger the sensor and perform a measurement
void ultrasonic_update();

// We need a function to calculate the median of an array of measurements
int getMedian(float *buffer, int size);