/*
 * uDummyLoadConfig.ino
 * ============================
 * ASMV Electrical
 * Last Updated: 04.18.2021
 * 
 * Writes configuration/calibration data to the uDummyLoad device. 
 * Requires three digital pins in addition to +5V and GND connections. 
 * Works with both 5V and 3.3V Arduino boards (change VDD accordingly.)
 * 
 * 
 * CONFIGURATION PROCEDURE
 * 
 * 1. Connect the uDummyLoad's 5-pin header to an Arduino board as
 *    shown below. Change the pin numbers in the `#define` statements
 *    to match the pins you use.
 * 
 *                                             +---------------------
 *                                             |
 *                                             |
 *                       FIVE PIN INTERFACE    |
 * +-----------------+                         |
 * |                 |=CLK <---------> PIN_CLK=|  Arduino
 * |                 |=DAT <---------> PIN_DAT=|  Microcontroller
 * |   uDummyLoad    |=GND <-------------- GND=|  Board
 * |                 |=VDD <-------------- +5V=|  (Uno, Nano, etc)
 * |                 |=RST*<---------- PIN_RST=| 
 * +-----------------+                         |
 *                                             |
 *   *Reset pin identified by square pad       |
 *                                             |
 *                                             +---------------------
 *                                             
 *                                             
 * WARNING: The uDummyLoad VDD and GND pins are not reverse-polarity
 * protected. Take care to connect them with the correct polarity.
 */
#define PIN_DAT 4
#define PIN_CLK 5
#define PIN_RST 6      
                                     
/*
 * 2. Confirm the serial settings below. Change the bitrate if needed. 
 *    If your Arduino board has multiple serial ports, replace `Serial` 
 *    with the port you are using. The serial console is used for 
 *    outputting status messages.
 */
#define SERIAL_BAUD 115200          // or 9600, 19200, 38400, etc
HardwareSerial *serial = &Serial;

/*
 * 3. Set the uDummyLoad configuration settings below as desired.
 *    
 *    Setting                    | Description
 *    -------------------------- | -----------------------------------------------------
 *    precision                  | Select 0.1W or 0.001W meter precision. The display 
 *                               |   scans between digits in the 0.001W mode.
 *    enableCalibration          | Enable calibration mode to bypass table lookup so that
 *                               |   calibration readings can be taken.
 *    enableDiodeComp            | Compensate the diode voltage drop in the peak voltage
 *                               |   reading by measuring the drop across a matched diode
 *                               |   (`true` enables the compensation; `false` disables)
 *    enableFactoryTest          | When true, the uDummyLoad runs the factory test on
 *                               |   startup, which consists of scanning through all LCD
 *                               |   digits among other operations.
 *    diodeOffsetMillivolts      | Fixed voltage to add to the peak voltage reading to
 *                               |   account for the difference in current flowing in
 *                               |   the compensation diode vs the peak detector diode
 *                               |   (fixed diode voltage drop when compensation disabled)
 *    displayUpdateMilliseconds  | Display update period in milliseconds 
 *    samplingPeriodMilliseconds | A/D sampling period in milliseconds
 *    table                      | Calibration table of up to 60 points for the wattmeter.
 *                               |   The uDummyLoad linearly interpolates between points
 *                               |   when looking up power readings.
 */
enum ePrecision {
    PRECISION_STANDARD = 0, // 0.1W
    PRECISION_EXTENDED = 1, // 0.001W
} precision = PRECISION_STANDARD;         // <= SELECT PRECISION
bool enableCalibration = false;           // <= ENABLE/DISABLE CALIBRATION MODE
bool enableDiodeComp = true;              // <= ENABLE/DISABLE DIODE COMPENSATION
bool enableFactoryTest = false;           // <= ENABLE/DISABLE FACTORY TEST MODE
uint16_t diodeOffsetMillivolts = 17;      // <= ENTER DIODE OFFSET (in mV)
uint16_t displayUpdateMilliseconds = 100; // <= ENTER DISPLAY UPDATE TIME (in msec)
uint8_t samplingPeriodMilliseconds = 20;  // <= ENTER SAMPLING PERIOD (in msec)
uint16_t table[61][2] = {
    // INSERT CALIBRATION DATA BELOW
    // Row format: (meter reading in milliwatts), (corrected value in milliwatts)
    //1234,       1234,

    // END OF TABLE
    0xFF,       0xFF,   // do not modify, marks end of table
};

/*
 * 4. Save and upload this sketch to your Arduino board using the 
 *    Arduino IDE.
 *    
 * 5. Open the Serial Monitor from the Arduino IDE and confirm that
 *    the correct uDummyLoad configuration options are displayed 
 *    on the screen. If any settings are incorrect, change the 
 *    options above and re-upload the sketch to your Arduino.
 *    
 * 6. Press enter on your keyboard or click the Send button in the 
 *    Serial Monitor to send configuration data to the uDummyLoad.
 *    
 * 7. Verify the configuration upload was successful: The serial
 *    output should display a "SUCCESS" message and the uDummyLoad
 *    should display 0.0W.
 *    
 * Troubleshooting: 
 * - If you receive garbled data over the serial port, ensure the baud
 *   rate setting in the Serial Monitor matches with step 2.
 * - If the uDummyLoad not flashes an "Ec" (error code) message or 
 *   this sketch outputs am error message, check your wiring and reset 
 *   your Arduino board to try again. Error code descriptions for
 *   the uDummyLoad follow:
 *   
 *   ﻿01  Timeout waiting for master (Arduino) to release DAT/CLK lines.
 *   02  Timeout waiting for master (Arduino) to send data.
 *   03  Receive buffer overflowed 255 bytes.
 *   04  Data header missing or malformed.
 *   05  Calibration table malformed.
 *   06  Calibration table not sorted in ascending order.

 *   
 * END CONFIGURATION PROCEDURE
 * DO NOT MODIFY CODE BELOW THIS LINE
 */

#define DATA_MAX_BYTES 255      // maximum number of bytes transmitted or received
#define TIMEOUT_MS 1000         // length of time after which most communication tasks fail with a timeout error
#define RESET_MS 100            // length of time to hold RST LOW to reset the uDummyLoad
#define TX_START_MS 200         // length of time to hold DAT/CLK LOW to initiate data transmission to uDummyLoad
#define RX_STOP_MS 100          // length of time without CLK activity signaling end of received data from uDummyLoad
#define TX_ACK_MS 100           // pulse duration for sending ACK to uDummyLoad
#define RX_ACK_MS 50            // expected ACK pulse duration when receiving from uDummyLoad
#define LINES_RELEASE_MS 10     // amount of time DAT/CLK must remain HIGH to be considered released

// configuration byte buffers
size_t bufLen = 0, rxLen = 0, tableLen;
uint8_t buf[DATA_MAX_BYTES] = {0};      // transmit buffer
uint8_t rxBuf[DATA_MAX_BYTES] = {0};    // read-back buffer

enum eError {
    ERROR_NONE = 0,
    ERROR_BUFFER_OVERFLOW = 1,
    ERROR_TIMEOUT = 2,
    ERROR_DATA_MISMATCH,
};

void setup() {
    char stringBuf[50];

    // compile configuration bytes
    PrepareConfigBytes(buf, &bufLen, &tableLen);

    serial->begin(SERIAL_BAUD);
    serial->println();
    serial->println(F("The following is to be transmitted to the uDummyLoad:"));
    serial->print(F("   Precision             : "));
    serial->println(precision == PRECISION_STANDARD ? F("standard (0.1W)") : F("extended (0.001W)"));
    serial->print(F("   Calibration Mode      : "));
    serial->println(enableCalibration ? F("enabled") : F("disabled"));
    serial->print(F("   Diode Compensation    : "));
    serial->println(enableDiodeComp ? F("enabled") : F("disabled"));
    serial->print(F("   Diode Offset Voltage  : "));
    serial->print(diodeOffsetMillivolts);
    serial->println(" mV");
    serial->print(F("   Display Update Period : "));
    serial->print(displayUpdateMilliseconds);
    serial->println(" ms");
    serial->print(F("   Sampling Period       : "));
    serial->print(samplingPeriodMilliseconds);
    serial->println(" ms");
    serial->println(F("   Calibration Table     : Meter Reading (mV) | Corrected Value (mV)"));
    serial->println(F("                           ------------------ | --------------------"));
    for (size_t i = 0; i < tableLen; ++i) {
        sprintf(stringBuf, "%18d | %20d", table[i][0], table[i][1]);
        Serial.print(F("                           "));
        Serial.println(stringBuf);
    } // for
    serial->print(F("                           ("));
    serial->print(tableLen);
    serial->println(F(" rows)"));
    serial->println(F("SEND ANY KEY TO PROCEED"));

    // wait for user input
    while(serial->available() == 0);

    serial->print(F("Transmitting: "));
    for (size_t i = 0; i < bufLen; ++i) {
        serial->print("0x");
        serial->print(buf[i], HEX);
        serial->print(" ");
    } // for
    serial->println();

    // transmit bytes
    enum eError error = WriteConfig(buf, bufLen, rxBuf, &rxLen);

    if (rxLen > 0) {
        serial->print(F("Read-Back   : "));
        for (size_t i = 0; i < rxLen; ++i) {
            serial->print("0x");
            serial->print(rxBuf[i], HEX);
            serial->print(" ");
        } // for
        serial->println(); 
    } // if

    switch (error) {
        case ERROR_NONE:
            serial->println(F("SUCCESS writing configuration data"));
            break;
            
        case ERROR_BUFFER_OVERFLOW:
            serial->println(F("ERROR: Receive buffer overflow"));
            break;
            
        case ERROR_TIMEOUT:
            serial->println(F("ERROR: Communication timeout; uDummyLoad failed to respond"));
            break;
            
        case ERROR_DATA_MISMATCH:
            serial->println(F("ERROR: Read-back data does not match transmitted data"));
            break;
    } // switch

    if (error != ERROR_NONE) {
        serial->println(F("Please check your wiring."));
    } // if
}

void loop() {
}

/*
 * Complile configuration settings into a byte array for transmission to the
 * uDummyLoad device.
 */
void PrepareConfigBytes(uint8_t *buf, size_t *bufLen, size_t *tableLen) {    
    *bufLen = 0;
    *tableLen = 0;
    
    // precision, diode comp, calibration mode, factory test
    buf[(*bufLen)++] =  (uint8_t)precision | 
                        (enableDiodeComp ? 2 : 0) | 
                        (enableCalibration ? 4 : 0) | 
                        (enableFactoryTest ? 8 : 0);
    // diode offset
    buf[(*bufLen)++] = (uint8_t)(diodeOffsetMillivolts & 0xFF);
    buf[(*bufLen)++] = (uint8_t)(diodeOffsetMillivolts >> 8);
    // display update time
    buf[(*bufLen)++] = (uint8_t)(displayUpdateMilliseconds & 0xFF);
    buf[(*bufLen)++] = (uint8_t)(displayUpdateMilliseconds >> 8);
    // sampling period
    buf[(*bufLen)++] = (uint8_t)(samplingPeriodMilliseconds & 0xFF);
    // calibration table
    uint8_t i;
    for (i = 0; table[i][0] != 0xFF && table[i][1] != 0xFF; ++i);   // determine table length
    buf[(*bufLen)++] = *tableLen = i;
    for (i = 0; i < *tableLen; ++i) {
        buf[(*bufLen)++] = (uint8_t)(table[i][0] & 0xFF);
        buf[(*bufLen)++] = (uint8_t)(table[i][0] >> 8);
        buf[(*bufLen)++] = (uint8_t)(table[i][1] & 0xFF);
        buf[(*bufLen)++] = (uint8_t)(table[i][1] >> 8);
    } // for
}

/*
 * Write configuration data to the uDummyLoad device and verify the correct data was 
 * received by load back.
 */
enum eError WriteConfig(uint8_t *txBuf, size_t len, uint8_t *rxBuf, size_t *rxLen) {    
    // Bootloader Initiation -------------------------------------------------------
    // reset the uDummyLoad device
    pinMode(PIN_DAT, OUTPUT);
    pinMode(PIN_CLK, OUTPUT);
    pinMode(PIN_RST, OUTPUT);
    digitalWrite(PIN_DAT, LOW);
    digitalWrite(PIN_CLK, LOW);
    digitalWrite(PIN_RST, LOW);
    delay(RESET_MS);
    // hold DAT/CLK LOW after reset to start the data transfer
    digitalWrite(PIN_RST, HIGH);
    delay(TX_START_MS);

    // uDummyLoad ACK -------------------------------------------------------------
    // wait for acknowledge signal from uDummyLoad (LOW on both DAT/CLK)
    pinMode(PIN_DAT, INPUT);
    pinMode(PIN_CLK, INPUT);
    if (!WaitForLines(LOW, RX_ACK_MS, TIMEOUT_MS)) {
        return ERROR_TIMEOUT;
    }
    // wait for uDummy load device to release the pins
    if (!WaitForLines(HIGH, LINES_RELEASE_MS, TIMEOUT_MS)) {
        return ERROR_TIMEOUT;
    }

    // Data Transmission ----------------------------------------------------------
    // clock out the data to the uDummyLoad
    pinMode(PIN_DAT, OUTPUT);
    pinMode(PIN_CLK, OUTPUT);
    WriteBytes(txBuf, len);

    // uDummyLoad ACK -------------------------------------------------------------
    // wait for acknowledge signal from uDummyLoad (LOW on both DAT/CLK)
    pinMode(PIN_DAT, INPUT);
    pinMode(PIN_CLK, INPUT);
    if (!WaitForLines(LOW, RX_ACK_MS, TIMEOUT_MS)) {
        return ERROR_TIMEOUT;
    }
    
    // Data Load-Back / Echo ------------------------------------------------------
    // receive bytes echoed by the uDummyLoad device
    enum eError rxError = ReceiveBytes(rxBuf, rxLen, DATA_MAX_BYTES, RX_STOP_MS, TIMEOUT_MS);
    if (rxError != ERROR_NONE) {
        return rxError;
    } // if
    // check that the echoed bytes are the same as those previously transmitted
    if (*rxLen != len || !BytesEqual(txBuf, rxBuf, len)) {
        return ERROR_DATA_MISMATCH;
    } // if

    // Master ACK -----------------------------------------------------------------
    // send acknowledge signal to uDummyLoad to commit the data to EEPROM
    pinMode(PIN_DAT, OUTPUT);
    pinMode(PIN_CLK, OUTPUT);
    digitalWrite(PIN_DAT, LOW);
    digitalWrite(PIN_CLK, LOW);
    delay(TX_ACK_MS);

    return ERROR_NONE;
}

/*
 * Transmit data bytes tp the uDummyLoad device. Software "bit banging" approach.
 * Least significant byte first; least significant bit first.
 * 
 * bytes - address of byte array to transmit
 * len - number of bytes to transmit from the array
 */
void WriteBytes(uint8_t *bytes, size_t len) {
    for (size_t k = 0; k < len; ++k) {
        for (uint8_t i = 0; i < 8; ++i) {
            digitalWrite(PIN_CLK, LOW);
            digitalWrite(PIN_DAT, bytes[k] & (1 << i) ? HIGH : LOW);
            delayMicroseconds(1000);
            digitalWrite(PIN_CLK, HIGH);
            delayMicroseconds(1000);
        } // for
    } // for
}

/*
 * Receive data bytes from the uDummyLoad device. Software "bit banging" approach.
 * Least significant byte first; least significant bit first.
 * 
 * buf - address of byte buffer in which to store received bytes
 * len - pointer to length variable where received byte count will be stored
 * maxLen - maximum length of the buffer (ERROR_BUFFER_OVERFLOW returned on overflow)
 * stopMs - amount of time required without activity on CLK line to signal STOP
 * timeoutMs - amount of time to wait to receive first byte before returning ERROR_TIMEOUT
 */
enum eError ReceiveBytes(uint8_t *buf, size_t *len, size_t maxLen, uint32_t stopMs, uint32_t timeoutMs) {
    enum eError error = ERROR_NONE;
    
    int8_t prevClk = -1, clk = -1, dat = -1;
    uint8_t byte = 0;       // bit accumulator
    uint8_t bitIndex = 0;   // index into bit accumulator
    uint8_t byteIndex = 0;  // index into byte buffer
    uint32_t now, rxMark;   // timing variables for detecting end of transmission (STOP signal)
    
    rxMark = millis();
    
    while (1) {
        now = millis();
        clk = digitalRead(PIN_CLK);
        dat = digitalRead(PIN_DAT);
        
        // store bits on rising clock edges (LSBs transmitted first)
        if (prevClk != -1 && prevClk == LOW && clk == HIGH) {
            byte |= dat << bitIndex;
            ++bitIndex;
        } // if
        
        // store byte once 8-bits received
        if (bitIndex >= 8) {
            // check for buffer overflow
            if (byteIndex >= maxLen) {
                error = ERROR_BUFFER_OVERFLOW;
                break;
            } // if

            // store byte
            buf[byteIndex] = byte;
            ++byteIndex;
            
            // clear the bit accumulator for next iteration
            byte = 0;                       
            bitIndex = 0;
        } // if
        
        // update receive time mark on clock edges
        if (prevClk != clk) {
            rxMark = now;
        } // if
        
        // stop receiving when clock level has not changed for 
        // the stop duration and we've received at least one byte
        if (byteIndex > 0 && now - rxMark > stopMs) {
            break;
        } // if
        
        // timeout if we haven't received any bytes
        if (byteIndex == 0 && now - rxMark > timeoutMs) {
            error = ERROR_TIMEOUT;
            break;
        } // if
        
        prevClk = clk;
    } // while
    
    *len = byteIndex;
    return error;
}

/*
 * Wait for the DAT/CLK lines to remain in the specified state for the specified
 * period. Configurable timeout.
 * 
 * state - the state of the lines to detect (HIGH or LOW)
 * period - the length of time the lines must remain at the specified state
 *     to constitute a positive detection
 * timeoutMs - maximum amount of time this function is allowed to hang waiting
 *     for a positive detection
 * 
 * Returns: 1 on positive detection; 0 on negative detection or timeout
 */
uint8_t WaitForLines(uint8_t state, uint32_t period, uint32_t timeoutMs) {
    uint8_t singleDetect = 0, pulseDetect = 0;
    uint32_t now, mark, singleMark;
    
    mark = singleMark = millis();
    
    while (1) {
        now = millis();
        
        // check timeout
        if (now - mark >= timeoutMs) {
            break;
        }
        
        // check for lines at the specified state at this single instant in time
        if (digitalRead(PIN_CLK) == state && digitalRead(PIN_DAT) == state) {
            if (!singleDetect) {
                singleDetect = 1;
                singleMark = now;
            } // if
        }
        else {
            singleDetect = 0;
        }
        
        // check for lines continuously at the specified state for the specified period
        if (singleDetect && now - singleMark >= period) {
            pulseDetect = 1;
            break;
        }
    } // while
    
    return pulseDetect;
}

/* 
 * Check equality of two byte arrays.
 * 
 * buf1 - address of first byte array
 * buf2 - address of second byte array
 * len - length of the arrays
 * 
 * Returns: true when all array elements are equal; false otherwise
 */
bool BytesEqual(uint8_t *buf1, uint8_t *buf2, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (buf1[i] != buf2[i])
            return false;
    } // for

    return true;
}
