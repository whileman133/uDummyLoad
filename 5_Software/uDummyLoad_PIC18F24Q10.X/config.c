/*
 * config.c
 */

#include "config.h"
#include "mcc_generated_files/mcc.h"
#include "timing.h"

// communication protocol constants
#define START_DURATION_MS 100
#define SYNC_TIMEOUT_MS 1000
#define ACK_DURATION_MS 100
#define RX_STOP_MS 100
#define RX_TIMEOUT_MS 1000

// data constants
#define DATA_MAX_BYTES 255
#define HEADER_SIZE 7               // number of bytes before calibration table data
#define CONFIG_TABLE_MAX_ROWS 60    // maximum number of calibration table rows allowed
#define TABLE_BYTES_PER_ROW 4       // number of bytes in each calibration table row

struct sDataBuffer {
    uint8_t length;
    uint8_t bytes[DATA_MAX_BYTES];
};

struct sConfigParams {
    // header
    enum eConfigPrecision precision;
    uint8_t diodeCompEnable;
    uint8_t calibrationEnable;
    uint8_t factoryTestEnable;
    uint16_t diodeOffsetMv;
    uint16_t displayPeriodMs;
    uint8_t samplingPeriodMs;
    uint8_t tableLen;
    // table data
    uint16_t table[CONFIG_TABLE_MAX_ROWS][2];
};

struct sConfig {
    struct sDataBuffer buf;
    struct sConfigParams params;
};
static struct sConfig gConfig;

static enum eConfigError _ReadBytesEEPROM(struct sDataBuffer *buf);
static enum eConfigError _WriteBytesEEPROM(struct sDataBuffer *buf);
static enum eConfigError _ReceiveBytes(struct sDataBuffer *buf);
static enum eConfigError _TransmitBytes(struct sDataBuffer *buf);
static uint8_t _WaitForLinesHigh(uint32_t timeoutMs);
static uint8_t _WaitForLines(uint8_t state, uint32_t period, uint32_t timeoutMs);
static enum eConfigError _ParseConfigParams(struct sDataBuffer *buf, struct sConfigParams *params);

void Config_Init(void) {
    // initialize the CLK/DAT pins for external data input
    CLK_SetOpenDrain();
    DAT_SetOpenDrain();
    CLK_SetPullup();
    DAT_SetPullup();
    CLK_SetHigh();
    DAT_SetHigh();
    CLK_SetDigitalOutput();
    DAT_SetDigitalOutput();
} 

enum eConfigError Config_Load(void) {
    enum eConfigError error;
    uint8_t externalInput = 1;
    
    // check for the condition signaling external input, i.e. a LOW on both
    // the DAT and CLK lines existing for START_DURATION_MS
    for (uint32_t mark = Timing_Milliseconds(); Timing_Milliseconds() - mark < START_DURATION_MS;) {
        if (CLK_GetValue() != LOW || DAT_GetValue() != LOW) {
            externalInput = 0;
            break;
        } // if
    } // for
    
    if (externalInput) {        
        // synchronize with external MCU by waiting for the external MCU 
        // to allow DAT and CLK lines to float HIGH
        if (!_WaitForLinesHigh(SYNC_TIMEOUT_MS)) {
            return CONFIG_ERROR_COMMUNICATION_SYNC;
        } // if
        
        // pull CLK/DAT LOW to acknowledge 
        // external MCU will start clocking in data once CLK/DAT go back HIGH
        CLK_SetLow();
        DAT_SetLow();
        DELAY_milliseconds(ACK_DURATION_MS);
        CLK_SetHigh();
        DAT_SetHigh();
        
        // read in data from CLK/DAT
        error = _ReceiveBytes(&gConfig.buf);
        if (error != CONFIG_ERROR_NONE)
            return error;
        
        // extract configuration parameters from received bytes
        error = _ParseConfigParams(&gConfig.buf, &gConfig.params);
        if (error != CONFIG_ERROR_NONE)
            return error;
        
        // echo received bytes back to external MCU
        // (serves as a basic error check - MCU may reset and reload on error)
        // wait for MCU to release the DAT/CLK lines
        if (!_WaitForLinesHigh(SYNC_TIMEOUT_MS)) {
            return CONFIG_ERROR_COMMUNICATION_SYNC;
        } // if
        // pull CLK/DAT LOW to acknowledge data reception / signal echo
        CLK_SetLow();
        DAT_SetLow();
        DELAY_milliseconds(ACK_DURATION_MS);
        // transmit bytes
        error = _TransmitBytes(&gConfig.buf);
        if (error != CONFIG_ERROR_NONE)
            return error;
        // release lines
        CLK_SetHigh();
        DAT_SetHigh();
        
        // wait for MCU to acknowledge
        if (!_WaitForLines(LOW, ACK_DURATION_MS, SYNC_TIMEOUT_MS)) {
            return CONFIG_ERROR_COMMUNICATION_SYNC;
        } // if
        
        // store received bytes into EEPROM for use on next boot
        _WriteBytesEEPROM(&gConfig.buf);
    }
    else {
        // load bytes from EEPROM
        error = _ReadBytesEEPROM(&gConfig.buf);
        if (error != CONFIG_ERROR_NONE)
            return error;
        
        // extract configuration parameters from loaded bytes
        error = _ParseConfigParams(&gConfig.buf, &gConfig.params);
        if (error != CONFIG_ERROR_NONE)
            return error;
    }
    
    return CONFIG_ERROR_NONE;
}

enum eConfigPrecision Config_GetPrecision(void) {
    return gConfig.params.precision;
}

uint8_t Config_IsDiodeCompEnabled(void) {
    return gConfig.params.diodeCompEnable;
}

uint8_t Config_IsCalibrationEnabled(void) {
    return gConfig.params.calibrationEnable;
}

uint8_t Config_IsFactoryTestEnabled(void) {
    return gConfig.params.factoryTestEnable;
}

uint16_t Config_GetDiodeOffsetMv(void) {
    return gConfig.params.diodeOffsetMv;
}

uint16_t Config_GetDisplayPeriodMs(void) {
    return gConfig.params.displayPeriodMs;
}

uint8_t Config_GetSamplingPeriodMs(void) {
    return gConfig.params.samplingPeriodMs;
}

uint16_t Config_TableLookup(uint16_t powerMilliwatts) {
    uint8_t length = gConfig.params.tableLen;
    uint8_t rowIndex;
    uint32_t x1, x2, y1, y2;    // ! use 32-bit variables to prevent overflow
    
    if (length < 2 || powerMilliwatts < gConfig.params.table[0][0] || powerMilliwatts > gConfig.params.table[length - 1][0]) {
        // table not present or input value out of table range
        return powerMilliwatts;
    } // if
    
    // locate the rows between which the input value falls
    for (rowIndex = 0; rowIndex < length; ++rowIndex) {
        if (powerMilliwatts == gConfig.params.table[rowIndex][0]) {
            // interpolation not required; perform direct lookup
            return gConfig.params.table[rowIndex][1];
        } // if
        if (powerMilliwatts > gConfig.params.table[rowIndex][0]) {
            break;
        } // if
    } // for
    
    // perform linear interpolation
    x1 = gConfig.params.table[rowIndex][0];
    x2 = gConfig.params.table[rowIndex + 1][0];
    y1 = gConfig.params.table[rowIndex][1];
    y2 = gConfig.params.table[rowIndex + 1][1];
    return (uint16_t)(y1 + (powerMilliwatts - x1) * (y2 - y1) / (x2 - x1));
}

static enum eConfigError _ReadBytesEEPROM(struct sDataBuffer *buf) {
    // fetch data length
    buf->length = DATAEE_ReadByte(0);
    
    for (uint8_t i = 0; i < buf->length; ++i) {
        buf->bytes[i] = DATAEE_ReadByte(i + 1);
    }
    
    return CONFIG_ERROR_NONE;
}

static enum eConfigError _WriteBytesEEPROM(struct sDataBuffer *buf) {
    // store data length
    DATAEE_WriteByte(0, buf->length);
    
    for (uint8_t i = 0; i < buf->length; ++i) {
        DATAEE_WriteByte(i + 1, buf->bytes[i]);
    }
    
    return CONFIG_ERROR_NONE;
}

static enum eConfigError _ReceiveBytes(struct sDataBuffer *buf) {
    enum eConfigError error = CONFIG_ERROR_NONE;
    
    int8_t prevClk = -1, clk = -1, dat = -1;
    uint8_t byte = 0;       // bit accumulator
    uint8_t bitIndex = 0;   // index into bit accumulator
    uint8_t byteIndex = 0;  // index into byte buffer
    uint32_t now, rxMark;   // timing variables for detecting end of transmission
    
    rxMark = Timing_Milliseconds();
    
    while (1) {
        now = Timing_Milliseconds();
        clk = CLK_GetValue();
        dat = DAT_GetValue();
        
        // store bits on rising clock edges (LSBs transmitted first)
        if (prevClk != -1 && prevClk == LOW && clk == HIGH) {
            byte |= dat << bitIndex;
            ++bitIndex;
        } // if
        
        // store byte once 8-bits received
        if (bitIndex >= 8) {
            // check for buffer overflow
            if (byteIndex >= DATA_MAX_BYTES) {
                error = CONFIG_ERROR_BUFFER_OVERFLOW;
                break;
            } // if

            // store byte
            buf->bytes[byteIndex] = byte;
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
        if (byteIndex > 0 && now - rxMark > RX_STOP_MS) {
            break;
        } // if
        
        // timeout if we haven't received any bytes
        if (byteIndex == 0 && now - rxMark > RX_TIMEOUT_MS) {
            return CONFIG_ERROR_RECEIVE_TIMEOUT;
        } // if
        
        prevClk = clk;
    } // while
    
    buf->length = byteIndex;
    return error;
}

static enum eConfigError _TransmitBytes(struct sDataBuffer *buf) {
    for (size_t i = 0; i < buf->length; ++i) {
        // transmit LSb first
        for (uint8_t k = 0; k < 8; ++k) {
            CLK_SetLow();
            if (buf->bytes[i] & (1 << k))
                DAT_SetHigh();
            else
                DAT_SetLow();
            DELAY_microseconds(1000);
            CLK_SetHigh();
            DELAY_microseconds(1000);
        } // for
    } // for
    
    return CONFIG_ERROR_NONE;
}

static uint8_t _WaitForLinesHigh(uint32_t timeoutMs) {
    uint8_t areHigh = 0;
    
    for (uint32_t mark = Timing_Milliseconds(); Timing_Milliseconds() - mark < timeoutMs;) {
        if (CLK_GetValue() == HIGH && DAT_GetValue() == HIGH) {
            areHigh = 1;
            break;
        } // if
    } // for
    
    return areHigh;
}

static uint8_t _WaitForLines(uint8_t state, uint32_t period, uint32_t timeoutMs) {
    uint8_t singleDetect = 0, pulseDetect = 0;
    uint32_t now, mark, singleMark;
    
    mark = Timing_Milliseconds();
    
    while (1) {
        now = Timing_Milliseconds();
        
        // check timeout
        if (now - mark >= timeoutMs) {
            break;
        }
        
        // check for lines at the specified state at this single instant in time
        if (CLK_GetValue() == state && DAT_GetValue() == state) {
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

static enum eConfigError _ParseConfigParams(struct sDataBuffer *buf, struct sConfigParams *params) {
    // Header ------------------------------------------------------------------
    if (buf->length < HEADER_SIZE)
        return CONFIG_ERROR_HEADER_MISSING;
    
    params->precision = (buf->bytes[0] & 1) == 0 ? CONFIG_PRECISION_STANDARD : CONFIG_PRECISION_EXTENDED;
    params->diodeCompEnable = (buf->bytes[0] & 2) ? 1 : 0;
    params->calibrationEnable = (buf->bytes[0] & 4) ? 1 : 0;
    params->factoryTestEnable = (buf->bytes[0] & 8) ? 1 : 0;
    params->diodeOffsetMv = ((uint16_t)buf->bytes[2] << 8) | (uint16_t)buf->bytes[1];
    params->displayPeriodMs = ((uint16_t)buf->bytes[4] << 8) | (uint16_t)buf->bytes[3];
    params->samplingPeriodMs = buf->bytes[5];
    params->tableLen = buf->bytes[6];
    
    // Calibration Table -------------------------------------------------------
    // check that the table is the right size
    if (params->tableLen == 1 || params->tableLen * TABLE_BYTES_PER_ROW != (buf->length - HEADER_SIZE))
        return CONFIG_ERROR_TABLE_MALFORMED;
    
    for (size_t rowIdx = 0, i = HEADER_SIZE; rowIdx < params->tableLen; ++rowIdx, i += TABLE_BYTES_PER_ROW) {
        params->table[rowIdx][0] = ((uint16_t)buf->bytes[i+1] << 8) | (uint16_t)buf->bytes[i];
        params->table[rowIdx][1] = ((uint16_t)buf->bytes[i+3] << 8) | (uint16_t)buf->bytes[i+2];
    } // for
    
    // verify the table data is sorted in ascending order if loaded
    if (params->tableLen > 0) {
        for (size_t rowIdx = 0; rowIdx < params->tableLen - 1; ++rowIdx) {
            if (params->table[rowIdx][0] > params->table[rowIdx + 1][0]) {
                return CONFIG_ERROR_TABLE_NOT_SORTED;
            } // if
        } // for   
    } // if
    
    return CONFIG_ERROR_NONE;
}
