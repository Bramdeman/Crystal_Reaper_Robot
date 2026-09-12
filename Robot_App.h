#pragma once

// again we want an init function...
void robot_logic_init();

// ... and an update function
void robot_logic_update();

// we need this function for the bluetooth implementation...
void bluetooth_manual_control();

// ...and for now we move our FSM to the autonomous function
void autonomous_update();