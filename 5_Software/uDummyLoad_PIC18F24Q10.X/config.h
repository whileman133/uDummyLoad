/* 
 * File:   config.h
 * ================================
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
    
    enum eConfigError {
        CONFIG_ERROR_NONE = 0,
        CONFIG_ERROR_COMMUNICATION_SYNC = 1,
        CONFIG_ERROR_RECEIVE_TIMEOUT = 2,
        CONFIG_ERROR_BUFFER_OVERFLOW = 3,
        CONFIG_ERROR_HEADER_MISSING = 4,
        CONFIG_ERROR_TABLE_MALFORMED = 5,
        CONFIG_ERROR_TABLE_NOT_SORTED = 6,
    };
    
    enum eConfigPrecision {
        CONFIG_PRECISION_STANDARD = 0,
        CONFIG_PRECISION_EXTENDED = 1,
    };

    void Config_Init(void);
    enum eConfigError Config_Load(void);
    enum eConfigPrecision Config_GetPrecision(void);
    uint8_t Config_IsDiodeCompEnabled(void);
    uint8_t Config_IsCalibrationEnabled(void);
    uint8_t Config_IsFactoryTestEnabled(void);
    uint16_t Config_GetDiodeOffsetMv(void);
    uint16_t Config_GetDisplayPeriodMs(void);
    uint8_t Config_GetSamplingPeriodMs(void);
    uint16_t Config_TableLookup(uint16_t powerMilliwatts);

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

