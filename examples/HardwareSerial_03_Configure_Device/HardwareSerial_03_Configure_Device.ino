/*
  HM1X Bluetooth HardwareSerial Device Configuration
  By: Jim Lindblom
  SparkFun Electronics
  Date: July 3, 2017
  License: This code is public domain but you buy me a beer 
  if you use this and we meet someday (Beerware license).

  Demonstrates how to configure the HM-13's EDR/BLE names,
  and how to reset the module. Also prints out EDR and BLE
  addresses (not configurable).

  Transitions into Serial passthrough once configured.

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

#define SerialPort SerialUSB // Abstract serial monitor debug port

// BLE and EDR device names
String edrName = "MyEDR";
String bleName = "MyBLE";

void setup() {
  SerialPort.begin(9600); // Serial debug port @ 9600 bps
  while (!SerialPort.available()) ; // Press a key to begin
  while (SerialPort.available()) SerialPort.read(); // Clear RX buffer

  // bt.begin --
  // in this case takes a HardwareSerial connection and
  // a desired serial baud rate.
  // Returns true on success
  if (bt.begin(Serial1, 9600) == false) {
    SerialPort.println(F("Failed to connect to the HM-13."));
    while (1) ;
  }
  SerialPort.println("Ready to Bluetooth!");

  boolean resetRequired = false; // Reset is required on name change

  String setEdrName = bt.getEdrName();
  // getEdrName returns a string containing EDR device name
  if (setEdrName != edrName) {
    // If set name is different, configure new EDR name
    SerialPort.println("Setting new EDR name");
    // Set EDR device name
    if (bt.setEdrName(edrName) == HM1X_SUCCESS) {
      SerialPort.println("Set EDR name");
      resetRequired = true;
    }
  } else {
    SerialPort.println("EDR name is: " + setEdrName);
  }
  
  // getBleName returns a string containing BLE device name
  String setBleName = bt.getBleName();
  if (setBleName != bleName) {
    // If set name is different, configure new BLE name
    SerialPort.println("Setting new BLE name");
    // Set BLE device name
    if (bt.setBleName(bleName) == HM1X_SUCCESS) {
      SerialPort.println("Set BLE name");
      resetRequired = true;
    }
  } else {
    SerialPort.println("BLE name is: " + setBleName);
  }
  
  SerialPort.println("EDR address: " + bt.edrAddress());
  SerialPort.println("BLE address: " + bt.bleAddress());

  if (resetRequired) {
    SerialPort.println("Resetting BT module. Wait a few seconds.");
    bt.reset();
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