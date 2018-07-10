/*
  HM1X Bluetooth Qwiic iBeacon
  By: Jim Lindblom
  SparkFun Electronics
  Date: July 3, 2017
  License: This code is public domain but you buy me a beer 
  if you use this and we meet someday (Beerware license).

  Demonstrates how to configure the HM-13 as an iBeacon.
  Sets UUID, and major, and minor versions.

  Works well with a SparkFun BlackBoard/Redboard -- 
  connecting via Qwiic.

  Hardware Connections:
  Bluetooth Mate 4.0 --------- SparkFun Blackboard
       GND ----------------------------- GND
       3.3VV -------------------------- 3.3V
       SDA -------------------------- (Qwiic) SDA
       SCL -------------------------- (QWiic) SCL
*/

// Use Library Manager or download here: https://github.com/sparkfun/SparkFun_HM1X_Bluetooth_Arduino_Library
#include <SparkFun_HM1X_Bluetooth_Arduino_Library.h>

HM1X_BT bt;
char * uuid0 = "01234567";
char * uuid1 = "89ABCDEF";
char * uuid2 = "01234567";
char * uuid3 = "89ABCDEF";

void setup() {
  Serial.begin(9600); // Serial debug port @ 9600 bps

  // bt.begin --
  // in this case takes a HardwareSerial connection and
  // a desired serial baud rate.
  // Returns true on success
  if (bt.begin(Wire, QWIIC_BLUETOOTH_DEFAULT_ADDRESS) == false) {
    Serial.println("Failed to connect to the HM-13.");
    while (1) ;
  }

  // Enable iBeacon
  if (bt.iBeacon(true)) {
    Serial.println("iBeacon enabled");
  } 

  // Configure iBeacon UUID's
  if (bt.setiBeaconUUID(uuid0, uuid1, uuid2, uuid3) == HM1X_SUCCESS) {
    Serial.println("Set iBeacon UUID");
  }
  Serial.println("UUID: " + String(bt.getiBeaconUUID()));

  // Set iBeacon major version
  if (bt.setiBeaconMajor(5) == HM1X_SUCCESS) {
    Serial.println("Set iBeaconMajor");
  }

  // Set iBeacon minor version
  if (bt.setiBeaconMinor(6) == HM1X_SUCCESS) {
    Serial.println("Set setiBeaconMinor");
  }

  // Reset to re-configure device
  if (bt.reset() == HM1X_SUCCESS) {
    Serial.println("Ready to iBeacon!");
  }
}

void loop() {
  if (bt.available()) {
    Serial.write((char) bt.read());
  }
  if (Serial.available()) {
    bt.write((char) Serial.read());
  } 
}