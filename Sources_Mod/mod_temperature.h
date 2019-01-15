//==================================================================================
//| 文件名称 | 温度控制
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#ifndef __MOD_TEMPERATURE_H__
#define __MOD_TEMPERATURE_H__

#include "mod_include.h"

typedef struct __Temperature {
    const FP32* pf_Volt;            /* 拟合电压数据 */
    const FP32* pf_Temperature;     /* 拟合温度数   */
    INT16U  uch_SampleDots;         /* 拟合采样点数 */
    FP32    af_K[5];                /* 拟合采样系数 */
    INT8U   uch_Order;              /* 拟合阶数     */
    FP32    f_Temperature;          /* 温度 可不用  */
    AdcChannel_t e_Channel;		    /* 通道 	  */
}Temper_t;

extern Temper_t st_LaserTemper;
extern Temper_t st_PcbTemper;

extern FP32 Mod_CalTemper(Temper_t* pst_Temper,FP32 f_Volt);
extern FP32 Mod_GetTemper(Temper_t* pst_Temper);
extern BOOL Mod_TemperInit(Temper_t* pst_Temper);

#endif
