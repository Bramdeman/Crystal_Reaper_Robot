#include <Arduino.h>

#include "Bluetooth.h"

#define BT_SERIAL 	Serial1			// pins 18 and 19 on Mega
#define BT_BAUD		9600

static char rxBuffer[64];
static uint8_t rxIndex = 0;
static bool commandReady = false;

static const int STATE_PIN_BLE = 50;	// not sure if we need those pins
static const int BRK_PIN_BLE = 51;		// but better to have them already

// let's first write the init function
void bluetooth_init()
{
	pinMode(STATE_PIN_BLE, INPUT);
	pinMode(BRK_PIN_BLE, OUTPUT);
	digitalWrite(BRK_PIN_BLE, HIGH);
	
	BT_SERIAL.begin(BT_BAUD);		// setup the second serial port with its baudrate
};

// Now we write the update function
void bluetooth_update()
{
	while (BT_SERIAL.available())		// stay in this function as long as there is data in the bluetooth buffer
	{
		char c = BT_SERIAL.read();		// store the first character in this variable

		//Serial.print(c);		// print the string to the serial monitor for debugging
		
		if (c == '\n')					// if character is a newline
		{
			rxBuffer[rxIndex] = '\0';
			commandReady = true;
			rxIndex = 0;

			Serial.println(rxBuffer);			// print the receive buffer for debugging
		}	
		else 							// in every other case
		{
			if (rxIndex < sizeof(rxBuffer) - 1)
			{
				rxBuffer[rxIndex++] = c;
			}
		}
	}
};

// This function is for other modules to check if there is data ready
bool bluetooth_hasCommand()
{
	return commandReady;
};

// And consequently we need a function to read data if it's ready
const char* bluetooth_getCommand()
{
	return rxBuffer;
};

// Use this function to indicate we used the data already
void bluetooth_clearFlag()
{
	commandReady = false;
}

// In case we want to send data back
void bluetooth_send(const char* msg)
{
	BT_SERIAL.println(msg);
};