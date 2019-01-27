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


typedef struct __SoftLia {
    FP32    f_PsdFreq;               /* 鉴相器频率 KHZ       */
    FP32    f_PsdPhase;              /* 鉴相器相位 角度      */
    FP32    f_SampleFreq;            /* 采样频率 KHZ         */
    INT16U  uin_SampleMaxDots;       /* 最大点数 */
    
    FP32*   pf_Buff;                 /* 计算缓冲区 */
}DLia_t;

extern DLia_t st_DLia;
extern const INT16U aui_TestSenseRecvBuff[10000];

BOOL Mod_DLiaInit(DLia_t* pst_DLia);

BOOL Mod_DLiaCal(DLia_t* pst_DLia,INT16S* puin_InData, INT16U uin_InDataLen,FP32* pf_OutData,INT16U* puin_OutDataLen);

BOOL Mod_DLiaSetPhase(DLia_t* pst_DLia,FP32 f_Phase);

#endif
