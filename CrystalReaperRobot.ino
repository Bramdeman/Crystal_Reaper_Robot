#include "Motor_control.h"
#include "Ultrasonic_sensor.h"
#include "LCD_screen.h"
#include "Bluetooth.h"
#include "Robot_App.h"

void setup() {
  // This code will run only once and is used to initialize stuff
  motors_init();
  ultrasonic_init();
  LCD_init();
  bluetooth_init();
  robot_logic_init();   
}

void loop() {
  // Here we will implement the main functionality of the robot
  motors_update();
  ultrasonic_update();
  LCD_update();
  bluetooth_update();
  robot_logic_update();  
}
