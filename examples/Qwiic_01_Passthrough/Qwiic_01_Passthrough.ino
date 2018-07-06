/*
  HM1X Bluetooth I2C (Qwiic) Passthrough
  By: Jim Lindblom
  SparkFun Electronics
  Date: July 3, 2017
  License: This code is public domain but you buy me a beer 
  if you use this and we meet someday (Beerware license).

  Initializes and connects to the HM1X module via 
  hardware serial on pins 0 and 1. Executes a I2C
  pass-through once connected.

  Works well with a SparkFun Blackboard -- connecting
  Qwiic connectors together.

  Hardware Connections:
  Bluetooth Mate 4.0 --------- SparkFun Blackboard
       GND ----------------------------- GND
       3.3VV -------------------------- 3.3V
       SDA -------------------------- (Qwiic) SDA
       SCL -------------------------- (QWiic) SCL
*/

// Use Library Manager or download here: https://github.com/sparkfun/SparkFun_HM1X_Bluetooth_Arduino_Library
#include <SparkFun_HM1X_Bluetooth_Arduino_Library.h>
#include <Wire.h>

HM1X_BT bt;

void setup() {
  Serial.begin(9600); // Serial debug port @ 9600 bps

  // bt.begin --
  // in this case takes a TwoWire (I2C) port and the
  // I2C address
  // Returns true on success
  if (bt.begin(Wire, 0x1B) == false) {
    Serial.println(F("Failed to connect to the HM-13."));
    while (1) ;
  }
  Serial.println("Ready to Bluetooth!");
}

void loop() {
  // If data is available from bt module, 
  // print it to serial port
  if (bt.available()) {
    Serial.write((char) bt.read());
  }
  // If data is available from serial port,
  // print it to bt module.
  if (Serial.available()) {
    bt.write((char) Serial.read());
  }
  
}
