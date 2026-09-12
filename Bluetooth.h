#pragma once

// initialize the bluetooth module
void bluetooth_init();

// this is the function that runs in the arduino loop
void bluetooth_update();

// the following function names are pretty self explanatory
bool bluetooth_hasCommand();
const char* bluetooth_getCommand();
void bluetooth_clearFlag();
void bluetooth_send(const char* msg);