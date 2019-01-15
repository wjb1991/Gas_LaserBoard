//==================================================================================
//| 文件名称 | 软件锁相放大器
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#ifndef __MOD_DLIA_H__
#define __MOD_DLIA_H__

#include "mod_include.h"

typedef struct __SoftLia {
    FP32    f_PsdFreq;               /* 鉴相器频率 KHZ       */
    FP32    f_PsdPhase;              /* 鉴相器相位 角度      */
    FP32    f_SampleFreq;            /* 采样频率 KHZ         */
    INT16U  uin_SampleMaxDots;       /* 最大点数 */
    
    INT16U  uin_SampleLen;           /* 采样点数量 */
    FP64*   plf_SampleBuff;          /* 采样点缓冲区 */
    INT16U  uin_ResultLen;           /* 结果点数 */
    FP64*   plf_ResultBuff;          /* 结果缓冲区 */
    
    FP64*   plf_CalBuff;             /* 计算缓冲区 */
    FP64* 	plf_PsdBuff;             /* 鉴相器波形 */
}DLia_t;

extern DLia_t st_DLia;

BOOL Mod_DLiaInit(DLia_t* pst_DLia);

BOOL  Mod_DLiaCal(DLia_t* pst_DLia,FP64* lf_ResBuff, INT16U* uin_ResLen);

BOOL Mod_DLiaSetPhase(DLia_t* pst_DLia,FP32 f_Phase,BOOL b_Init);

#endif
