/*
  HM1X Bluetooth SoftwareSerial
  By: Jim Lindblom
  SparkFun Electronics
  Date: July 3, 2017
  License: This code is public domain but you buy me a beer 
  if you use this and we meet someday (Beerware license).

  Initializes and connects to the HM1X module via 
  SoftwareSerial on pins 3 and 4. Executes a serial
  pass-through once connected.

  Hardware Connections:
  Bluetooth Mate 4.0 --------- Arduino Uno/RedBoard
       GND ------------------------- GND
       5V -------------------------- 5V
       TX -------------------------- 3 
       RX -------------------------- 4
*/

// Use software serial to communicate with the BT module
#include <SoftwareSerial.h>
// Use Library Manager or download here: https://github.com/sparkfun/SparkFun_HM1X_Bluetooth_Arduino_Library
#include <SparkFun_HM1X_Bluetooth_Arduino_Library.h>

// BT module TX connected to 3, RX connected to 4
SoftwareSerial hm13Serial(3, 4); // RX, TX on Arduino

HM1X_BT bt;

void setup() {
  Serial.begin(9600); // Serial debug port @ 9600 bps
  Serial.println("Press any key to begin!");
  while (!Serial.available()) ; Serial.read();

  // bt.begin --
  // in this case takes a Software Serial port and the
  // desired baud rate.
  // Returns true on success
  if (bt.begin(hm13Serial, 9600) == false) {
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