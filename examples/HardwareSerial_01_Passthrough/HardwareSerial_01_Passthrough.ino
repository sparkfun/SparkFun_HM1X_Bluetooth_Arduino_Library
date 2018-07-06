/*
  HM1X Bluetooth HardwareSerial Passthrough
  By: Jim Lindblom
  SparkFun Electronics
  Date: July 3, 2017
  License: This code is public domain but you buy me a beer 
  if you use this and we meet someday (Beerware license).

  Initializes and connects to the HM1X module via 
  hardware serial on pins 0 and 1. Executes a serial
  pass-through once connected.

  Works well with a SparkFun SAMD21 Dev Breakout -- connecting
  pins 0 and 1 serial to the BT module.

  Once programmed, open the serial port and send a character to begin.

  Hardware Connections:
  Bluetooth Mate 4.0 --------- SAMD21 Dev Breakout
       GND ------------------------- GND
       5V -------------------------- 3.3V
       TX -------------------------- 0/RX
       RX -------------------------- 1/TX
*/

// Use Library Manager or download here: https://github.com/sparkfun/SparkFun_HM1X_Bluetooth_Arduino_Library
#include <SparkFun_HM1X_Bluetooth_Arduino_Library.h>

HM1X_BT bt; // Instance of HM1X bluetooth module class

#define SerialPort SerialUSB // Abstract serial monitor debug port

void setup() {
  SerialPort.begin(9600); // Serial debug port @ 9600 bps
  while (!SerialPort.available()) ; // Press a key to begin
  while (SerialPort.available()) SerialPort.read(); // Clear RX buffer
  SerialPort.println("Beginning HM1X BT serial test");

  // bt.begin --
  // in this case takes a Hardware Serial port and the
  // desired baud rate.
  // Returns true on success
  if (bt.begin(Serial1, 9600) == false) {
    SerialPort.println(F("Failed to connect to the HM-13."));
    while (1) ;
  }
  SerialPort.println("Ready to Bluetooth!");
}

void loop() {
  // If data is available from bt module, 
  // print it to serial port
  if (bt.available()) {
    SerialPort.write((char) bt.read());
  }
  // If data is available from serial port,
  // print it to bt module.
  if (SerialPort.available()) {
    bt.write((char) SerialPort.read());
  }
  
}
