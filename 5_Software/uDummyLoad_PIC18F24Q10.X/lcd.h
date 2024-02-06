/* 
 * lcd.h
 * =================
 * Routines for printing numbers to the 2.5 digit glass segmented LCD.
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>

/*
 * Print the specified fixed-point number to the display.
 * 
 * The displayed number is given by the following:
 * 
 *      n = fixedPointValue * 10^(-base10Exponent)
 * 
 * where fixedPointValue is constrained between 0 and 299 and base10Exponent 
 * is constrained between 0 and 2.
 * 
 * @param fixedPointValue Integer between 0 and 299.
 * @param base10Exponent Exponent between 0 and 1 determining the location of 
 *  the decimal point.
 * @return 1 if the operation was successful; 0 on error.
 */
uint8_t LCD_Print(uint16_t fixedPointValue, uint8_t base10Exponent);

/*
 * Print the specified string to the display.
 * 
 * @param string Character string with a maximum length of 2.
 * @return 1 if the operation was successful; 0 on error.
 */
uint8_t LCD_PrintString(char *string);

/*
 * Clear all digits on the display.
 */
void LCD_Clear(void);

/*
 * Prevent GPIO writes to the LCD for one update interval.
 * (Use to reduce noise during A/D conversion.)
 */
void LCD_Lock(void);

/*
 * Allow GPIO writes to the LCD.
 */
void LCD_Unlock(void);

/*
 * Update the display by toggling the I/O lines. Call this function with
 * an interval less than 33ms (30Hz) for best appearance.
 */
void LCD_Update(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

