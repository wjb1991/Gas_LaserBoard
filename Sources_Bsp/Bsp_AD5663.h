#ifndef __BSP_AD5663_H__
#define	__BSP_AD5663_H__

#include "Bsp.h"

typedef enum {
    eAD5663_CHA = 0,    eAD5563_CHB = 1,    eAD5563_CHAB = 7
}Ad5563Channel_t;

#ifdef __cplusplus
extern "C" {
#endif


INT16U Bsp_AD5663CHAVoltToHex(FP64 volt);
INT16U Bsp_AD5663CHBVoltToHex(FP64 volt);

void Bsp_AD5663Set(Ad5563Channel_t channel,uint16_t val);

void Bsp_AD5663Init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif  /* __cplusplus */

#endif
