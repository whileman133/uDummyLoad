/*
 * 20W/50-Ohm micro-Dummy Load with Wattmeter
 * --------------------------------------
 * ASMV Electrical
 * Target: PIC18F24Q10
 * Last Updated: 04.12.2021
 * 
 * Periodically samples the divided envelope voltage, computes the power
 * dissipated by the 50-ohm load, and displays the result on the numeric
 * segmented LCD.
 * 
 * Abbreviations
 *  FVR = Fixed Voltage Reference
 *  A/D = Analog to digital
 *  DAC = Digital to Analog Converter
 *  LSb = Least-Significant bits (the unit of measure of A/D output codes)
 *  VDD = microcontroller positive supply voltage
 */

#include <stdint.h>
#include "mcc_generated_files/mcc.h"
#include <math.h>
#include "lcd.h"
#include "timing.h"
#include "pwrman.h"
#include "config.h"

// circular buffer length - sets the length of the peak voltage moving average
#define CIRCBUF_LENGTH 10

// digit scan time for 0.001W extended precision mode
#define DIGIT_SCAN_PERIOD_MS 1000

// default configuration data
// byte 0: number of configuration bytes following
// byte 1: precision(bit0), diodeCompEnable(bit1), calibrationEnable(bit2), factoryTestEnable(bit3)
// byte 2: diodeOffsetMv (LSB) 
// byte 3: diodeOffsetMv (MSB) 
// byte 4: displayPeriodMs (LSB)
// byte 5: displayPeriodMs (MSB)
// byte 6: samplingPeriodMs
// byte 7: tableLength (0 indicates table not loaded)
// additional 4*tableLength bytes for table data
__EEPROM_DATA(0x07, 0x02, 0x11, 0x00, 0x64, 0x00, 0x32, 0x00);

static void uDummyLoad_Initialize(void);
static void uDummyLoad_FactoryTest(void);
static uint32_t uDummyLoad_GetVpeakMillivolts(void);

// circular buffer implementation for averaging peak voltage readings
struct sCircBuf {
    uint8_t head;                       // index where next item is to be placed
    uint8_t runon;                      // index used to avoid run-on effect after startup
    uint32_t sum;                       // moving sum of buffer values
    uint32_t values[CIRCBUF_LENGTH];    // buffer values
};
// initialize a buffer
static void CircBuf_Init(struct sCircBuf *buf);
// place an item into the buffer
static void CircBuf_Store(struct sCircBuf *buf, uint32_t value);
// calculate the average of the buffered values
static uint32_t CircBuf_GetAvg(struct sCircBuf *buf);

void main(void) {
    uDummyLoad_Initialize();
    
    // run the factory test
    if (Config_IsFactoryTestEnabled())
        uDummyLoad_FactoryTest();
    
    struct sCircBuf vpeakBuf;
    uint32_t vpeakMillivolts, powerMilliwatts, powerDeciwatts;
    uint32_t now, sampleMark, displayMark, scanMark;
    uint8_t scanLower = 0;
    
    CircBuf_Init(&vpeakBuf);
    sampleMark = displayMark = scanMark = Timing_Milliseconds();
    
    while (1) {
        now = Timing_Milliseconds();
        
        // Peak Voltage Sampling -----------------------------------------------
        // periodically convert Vdivider and Vdiode, calculate the peak 
        // input voltage, and store the result into circular buffer
        if (now - sampleMark >= Config_GetSamplingPeriodMs()) {
            sampleMark = now;
            CircBuf_Store(&vpeakBuf, uDummyLoad_GetVpeakMillivolts());
        } // if
        
        // Display Update ------------------------------------------------------
        // periodically update the display with current power
        if (now - displayMark >= Config_GetDisplayPeriodMs()) {
            displayMark = now;
            
            // compute the average power absorbed by the load resistor as 
            //  P = Vpk^2/2*R
            // where 
            //  Vpk = averaged peak input voltage
            //  R = 50Ohms is the load resistance
            // The maximum error in the fixed-point computation is 2mW as 
            // determined by exhaustive numerical analysis.
            vpeakMillivolts = CircBuf_GetAvg(&vpeakBuf);
            powerMilliwatts = vpeakMillivolts * vpeakMillivolts / 100000;
            
            // apply calibration if we're not in calibration mode
            if (!Config_IsCalibrationEnabled())
                powerMilliwatts = Config_TableLookup((uint16_t)powerMilliwatts);
            
            powerDeciwatts = powerMilliwatts / 100; // integer division rounds down
            if (powerMilliwatts % 100 >= 50)
                // round up if remainder greater than or equal to 0.5dW
                ++powerDeciwatts;
            
            // display power reading as appropriate
            switch (Config_GetPrecision()) {
                // 0.1W precision
                case CONFIG_PRECISION_STANDARD:
                    LCD_Print((uint16_t)powerDeciwatts, 1);
                    break;
                
                // 0.001W precision
                case CONFIG_PRECISION_EXTENDED:
                    if (scanLower)
                        LCD_Print((uint16_t)powerMilliwatts % 100, 0);
                    else
                        LCD_Print((uint16_t)powerMilliwatts / 100, 1);
                    break;
            } // switch
        } // if
        
        // Digit Scan Tracking (Extended Precision Mode) -----------------------
        // - periodically toggle the `scanLower` variable which controls
        //   the display of the lower two milliwatt digits
        // - used in extended precision mode (0.001W) only
        if (now - scanMark >= DIGIT_SCAN_PERIOD_MS) {
            scanMark = now;
            scanLower = !scanLower;
        } // if
    } // while
}

static void uDummyLoad_Initialize(void) {
    // System Initialization ---------------------------------------------------
    // initialize peripherals as configured in the MCC GUI
    SYSTEM_Initialize();
    // initialize modules
    Config_Init();
    Timing_Init();

    // Timers ------------------------------------------------------------------
    // configure timer0 to periodically call LCD update routine
    TMR0_SetInterruptHandler(LCD_Update);
    TMR0_StartTimer();
    
    // configure timer2 to periodically call the millisecond counting routine
    TMR2_SetInterruptHandler(Timing_UpdateMilliseconds);
    TMR2_StartTimer();

    // A/D Converter -----------------------------------------------------------
    // set A/D converter acquisition time in clock periods (TAD); 1 TAD = 2us
    // hardware delay to charge the hold capacitor before conversion occurs
    ADCC_LoadAcquisitionRegister(5);
    // discharge the sample and hold capacitor to prevent erroneous A/D 
    // readings after initial power up
    ADCC_GetSingleConversion(channel_VSS);
    
    // VDD Monitoring ----------------------------------------------------------
    // enable the high/low voltage detect (HLVD) for monitoring VDD
    HLVD_Enable();
    while (!HLVD_IsBandGapVoltageStable());

    // Interrupt Enable --------------------------------------------------------
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    // Configuration Data Bootloader -------------------------------------------
    // load configuration data from EEPROM or receive from external MCU
    // NOTE: make sure to call after interrupts enabled so that Timing
    // module functions work
    enum eConfigError error = Config_Load();
    if (error != CONFIG_ERROR_NONE) {
        // display error code
        while(1) {
            LCD_PrintString("Ec");
            DELAY_milliseconds(1000);
            LCD_Print(error, 0);
            DELAY_milliseconds(1000);
        } // while
    } // if
}

static void uDummyLoad_FactoryTest(void) {
#define FT_LCD_DELAY 500
    
    // test digit 3
    for (size_t i = 0; i < 10; ++i) {
        LCD_Print(i, 1);
        DELAY_milliseconds(FT_LCD_DELAY);
    } // for
    
    // test digit 2
    for (size_t i = 10; i < 100; i += 10) {
        LCD_Print(i, 1);
        DELAY_milliseconds(FT_LCD_DELAY);
    } // for
    
    // test digit 1
    for (size_t i = 100; i < 300; i += 100) {
        LCD_Print(i, 1);
        DELAY_milliseconds(FT_LCD_DELAY);
    } // for
}

static uint32_t uDummyLoad_GetVpeakMillivolts(void) {
    uint8_t vddIsLow = 0;
    uint16_t vdividerCode, vdiodeCode;
    uint32_t vpeakMillivolts;
    
    // A/D Voltage Reference Selection -----------------------------------------
    // - use the external 4.096V reference when VDD greater than about 4.2V
    // - use the internal 1.042V FVR when VDD less than 4.2V to prevent 
    //   erroneous readings due to dropout of the 4.096V reference
    vddIsLow = HLVD_OutputStatusGet();
    if (vddIsLow)
        PowerManager_SetVrefMv(1024);
    else
        PowerManager_SetVrefMv(4096);

    // Analog to Digital Conversion --------------------------------------------
    // - convert and read divider and diode voltage A/D codes
    // - prevent LCD GPIO writes during the conversions to reduce noise
    LCD_Lock();
    DELAY_microseconds(100);    // wait for any transients to fade
    vdividerCode = ADCC_GetSingleConversion(VDIVIDER);
    if (Config_IsDiodeCompEnabled())
        vdiodeCode = ADCC_GetSingleConversion(VDIODE);
    else
        vdiodeCode = 0;
    LCD_Unlock();

    // Fixed-Point Peak Voltage Calculation ------------------------------------
    // compute the peak input voltage Vpk as
    //  Vpk = Av * Vdivider + Vdiode + Vos
    // where 
    //  Av = 11V/V is the voltage division factor
    //  Vos = 0.017V(default) is the diode offset voltage
    vpeakMillivolts = ((uint32_t)vdividerCode * 11 
        + (uint32_t)vdiodeCode) * PowerManager_GetVrefMv() / 1023 
        + Config_GetDiodeOffsetMv();
    
    return vpeakMillivolts;
}

static void CircBuf_Init(struct sCircBuf *buf) {
    buf->head = 0;
    buf->sum = 0;
    buf->runon = CIRCBUF_LENGTH;
    for (size_t i = 0; i < CIRCBUF_LENGTH; ++i)
        buf->values[i] = 0;
}

static void CircBuf_Store(struct sCircBuf *buf, uint32_t value) {
    // update moving sum by subtracting oldest value and adding new value
    buf->sum += value;
    buf->sum -= buf->values[buf->head];
    
    // store new value in the buffer
    buf->values[buf->head] = value;
    if (++buf->head >= CIRCBUF_LENGTH)
        buf->head = 0;
    
    if (buf->runon > 0)
        --buf->runon;
}

static uint32_t CircBuf_GetAvg(struct sCircBuf *buf) {
    uint32_t divisor, sum, avg;
    
    divisor = CIRCBUF_LENGTH - buf->runon;
    if (divisor == 0)
        // don't want to divide by zero if this is called before items
        // are added to the circular buffer
        divisor = CIRCBUF_LENGTH;

    sum = buf->sum;
    avg = sum / divisor;   // integer divide rounds down
    if (2 * (sum % divisor) >= divisor)
        // round up if remainder greater than or equal to half of divisor
        ++avg;
    
    return avg;
}
