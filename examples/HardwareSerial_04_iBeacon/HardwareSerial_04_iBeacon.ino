/*
  HM1X Bluetooth HardwareSerial iBeacon
  By: Jim Lindblom
  SparkFun Electronics
  Date: July 3, 2017
  License: This code is public domain but you buy me a beer 
  if you use this and we meet someday (Beerware license).

  Demonstrates how to configure the HM-13 as an iBeacon.
  Sets UUID, and major, and minor versions.

  Works well with a SparkFun SAMD21 Dev Breakout -- 
  connecting via hardware serial (D0, D1).

  Hardware Connections:
  Bluetooth Mate 4.0 --------- SparkFun SAMD21 Dev Breakou
       GND ----------------------------- GND
       3.3VV --------------------------- 3.3V
       TX ------------------------------ 0/RX
       RX ------------------------------ 1/TX
*/

// Use Library Manager or download here: https://github.com/sparkfun/SparkFun_HM1X_Bluetooth_Arduino_Library
#include <SparkFun_HM1X_Bluetooth_Arduino_Library.h>

HM1X_BT bt;

// Below sets UUID to:
// 01234567-89AB-CDEF-0123-457889ABCDEF
// (uuid0[0-7])-(uuid1[0-3])-(uuid1[4-7])-(uuid2[0-3])-(uuid2[4-7])(uuid3[0-7])
char * uuid0 = "01234567";
char * uuid1 = "89ABCDEF";
char * uuid2 = "01234567";
char * uuid3 = "89ABCDEF";

const unsigned int minor = 1;
const unsigned int major = 2;

#define SerialPort SerialUSB

void setup() {
  SerialPort.begin(9600); // Serial debug port @ 9600 bps
  while ( !SerialPort.available() ) ;
  SerialPort.read();

  // bt.begin --
  // in this case takes a HardwareSerial connection and
  // a desired serial baud rate.
  // Returns true on success
  if (bt.begin(Serial1, 9600) == false) {
    SerialPort.println(F("Failed to connect to the HM-13."));
    while (1) ;
  }

  // Enable iBeacon
  if (bt.iBeacon(true)) {
    SerialPort.println("iBeacon enabled");
  } 

  // Configure iBeacon UUID's
  if (bt.setiBeaconUUID(uuid0, uuid1, uuid2, uuid3) == HM1X_SUCCESS) {
    SerialPort.println("Set iBeacon UUID");
  }
  Serial.println("UUID: " + String(bt.getiBeaconUUID()));

  // Set iBeacon major version
  if (bt.setiBeaconMajor(minor) == HM1X_SUCCESS) {
    SerialPort.println("Set iBeaconMajor");
  }

  // Set iBeacon minor version
  if (bt.setiBeaconMinor(major) == HM1X_SUCCESS) {
    SerialPort.println("Set setiBeaconMinor");
  }

  // Reset to re-load device configuration
  if (bt.reset() == HM1X_SUCCESS) {
    SerialPort.println("Ready to iBeacon!");
  }
}

void loop() {
  if (bt.available()) {
    SerialPort.write((char) bt.read());
  }
  if (SerialPort.available()) {
    bt.write((char) SerialPort.read());
  } 
}
