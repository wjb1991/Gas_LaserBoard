//==================================================================================
//| 文件名称 | LTC1867驱动
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================

#ifndef __BSP_LTC1867_H__
#define __BSP_LTC1867_H__

#include "bsp.h"

typedef enum {
    eSignalDC = 0,
    eLaserVolt = 1,
    eLaserCurr = 2,
    eI_DC = 3,
    eRefTempVolt = 4,
    eBoardTemp = 5,
    eSenseTemp = 6,
    eLaserTemp = 7,
}AdcChannel_t;

void Bsp_Ltc1867Init(void);

INT16U Bsp_Ltc1867SampleOne(INT16U ch);
INT16U Bsp_Ltc1867SampleAvg(INT16U ch,INT16U avg);
FP64   Bsp_Ltc1867HexToVolt(INT16U hex);

#endif /* _BSP_LTC1867_H_ */
