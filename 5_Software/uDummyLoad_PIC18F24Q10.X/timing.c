#include "timing.h"
#include "mcc_generated_files/tmr2.h"

struct sTiming {
    volatile uint32_t millisec, millisecLocked;
    volatile uint8_t lock;
};
static struct sTiming gTiming = {0};

void Timing_Init(void) {
    gTiming.millisec = 0;
    gTiming.millisecLocked = 0;
    gTiming.lock = 0;
}

uint32_t Timing_Milliseconds(void) {
    uint32_t millisec;
    
    // set the lock to ensure atomic read
    gTiming.lock = 1;
    millisec = gTiming.millisecLocked;
    gTiming.lock = 0;
    
    return millisec;
}

void Timing_UpdateMilliseconds(void) {
    gTiming.millisec += 1;
    
    // update the copy if it's not being read in another thread
    if (!gTiming.lock)
        gTiming.millisecLocked = gTiming.millisec;
}
