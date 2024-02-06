/* 
 * File:   timing.h
 * ====================================
 * Bare-bones real-time counter implementation.
 */

#ifndef TIMING_H
#define	TIMING_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>

    void Timing_Init(void);
    uint32_t Timing_Milliseconds(void);
    void Timing_UpdateMilliseconds(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMING_H */

