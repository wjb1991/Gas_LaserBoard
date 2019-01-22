#ifndef __BSP_AD5546_H__
#define	__BSP_AD5546_H__

#include "Bsp.h"

#define  DEF_AD5546ADDR		(0x00300000)
#define  DEF_AD5546_REF_VOLTA   (2.5)       //参考电压
#define  DEF_AD5546_SCALE       (-5)        //运放放大倍数

#ifdef __cplusplus
extern "C" {
#endif

FP64 Bsp_AD5546HexToVolt(INT16U hex);

INT16U Bsp_AD5546VoltToHex(FP64 volt);

void Bsp_AD5546Disable(void);

void Bsp_AD5546Enable(void);

void Bsp_AD5546Reset(void);

void Bsp_AD5546Set(INT16U  data);

void Bsp_AD5546Init(void);


#ifdef __cplusplus
} /* extern "C" */
#endif  /* __cplusplus */

#endif
