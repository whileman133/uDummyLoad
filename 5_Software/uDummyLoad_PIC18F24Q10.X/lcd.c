/*
 * lcd.c
 */

#include "lcd.h"
#include "xc.h"
#include <string.h>
#include <ctype.h>

#define NUM_DIGITS 3

// Use volatile to avoid compiler optimizations on the contained
// variables; they are accessed in the ISR.
static volatile struct sLcd {
    uint8_t sevenseg1;
    uint8_t sevenseg2;
    uint8_t sevenseg3;
    uint8_t lock;       // ISR lock to ensure operations are atomic
} lcd = {0};

// Segment bitmap for the first digit.
// The first digit is a "half" display, meaning the ADEG lines are tied
// together and segment F is left disconnected such that the only display-able
// digits are 1 and 2. Zero is displayed by turning all segments off.
// 1 = segment active; 0 = segment inactive
// The segment bits are packed for direct assignment to port B registers
static const uint8_t SEGMENT_MAP_DIGIT1[] = {
    // (ADEG)(B)(C)
    0x0,                        // NULL
    0x0,                        // 0 (turn off)
    0x3,                        // 1
    0x6,                        // 2
};

// Segment bitmap for the second digit.
// Maps digits between 0 and 9 to a bitmap describing the states of the ABCDEF 
// segments required to show the corresponding digit on the display.
// 1 = segment active; 0 = segment inactive
// The segment bits are packed for direct assignment to port C registers
static const uint8_t SEGMENT_MAP_DIGIT2[] = {
    // (G)(F)(A)_(C)(D)(E)(B)
    0x00,                       // NULL
    0x3F,                       // 0
    0x09,                       // 1
    0x57,                       // 2
    0x5D,                       // 3
    0x69,                       // 4
    0x7C,                       // 5
    0x7E,                       // 6
    0x19,                       // 7
    0x7F,                       // 8
    0x79,                       // 9
    0x00,                       // A
    0x00,                       // B
    0x46,                       // C
    0x00,                       // D
    0x76,                       // E
    0x72,                       // F
    0x00,                       // G
    0x6B,                       // H
    0x22,                       // I
    0x00,                       // J
    0x00,                       // K
    0x26,                       // L
    0x00,                       // M
    0x00,                       // N
    0x4E,                       // O
    0x00,                       // P
    0x00,                       // Q
    0x42,                       // R
    0x00,                       // S
    0x00,                       // T
    0x00,                       // U
    0x00,                       // V
    0x00,                       // W
    0x00,                       // X
    0x00,                       // Y
    0x00,                       // Z
};

// Segment bitmap for the third digit.
// Maps digits between 0 and 9 to a bitmap describing the states of the ABCDEF 
// segments required to show the corresponding digit on the display.
// 1 = segment active; 0 = segment inactive
// The segment bits are packed for direct assignment to port A registers
static const uint8_t SEGMENT_MAP_DIGIT3[] = {
    // (D)(E)(C)(G)_(null)(F)(A)(B)
    0x00,                       // NULL
    0xE7,                       // 0
    0x21,                       // 1
    0xD3,                       // 2
    0xB3,                       // 3
    0x35,                       // 4
    0xB6,                       // 5
    0xF6,                       // 6
    0x27,                       // 7
    0xF7,                       // 8
    0x37,                       // 9
    0x00,                       // A
    0x00,                       // B
    0xD0,                       // C
    0x00,                       // D
    0xD6,                       // E
    0x56,                       // F
    0x00,                       // G
    0x75,                       // H
    0x44,                       // I
    0x00,                       // J
    0x00,                       // K
    0xC4,                       // L
    0x00,                       // M
    0x00,                       // N
    0xF0,                       // O
    0x00,                       // P
    0x00,                       // Q
    0x50,                       // R
    0x00,                       // S
    0x00,                       // T
    0x00,                       // U
    0x00,                       // V
    0x00,                       // W
    0x00,                       // X
    0x00,                       // Y
    0x00,                       // Z
};

uint8_t LCD_Print(uint16_t fixedPointValue, uint8_t base10Exponent) {
    uint8_t digits[NUM_DIGITS] = {0};
    uint8_t ss1, ss2, ss3;
    
    if (fixedPointValue > 299 || base10Exponent > 1) {
        // the value is out of the displayable range
        return 0;
    } // if
    
    for (int i = 0; i < NUM_DIGITS; ++i) {
        digits[i] = fixedPointValue % 10;
        fixedPointValue /= 10;
    } // for
    
    // lookup 7-segment bitmaps based on extracted digits
    // (+1 to offset from NULL bitmap)
    ss3 = SEGMENT_MAP_DIGIT3[digits[0] + 1];
    ss2 = SEGMENT_MAP_DIGIT2[digits[1] + 1];
    ss1 = SEGMENT_MAP_DIGIT1[digits[2] + 1];
    
    // enable the correct decimal point
    switch (base10Exponent) {
        case 1:
            ss2 |= 0x80;    // DP2 state stored in bit8 of digit2's bitmap
            break;
    } // switch
    
    // setting the lock ensures the bitmaps are set atomically
    lcd.lock = 1;
    lcd.sevenseg1 = ss1;
    lcd.sevenseg2 = ss2;
    lcd.sevenseg3 = ss3;
    lcd.lock = 0;
    
    // success writing the display value
    return 1;
}

uint8_t LCD_PrintString(char *string) {
    size_t len = strlen(string);
    // indices into the segment maps for digits 2 and 3
    // (default to 0, the NULL index)
    uint8_t mapidx[2] = {0};
    uint8_t ss2, ss3;
    
    if (len > 2) {
        // only two or fewer characters may be displayed
        return 0;
    }
    
    for (int8_t i = 0; i < len; ++i) {
        char c = string[i];
        
        if (isalpha(c)) {
            // +11 to offset from digits 0-9 and NULL bitmaps
            mapidx[i] = (uint8_t)((uint8_t)toupper(c) - (uint8_t)'A' + 11);
        }
        else if (isdigit(c)) {
            // +1 to offset from NULL bitmap
            mapidx[i] = (uint8_t)(c - '0' + 1);
        }
    }
    
    ss2 = SEGMENT_MAP_DIGIT2[mapidx[0]];
    ss3 = SEGMENT_MAP_DIGIT3[mapidx[1]];
    
    // setting the lock ensures the bitmaps are set atomically
    lcd.lock = 1;
    lcd.sevenseg1 = 0;
    lcd.sevenseg2 = ss2;
    lcd.sevenseg3 = ss3;
    lcd.lock = 0;
    
    // success writing the display value
    return 1;
}

void LCD_Clear(void) {
    // setting the lock ensures the bitmaps are set atomically
    lcd.lock = 1;
    lcd.sevenseg1 = 0;
    lcd.sevenseg2 = 0;
    lcd.sevenseg3 = 0;
    lcd.lock = 0;
}

void LCD_Lock(void) {
    lcd.lock = 1;
}

void LCD_Unlock(void) {
    lcd.lock = 0;
}

void LCD_Update(void) {
    // NOTE: read pin values via LATx, not PORTx!
    // (reading from PORT gives the actual logic state of the pin,
    // which may differ from the latch value in some cases --e.g. when 
    // an output is configured open-drain)
    
    // frame sequencing counter
    static uint8_t tick = 0;
    
    if (lcd.lock) {
        // clear lock to prevent LCD lines from hanging in the same state 
        // for too long and damaging the segments
        lcd.lock = 0;
        return;
    } // if
    
    if (tick % 2 == 0) {
        // update active segments from global bitmaps
        
        // digit 1
        LATB &= 0xF0;              // clear COM and lines utilized by digit1
        LATB |= lcd.sevenseg1;
        
        // digit 2
        LATC = lcd.sevenseg2;      // all PORTC lines utilized by LCD
        
        // digit 3
        LATA &= 0x08;              // clear lines utilized by LCD digit3
        LATA |= lcd.sevenseg3;
    } // if
    else {
        // invert segment lines to alternate the voltage applied across
        // each LCD segment
        LATB ^= 0x0F;      // digit 1
        LATC ^= 0xFF;      // digit 2
        LATA ^= 0xF7;      // digit 3
    } // else
    
    ++tick;
}
