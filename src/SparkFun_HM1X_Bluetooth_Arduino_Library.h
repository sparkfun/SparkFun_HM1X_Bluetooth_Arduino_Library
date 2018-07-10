/*
  This is a library written for the HM12/13 Bluetooth 4.0 module (with
  an eye towards expansion for support for HM10/11, HM16/17, HM18/19)
  SparkFun sells these at its website: www.sparkfun.com
  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/14840
  https://www.sparkfun.com/products/14839
  Written by Jim Lindblom @ SparkFun Electronics, July 3, 2018
  
  The HM13 modules are dual mode, Bluetooth 4.0 modules. They support
  both BLE and SPP profiles (SPP via enhanced data-rate [EDR]).
  
  This library handles iniitialization and configuration of the module
  as well as polling for connect/disconnect messages.

  It supports interfacing the module via either hardware or software serial
  or an I2C interface (using the Qwiic Bluetooth 4.0 module). Library is
  designed to support both the Qwiic Bluetooth and the Bluetooth Mate 4.0:
  https://github.com/sparkfunX/Bluetooth_Mate_4.0
  https://github.com/sparkfunX/Qwiic_Bluetooth_HC1x

  Development environment specifics:
  Arduino IDE 1.8.5
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef ARDUINO_AVR_UNO               // Arduino UNO
#define HM1X_SOFTWARE_SERIAL_ENABLED // Enable software serial
#define HM1X_HARDWARE_SERIAL_ENABLED // Enable hardware serial :/
#define HM1X_I2C_ENABLED
#endif

#ifdef ARDUINO_ARCH_SAMD              // SAMD21
#define HM1X_SOFTWARE_SERIAL_ENABLEDx // Disable software serial
#define HM1X_HARDWARE_SERIAL_ENABLED
#define HM1X_I2C_ENABLED
#endif

#ifdef HM1X_I2C_ENABLED
#include <Wire.h>
#endif
#ifdef HM1X_SOFTWARE_SERIAL_ENABLED
#include <SoftwareSerial.h>
#endif

#define QWIIC_BLUETOOTH_DEFAULT_ADDRESS 0x1B
#define QWIIC_BLUETOOTH_JUMPED_ADDRESS 0x1C

typedef enum {
    HM1X_OUT_OF_MEMORY       = -8,
    HM1X_RX_OVERFLOW         = -7,
    HM1X_UNEXPECTED_RESPONSE = -6,
    HM1X_ERROR_NO_CONNECTION = -5,
    HM1X_ERROR_TRY_LATER     = -4,
    HM1X_ERROR_READ_ERROR    = -3,
    HM1X_ERROR_TIMEOUT       = -2,
    HM1X_ERROR_ER            = -1,
    HM1X_SUCCESS             = 0
} HM1X_error_t;

class HM1X_BT : public Print {
public:
    
    typedef enum {
        HM10,
        HM11,
        HM12,
        HM13,
        HM14,
        HM15,
        HM16,
        HM17,
        HM18,
        HM19,
        NUM_HM_MODELS
    } HM1X_model_t;

    HM1X_BT(HM1X_model_t type = HM13);

    // Begin -- initialize BT module and ensure it's connected
#ifdef HM1X_SOFTWARE_SERIAL_ENABLED
    boolean begin(SoftwareSerial & softSerial, unsigned long baud = 9600);
#endif
#ifdef HM1X_HARDWARE_SERIAL_ENABLED
    boolean begin(HardwareSerial &serialPort = Serial, unsigned long baud = 9600);
#endif
#ifdef HM1X_I2C_ENABLED
    boolean begin(TwoWire &wirePort, uint8_t address);
#endif
    
    boolean connected(void) { return (_connectedBle || _connectedEdr);};
    boolean connectedEdr(void) { return _connectedEdr;};
    boolean connectedBle(void) { return _connectedBle;};

    boolean setupPoll(void);
    boolean poll(void);
    int available(void);
    char read(void);

    virtual size_t write(uint8_t c);
    virtual size_t write(const char *str);
    virtual size_t write(const char * buffer, size_t size);

    /* size_t send(String s); */

    // ---- AT commands -----

    // AT -- Test Command/ Disconnect command
    HM1X_error_t testOrDisconnect(void);
    HM1X_error_t disconnect(void) { return testOrDisconnect();};
    HM1X_error_t test(void) { return testOrDisconnect();};

    // AT+RENEW -- Restore factory defaults
    HM1X_error_t factoryDefaults(void);

    // AT+RESET -- Restart module
    HM1X_error_t reset(void);

    // AT+VERR -- Software version
    HM1X_error_t version(char * version);

    // AT+NOTI, AT+NOTP -- Notify information
    HM1X_error_t notifyInfo(boolean enabled = true);
    HM1X_error_t notifyMode(boolean enabled = true);
    HM1X_error_t notify(boolean enabled = true, boolean withAddress = true);

    // AT+NAME, AT+NAMB -- Set EDR/BLE name
    String getEdrName(void);
    HM1X_error_t getEdrName(char * name);
    HM1X_error_t setEdrName(String name);
    HM1X_error_t setEdrName(const char * name);
    String getBleName(void);
    HM1X_error_t getBleName(char * name);
    HM1X_error_t setBleName(String name);
    HM1X_error_t setBleName(const char * name);

    // AT+ADDE -- EDR address
    String edrAddress(void);
    HM1X_error_t edrAddress(char * retAddress);
    // AT+ADDB -- BLE address
    String bleAddress(void);
    HM1X_error_t bleAddress(char * retAddress);

    // AT+RADE, AT+RADB -- Last connected EDR/BLE address
    HM1X_error_t lastEdrAddress(char * address);
    HM1X_error_t lastBleAddress(char * address);

    // AT+BONDE, AT+BONDB --- Clear EDR/BLE bond info
    HM1X_error_t clearEdrBond(void);
    HM1X_error_t clearBleBond(void);

    // AT+CLEAE, AT+CLEAB -- Clear last connected EDR/BLE address
    HM1X_error_t clearEdrConnected(void);
    HM1X_error_t clearBleConnected(void);

    // AT+ROLE, AT+ROLB -- EDR/BLE mode
    typedef enum {
        EDR_SLAVE,
        EDR_MASTER,
        EDR_MODE_INVALID
    } HM1X_edr_mode_t;
    typedef enum {
        BLE_PERIPHERAL,
        BLE_CENTRAL,
        BLE_MODE_INVALID
    } HM1X_ble_mode_t;
    HM1X_error_t getEdrMode(HM1X_edr_mode_t * mode);
    HM1X_error_t setEdrMode(HM1X_edr_mode_t mode);
    HM1X_error_t getBleMode(HM1X_ble_mode_t * mode);
    HM1X_error_t setBleMode(HM1X_ble_mode_t mode);

    // AT+HIGH -- Data transmission speed mode
    // Disabled: SPP and BLE speeds balanced
    // Enabled: SPP will go high speed
    HM1X_error_t enableHighSpeedSPP(boolean enabled = true);

    // AT+DUAL -- Dual work mode
    // Enabled: SPP and BLE allowed. Disabled: Only one connection at a time.
    HM1X_error_t enableDualMode(boolean enabled = true);

    // AT+MODE -- Module work mode
    // Enabled: Allow remote control (send AT commands remotely)
    // Disabled: Data transmission only 
    HM1X_error_t enableRemoteControl(boolean enabled = true);

    // AT+ATOB -- A to B mode
    // When two modules connected (BLE and SPP), this will route data from one to the other
    HM1X_error_t enableAtoB(boolean enable = true);

    // AT+AUTH -- Authentication mode
    HM1X_error_t enableAuthenticationMode(boolean enable = true);

    // AT+PINE, AT+PINB -- EDR/BLE PIN Code
    HM1X_error_t getEdrPin(char * code);
    HM1X_error_t getBlePin(char * code);
    HM1X_error_t setEdrPin(char * code);
    HM1X_error_t setBlePin(char * code);

    // AT+COFD -- Class of device
    // Can set to any value between 0x000000 to 0xFFFFFE
    HM1X_error_t setCod(uint32_t cod);

    // AT+COUP -- Update connection parameter
    // Only usable in  BLE slave mode. Updates min/max interval, slave latency, and connection supervised timeout
    HM1X_error_t enableUpdateConnectionParameter(boolean enable = true);

    // AT+IBEA -- Enable iBeacon
    boolean iBeacon(boolean enable = true);
    HM1X_error_t enableiBeacon(boolean enabled = true);

    // AT+IBE0, AT+IBE1, AT+IBE2, AT+IBE3 -- Get/Set iBeacon UUID
    String getiBeaconUUID(void);
    HM1X_error_t getiBeaconUUID(char * uuid); 
    HM1X_error_t getiBeaconUUID(char * uuid, uint8_t position);
    HM1X_error_t setiBeaconUUID(char * first, char * second, char * third, char * fourth);
    HM1X_error_t setiBeaconUUID(char * uuid, uint8_t position);

    // AT+MAJO, AT+MINO -- iBeacon Major version
    /*HM1X_error_t getiBeaconVersions(uint16_t * major, uint16_t * minor);
    HM1X_error_t setiBeaconVersions(uint16_t major, uint16_t minor);*/
    HM1X_error_t getiBeaconMajor(uint16_t * version);
    HM1X_error_t setiBeaconMajor(uint16_t version);
    HM1X_error_t getiBeaconMinor(uint16_t * version);
    HM1X_error_t setiBeaconMinor(uint16_t version);

    // AT+MEAS -- iBeacon Measured Power
    HM1X_error_t getiBeaconPower(uint8_t * power);
    HM1X_error_t setiBeaconPower(uint8_t power);

    // AT+MTUS -- MTU Size
    typedef enum {
        MTU_SIZE_60,
        MTU_SIZE_120
    } HM1X_mtu_size_t;
    HM1X_error_t setMtuSize(HM1X_mtu_size_t mtuSize);

    // AT+SCAN -- EDR Advert type
    typedef enum {
        DISCOVERY_AND_CONNECTABLE,
        ONLY_CONNECTABLE,
        EDR_ADVERT_UNDEFINED
    } HM1X_edr_advert_t;
    HM1X_error_t getEdrAdvertType(HM1X_edr_advert_t * type);
    HM1X_error_t setEdrAdvertType(HM1X_edr_advert_t type);

    // AT+SAFE -- Module safe mode
    HM1X_error_t enableSafeMode(boolean enabled = true);

    // AT+ONEM -- Whether to use BLE MAC address
    // Note: If you want to use BLE in Android, don't use this command :S
    HM1X_error_t disableBleAddress(boolean disabled = true);

    // AT+PIO0 -- Enable system key function on PIO0
    HM1X_error_t enableSystemKey(boolean enabled = true);

    // AT+POIO1 -- System LED, PIO1 control
    typedef enum {
        BLINK_DISCONNECTED,
        HIGH_DISCONNECTED,
    } HM1X_led_mode_t;
    HM1X_error_t setLedMode(HM1X_led_mode_t mode);

    // AT+PIO -- Write/query PIO
    HM1X_error_t readPio(uint8_t pin, uint8_t * value);
    HM1X_error_t writePio(uint8_t pin, uint8_t value);

    // AT+BAUD -- Baud rate
    typedef enum {
        HM1X_BAUD_INVALID,
        HM1X_BAUD_4800,
        HM1X_BAUD_9600,
        HM1X_BAUD_19200,
        HM1X_BAUD_38400,
        HM1X_BAUD_57600,
        HM1X_BAUD_115200,
        HM1X_BAUD_230400,
        NUM_HM1X_BAUDS
    } HM1X_baud_t;
    HM1X_error_t setBaud(HM1X_baud_t atob);
    HM1X_error_t setBaud(uint32_t baud);

private:
    
    HM1X_model_t _btModel;

#ifdef HM1X_HARDWARE_SERIAL_ENABLED
    HardwareSerial * _serialPort;
#endif
#ifdef HM1X_SOFTWARE_SERIAL_ENABLED
    SoftwareSerial * _softSerial;
#endif
#ifdef HM1X_I2C_ENABLED
    TwoWire * _wirePort;
    uint8_t _wireAddress;
#endif

    boolean _connectedEdr;
    boolean _connectedBle;
    String _edrAddress;
    String _bleAddress;

    String _response;

    boolean _polling;

    HM1X_error_t init(void);

    // Send command with an expected response string/length -- e.g. "OK":
    HM1X_error_t sendCommandWithResponseAndTimeout(const char * command, char * expectedResponse, uint16_t commandTimeout);
    // Send a command wait for a timeout, check for response -- e.g. "OK" or "OK+LSTE:001122334455"
    int sendCommandWithTimeout(const char * command, char ** responseDest, uint16_t commandTimeout);

    // Send a command -- prepend AT
    boolean sendCommand(const char * command);

    /*void hwFlush(void); // Read and trash all bytes from serial buffer*/
    size_t hwPrint(const char * s);

    int readAvailable(char * inString);
    char readChar(void);
    int hwAvailable(void);
    
#ifdef HM1X_I2C_ENABLED
    void writeI2cBaud(uint8_t baudIndex);
    void setI2cAddress(uint8_t address);
#endif

    HM1X_error_t forceBaud(unsigned long baud);
    HM1X_error_t forceBaud(HM1X_baud_t baud);
};