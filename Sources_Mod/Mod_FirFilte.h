//==================================================================================
//| 文件名称 | 软件FIR滤波器
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#ifndef __MOD_FIRFILTE_H__
#define __MOD_FIRFILTE_H__

//纯软件IR
void Mod_FIRFilter(FP32 * pf_Input, INT16U uin_Lenth, const FP32* pf_Factor,INT16U uin_Order,INT16U uin_Avg,INT16U uin_Spand);
//使用DSP库优化的FIR
void Mod_FIRFilterDsp(FP32 * pf_Input, INT16U uin_Lenth, const FP32* pf_Factor,INT16U uin_Order,INT16U uin_Avg,INT16U uin_Spand);
//使用双核优化的FIR
void Mod_FIRFilterTwoCpu(FP32 * pf_Input, INT16U uin_Lenth, const FP32* pf_Factor,INT16U uin_Order,INT16U uin_Avg,INT16U uin_Spand);

#endif
