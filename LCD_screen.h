#pragma once

// initialize the LCD screen
void LCD_init();

// give the measured values to this LCD subblock
void LCD_print(int front, int rear);

// print the values to the screen
void LCD_update();