//==================================================================================
//| 文件名称 | mod_tec.h
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#ifndef __MOD_TEC_H__
#define __MOD_TEC_H__


#define   DEF_TECVOLT_MAX   (2.0)
#define   DEF_TECVOLT_MIN   (0.5)


typedef struct {
    FP32        f_SetTemper;    /* 设置温度 */
    FP32        f_SetCtrlVolt;  /* 温控电压 */
    FP32        f_FbTemper;     /* 反馈的实际温度 */
    Temper_t*   pst_Temper;     /* 温度句柄 */
    BOOL        b_Enable;       /* 开启状态 */   
    void (*cb_TecOps)(TecStatus_t); /* TEC操作函数 */
}Tec_t;

extern Tec_t st_LaserTEC;

BOOL Mod_TecSetVolt(Tec_t* pst_Tec, FP32 f_CtrlVolt,BOOL b_WriteEPROM);
BOOL Mod_TecEnable(Tec_t* pst_Tec, INT16S uin_TimeOut);

#endif
