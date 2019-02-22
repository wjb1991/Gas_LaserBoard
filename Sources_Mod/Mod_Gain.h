//==================================================================================
//| 文件名称 | Mod_Range.h
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#ifndef __MOD_RANGE_H__
#define __MOD_RANGE_H__




typedef struct {
    INT16S  in_DcGain;
    INT16S  in_AcGain;
}Gain_t;

extern Gain_t st_Gain;

BOOL Mod_GainInit(Gain_t* pst_Gain);

BOOL Mod_GainSetAcGain(Gain_t* pst_Gain, INT16S in_Gain,BOOL b_WriteEPROM);
BOOL Mod_GainSetDcGain(Gain_t* pst_Gain, INT16S in_Gain,BOOL b_WriteEPROM);

#endif
