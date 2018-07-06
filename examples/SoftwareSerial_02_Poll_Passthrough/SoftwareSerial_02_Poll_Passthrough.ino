/*
  HM1X Bluetooth SoftwareSerial Poll/Passthrough
  By: Jim Lindblom
  SparkFun Electronics
  Date: July 3, 2017
  License: This code is public domain but you buy me a beer 
  if you use this and we meet someday (Beerware license).

  Initializes and connects to the HM1X module via 
  software serial on pins 3 and 4. 

  Uses the library's polling function to check for new
  BLE or EDR connections.
  
  Executes a SoftSerial pass-through once connected. 
  Inidicates connection when a device connects to it.

  Works well with a SparkFun RedBoard -- connecting
  via SoftwareSerial (D3, D4)

  Hardware Connections:
  Bluetooth Mate 4.0 --------- SparkFun Blackboard
       GND ----------------------------- GND
       3.3VV -------------------------- 3.3V
       TX ------------------------------ 3
       RX ------------------------------ 4
*/

// Use Library Manager or download here: https://github.com/sparkfun/SparkFun_HM1X_Bluetooth_Arduino_Library
#include <SparkFun_HM1X_Bluetooth_Arduino_Library.h>
#include <SoftwareSerial.h>

HM1X_BT bt;
SoftwareSerial hm13Serial(3, 4); // RX, TX on Arduino

void setup() {
  Serial.begin(9600); // Serial debug port @ 9600 bps

  // bt.begin --
  // in this case takes a SoftwareSerial connection and
  // a desired serial baud rate.
  // Returns true on success
  if (bt.begin(hm13Serial, 9600) == false) {
    Serial.println(F("Failed to connect to the HM-13."));
    while (1) ;
  }
  Serial.println("Ready to Bluetooth!");

  bt.setupPoll();
}

void loop() {
  static boolean connectedBle = bt.connectedBle();
  static boolean connectedEdr = bt.connectedEdr();
  if (bt.poll()) {
    if ((connectedBle != bt.connectedBle())) {
      if (bt.connectedBle()) {
        Serial.println("Connected to BLE!");
      } else {
        Serial.println("Disconnected from BLE");
      }
      connectedBle = bt.connectedBle();
    } else if ((connectedEdr != bt.connectedEdr())) {
      if (bt.connectedEdr()) {
        Serial.println("Connected to EDR!");
      } else {
        Serial.println("Disconnected from EDR");
      }
      connectedEdr = bt.connectedEdr();
    }
  }
  else {
    if (bt.available()) {
      Serial.write((char) bt.read());
    }
    if (Serial.available()) {
      bt.write((char) Serial.read());
    }
  }  
}
