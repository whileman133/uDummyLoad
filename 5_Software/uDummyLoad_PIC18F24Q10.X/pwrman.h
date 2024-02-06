/* 
 * pwrman.h
 * ===================
 * Routines for configuring power management options such as the
 * A/D converter reference voltage.
 */

#ifndef PWRMAN_H
#define	PWRMAN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
    /*
    * Set the A/D positive reference voltage.
    * 
    * @param vrefMillivolts Reference voltage in mV, either 4096 or 1024.
    * @return 1 on success; 0 if the specified reference voltage is invalid.
    */
    uint8_t PowerManager_SetVrefMv(uint16_t vrefMillivolts);
    
    /*
    * Fetch the current reference voltage.
    */
    uint16_t PowerManager_GetVrefMv(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PWRMAN_H */

