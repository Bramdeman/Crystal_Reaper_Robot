#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "LCD_screen.h"

static int distFront = 0;
static int distRear = 0;

static unsigned long lastLCDupdate = 0;
static const unsigned long LCDinterval = 200;

static LiquidCrystal_I2C lcd(0x27, 16, 2);   // LCD constructor

void LCD_init()
{	
	// set the proper screen options
	lcd.init();
	lcd.clear();
	lcd.backlight();

	// print a starting line...
	lcd.setCursor(0,0);
	lcd.print(" Crystal Reaper ");

	// and wait for sensor data to be ready
	lcd.setCursor(1,1);
	lcd.print("<Initializing>");
	
	delay(3000);		// watch out with delays, this will stop the whole code from executing instructions

	// ready the screen for printing sensor data
	lcd.setCursor(0, 0);
	lcd.print("SensorFront=    ");
	lcd.setCursor(0, 1);
	lcd.print("SensorRear =    ");
};

void LCD_print(int front, int rear)
{
	distFront = front;
	distRear = rear;
};

void LCD_update()
{
	unsigned long currentMillis = millis();

	if (currentMillis - lastLCDupdate < LCDinterval)			// set the screen refresh rate
		return;
	
	lastLCDupdate = currentMillis;

	// write front distance to screen
	lcd.setCursor(13, 0);	// first line
	lcd.print(distFront);

	if (distFront < 100.0) {
		lcd.setCursor(15, 0);
		lcd.print(" ");
	}
	
	// write rear distance to screen
	lcd.setCursor(13, 1);	// second line
	lcd.print(distRear);

	if (distRear < 100.0) {
		lcd.setCursor(15, 1);
		lcd.print(" ");
	}
};
