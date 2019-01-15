//==================================================================================
//| 文件名称 | 激光发射接收处理
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#ifndef __MOD_DOUBLEBUFF_H__
#define __MOD_DOUBLEBUFF_H__

#include "mod_include.h"

typedef struct __DoubleBuff {
    BOOL    b_SwitchFlag;   /* 接受缓冲区切换标志  */
    INT16U  uin_BuffLen;        /* 接受缓冲区长度      */
    void* 	pv_UseBuff;      /* 接受缓冲区 乒乓切换 */
    void* 	pv_IdleBuff;      /* 处理缓冲区 乒乓切换 */

}DoubleBuff_t;

extern void Mod_DoubleBuffInit(DoubleBuff_t* pst_DoubleBuff, void * vp_Buff0, void *vp_Buff1, INT16U uin_BuffLen);
extern void Mod_SwitchBuff(DoubleBuff_t* pst_DoubleBuff);

#endif
