//==================================================================================
//| 文件名称 | 调制波处理
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#ifndef __MOD_WAVE_H__
#define __MOD_WAVE_H__

#include "App_Include.h"

#define     DEF_SINVPP_MIN      (0.0)
#define     DEF_SINVPP_MAX      (0.1)
#define     DEF_TRGVPP_MIN      (0.0)
#define     DEF_TRGVPP_MAX      (0.6)
#define     DEF_DCOFFSET_MIN    (0.0)    
#define     DEF_DCOFFSET_MAX    (1.0)    
#define     DEF_SINFREQ_MIN     (1.0)
#define     DEF_SINFREQ_MAX     (20.0)
#define     DEF_SAMPLEFREQ_MIN  (100.0)
#define     DEF_SAMPLEFREQ_MAX  (750.0)
#define     DEF_SAMPLEDOT_MIN   (1000)
#define     DEF_SAMPLEDOT_MAX   (10000)

#define		 DEF_FALLDOT_MIN	 (400)
#define		 DEF_FALLDOT_MAX	 (1000)

#define		 DEF_WAVE_MAX		 (2)

typedef struct {
    /* public */
    FP32    f_SinVpp;           /* 正弦波幅值 V */ 
    FP32    f_TrgVpp;           /* 三角波抬升电压 V */ 
    FP32    f_DcOffset;         /* 直流偏置电压 V */ 
    
    FP32    f_SinFreq;          /* 正弦波频率 KHZ */
    FP32    f_SampleFreq;       /* 采样频率 KHZ */
    INT16U  uin_SampleDot;      /* 采样点数 */
    
    FP32    f_RiseTime;         /* 上升沿持续时间 Ms*/    
    FP32    f_HigtTime;         /* 高电平持续时间 Ms*/    
    FP32    f_FallTime;         /* 下降沿持续时间 Ms*/
    FP32    f_LowTime;          /* 低电平持续时间 Ms*/
    
    FP32    f_Interval;         /* 每个点的间隔时间 US*/
    INT16U  uin_RiseDot;        /* 上升区段的数据缓冲长度 */
    INT16U  uin_HigtDot;        /* 高电平时的数据缓冲长度 */
    INT16U  uin_FallDot;        /* 下降区段的数据缓冲长度 */
    INT16U  uin_LowDot;         /* 低电平时的数据缓冲长度 */
    volatile INT16U* puin_RiseBuff;      /* 上升区段的数据缓冲 */
    volatile INT16U* puin_FallBuff;      /* 下降区段的数据缓冲 */
    
    FP32    f_HwDcOffset;       /* 硬件直流偏置电压 */    
    FP32    f_HwAcOffset;       /* 硬件直流偏置的交流分量 */

    volatile INT16U* puin_RecvBuff;      /* 接受波形 */

    volatile BOOL    b_GenerateWave;     /* 重新生成波形 */
}Wave_t;

extern Wave_t    st_ModWave;

extern BOOL Mod_GenerateModWave(void * pv_Wave);
extern BOOL Mod_SetSinVpp(void * pv_Wave,FP32 f_SinVpp,BOOL b_WriteEPROM);
extern BOOL Mod_SetTrgVpp(void * pv_Wave,FP32 f_TrgVpp,BOOL b_WriteEPROM);
extern BOOL Mod_SetDcOffset(void * pv_Wave,FP32 f_DcOffset,BOOL b_WriteEPROM);
extern BOOL Mod_SetSinFreq(void * pv_Wave,FP32 f_SinFreq,BOOL b_WriteEPROM);
extern BOOL Mod_SetSampleFreq(void * pv_Wave,FP32 f_SampleFreq,BOOL b_WriteEPROM);
extern BOOL Mod_SetSampleDot(void * pv_Wave,INT16U uin_SampleDot,BOOL b_WriteEPROM);
extern BOOL Mod_SetRiseDots(void * pv_Wave,INT16U uin_Dots,BOOL b_WriteEPROM);
extern BOOL Mod_SetHighDots(void * pv_Wave,INT16U uin_Dots,BOOL b_WriteEPROM);
extern BOOL Mod_SetLowDots(void * pv_Wave,INT16U uin_Dots,BOOL b_WriteEPROM);
extern BOOL Mod_SetFallDots(void * pv_Wave,INT16U uin_Dots,BOOL b_WriteEPROM);

#endif
