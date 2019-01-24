//==================================================================================
//| 文件名称 | 双缓冲切换
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================

#include "App_Include.h"



void Mod_DoubleBuffInit(DoubleBuff_t* pst_DoubleBuff, void * vp_Buff0, void *vp_Buff1, INT16U uin_BuffLen)
{
    pst_DoubleBuff->b_SwitchFlag = FALSE;
    pst_DoubleBuff->uin_BuffLen = uin_BuffLen;
    pst_DoubleBuff->pv_UseBuff = vp_Buff0;
    pst_DoubleBuff->pv_IdleBuff = vp_Buff1;
}

/* 切换缓冲区 */
void Mod_SwitchBuff(DoubleBuff_t* pst_DoubleBuff)
{
    void * temp = pst_DoubleBuff->pv_UseBuff;

    pst_DoubleBuff->pv_UseBuff =  pst_DoubleBuff->pv_IdleBuff;
    pst_DoubleBuff->pv_IdleBuff = (void*)temp;
    
    if(pst_DoubleBuff->b_SwitchFlag == FALSE)
        pst_DoubleBuff->b_SwitchFlag = TRUE;
    else
        pst_DoubleBuff->b_SwitchFlag = FALSE;
}
