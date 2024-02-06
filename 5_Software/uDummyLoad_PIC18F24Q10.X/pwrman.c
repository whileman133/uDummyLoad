/*
 * pwrman.c
 */

#include "pwrman.h"
#include "xc.h"

struct sPowerManagement {
    uint16_t vrefMillivolts;
};
static struct sPowerManagement gPwrMan = {4096};

uint8_t PowerManager_SetVrefMv(uint16_t vrefMillivolts) {
    switch (vrefMillivolts) {
        case 4096:
            ADREF = 0x2;    // connect A/D Vref+ to external 4.096V reference
            break;
        case 1024:
            ADREF = 0x3;    // connect A/D Vref+ to internal 1.024V FVR
            break;
        default:
            return 0;
    }
    
    gPwrMan.vrefMillivolts = vrefMillivolts;
    return 1;
}

uint16_t PowerManager_GetVrefMv(void) {
    return gPwrMan.vrefMillivolts;
}
