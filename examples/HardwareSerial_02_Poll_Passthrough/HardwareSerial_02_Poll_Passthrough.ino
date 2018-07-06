/*
  HM1X Bluetooth HardwareSerial Poll/Passthrough
  By: Jim Lindblom
  SparkFun Electronics
  Date: July 3, 2017
  License: This code is public domain but you buy me a beer 
  if you use this and we meet someday (Beerware license).

  Initializes and connects to the HM1X module via 
  hardware serial on pins 0 and 1. 

  Uses the library's polling function to check for new
  BLE or EDR connections.
  
  Executes a Hardware pass-through once connected. Inidicates 
  connection when a device connects to it.

  Works well with a SparkFun SAMD21 Dev Breakout -- 
  connecting via hardware serial (D0, D1).

  Hardware Connections:
  Bluetooth Mate 4.0 --------- SparkFun SAMD21 Dev Breakou
       GND ----------------------------- GND
       3.3VV -------------------------- 3.3V
       TX ------------------------------ 0/RX
       RX ------------------------------ 1/TX
*/

// Use Library Manager or download here: https://github.com/sparkfun/SparkFun_HM1X_Bluetooth_Arduino_Library
#include <SparkFun_HM1X_Bluetooth_Arduino_Library.h>

HM1X_BT bt;

#define SerialPort SerialUSB // Abstract serial monitor debug port

void setup() {
  SerialPort.begin(9600); // Serial debug port @ 9600 bps

  // bt.begin --
  // in this case takes a HardwareSerial connection and
  // a desired serial baud rate. (9600 recommended)
  // Returns true on success
  if (bt.begin(Serial1, 9600) == false) {
    SerialPort.println(F("Failed to connect to the HM-13."));
    while (1) ;
  }
  SerialPort.println("Ready to Bluetooth!");

  // Call setupPoll to initialize poll buffer and to
  // tell the library that we'll be polling -- configures
  // module to indicate new connections.
  bt.setupPoll();
}

void loop() {
  // bt.connectedEdr and bt.connectedBle will return true
  // if connected to either EDR or BLE.
  static boolean lastConnectedBle = bt.connectedBle();
  static boolean lastConnectedEdr = bt.connectedEdr();

  // bt.poll will check for a BLE or EDR connection. Also
  // stores serial data received into a buffer to be 
  // read by bt.available.
  if (bt.poll()) {
    // If BT connection status has changed.
    if ((lastConnectedBle != bt.connectedBle())) {
      if (bt.connectedBle()) {
        SerialPort.println("Connected to BLE!");
      } else {
        SerialPort.println("Disconnected from BLE");
      }
      lastConnectedBle = bt.connectedBle();
    // If EDR connection status has changed
    } else if ((lastConnectedEdr != bt.connectedEdr())) {
      if (bt.connectedEdr()) {
        SerialPort.println("Connected to EDR!");
      } else {
        SerialPort.println("Disconnected from EDR");
      }
      lastConnectedEdr = bt.connectedEdr();
    }
  }
  // bt.poll returns true if it parsed a response
  // (e.g. connection status change). Otherwise Returns
  // false, and you can use available to read in BT serial data
  else {
    if (bt.available()) {
      SerialPort.write((char) bt.read());
    }
    if (SerialPort.available()) {
      bt.write((char) SerialPort.read());
    }
  }  
}
