/*
  HM1X Bluetooth Qwiic (I2C) Device Configuration
  By: Jim Lindblom
  SparkFun Electronics
  Date: July 3, 2017
  License: This code is public domain but you buy me a beer 
  if you use this and we meet someday (Beerware license).

  Demonstrates how to configure the HM-13's EDR/BLE names,
  and how to reset the module. Also prints out EDR and BLE
  addresses (not configurable).

  Transitions into Serial passthrough once configured.

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

// BLE and EDR device names
String edrName = "MyEDR";
String bleName = "MyBLE";

void setup() {
  Serial.begin(9600); // Serial debug port @ 9600 bps

  // bt.begin --
  // in this case takes a HardwareSerial connection and
  // a desired serial baud rate.
  // Returns true on success
  if (bt.begin(Wire, 0x1B) == false) {
    Serial.println(F("Failed to connect to the HM-13."));
    while (1) ;
  }
  Serial.println("Ready to Bluetooth!");

  boolean resetRequired = false; // Reset is required on name change

  String setEdrName = bt.getEdrName();
  // getEdrName returns a string containing EDR device name
  if (setEdrName != edrName) {
    // If set name is different, configure new EDR name
    Serial.println("Setting new EDR name");
    // Set EDR device name
    if (bt.setEdrName(edrName) == HM1X_SUCCESS) {
      Serial.println("Set EDR name");
      resetRequired = true;
    }
  } else {
    Serial.println("EDR name is: " + setEdrName);
  }

  // getBleName returns a string containing BLE device name
  String setBleName = bt.getBleName();
  // getEdrName returns a string containing BLE device name
  if (setBleName != bleName) {
    // If set name is different, configure new BLE name
    Serial.println("Setting new BLE name");
    // Set BLE device name
    if (bt.setBleName(bleName) == HM1X_SUCCESS) {
      Serial.println("Set BLE name");
      resetRequired = true;
    }
  }
  
  Serial.println("EDR address: " + bt.edrAddress());
  Serial.println("BLE address: " + bt.bleAddress());

  if (resetRequired) {
    Serial.println("Resetting BT module. Wait a few seconds.");
    bt.reset();
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


