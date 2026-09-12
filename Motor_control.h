#pragma once

// this function will setup the correct initial state of the two DC motors
void motors_init();

// use this function to give a setpoint to both motors using the two arguments
void motors_set(int left, int right);

// this function will update both speed and direction of the motors
void motors_update();