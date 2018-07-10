/*
  HM1X Bluetooth I2C (Qwiic) Poll/Passthrough
  By: Jim Lindblom
  SparkFun Electronics
  Date: July 3, 2017
  License: This code is public domain but you buy me a beer 
  if you use this and we meet someday (Beerware license).

  Initializes and connects to the HM1X module via 
  hardware serial on pins 0 and 1. Executes a I2C
  pass-through once connected. Inidicates connection
  when a device connects to it.

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
  if (bt.begin(Wire, QWIIC_BLUETOOTH_DEFAULT_ADDRESS) == false) {
    Serial.println("Failed to connect to the HM-13.");
    while (1) ;
  }
  Serial.println("Ready to Bluetooth!");

  // Call setupPoll to initialize poll buffer and to
  // tell the library that we'll be polling -- configures
  // module to indicate new connections.
  bt.setupPoll();
}

void loop() {
  // bt.connectedEdr and bt.connectedBle will return true
  // if connected to either EDR or BLE.
  static boolean connectedBle = bt.connectedBle();
  static boolean connectedEdr = bt.connectedEdr();

  // bt.poll will check for a BLE or EDR connection. Also
  // stores serial data received into a buffer to be 
  // read by bt.available.
  if (bt.poll()) {
    // If BT connection status has changed.
    if ((connectedBle != bt.connectedBle())) {
      if (bt.connectedBle()) {
        Serial.println("Connected to BLE!");
      } else {
        Serial.println("Disconnected from BLE");
      }
      connectedBle = bt.connectedBle();
    // If EDR connection status has changed
    } else if ((connectedEdr != bt.connectedEdr())) {
      if (bt.connectedEdr()) {
        Serial.println("Connected to EDR!");
      } else {
        Serial.println("Disconnected from EDR");
      }
      connectedEdr = bt.connectedEdr();
    }
  }
  // bt.poll returns true if it parsed a response
  // (e.g. connection status change). Otherwise Returns
  // false, and you can use available to read in BT serial data
  else {
    if (bt.available()) {
      Serial.write((char) bt.read());
    }
    if (Serial.available()) {
      bt.write((char) Serial.read());
    }
  }  
}
