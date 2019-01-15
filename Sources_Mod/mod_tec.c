//==================================================================================
//| 文件名称 | mod_tec.c    
//|----------|----------------------------------------------------------------------
//| 文件功能 | TEC控制
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#include "mod_tec.h"

Tec_t st_LaserTEC = {
    25.0,               /* 设置温度 */
    1.25,               /* 温控电压 */
    25.0,               /* 反馈的实际温度 */
    &st_LaserTemper,    /* 温度句柄 */
    FALSE,              /* 开启状态 */
    Bsp_Tec1Enable,     /* TEC操作函数 */
};

//==================================================================================
//| 函数名称 | Mod_TecSetVolt
//|----------|----------------------------------------------------------------------
//| 函数功能 | 设置TEC温控电压 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Tec:句柄   f_CtrlVolt:温度控制电压
//|----------|----------------------------------------------------------------------
//| 返回参数 | BOOL:成功 TRUE 失败 FALSE
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_TecSetVolt(Tec_t* pst_Tec, FP32 f_CtrlVolt)
{
    if(pst_Tec == NULL || pst_Tec->pst_Temper == NULL)
        return FALSE;
    
    if(f_CtrlVolt >= DEF_TECVOLT_MIN && f_CtrlVolt <= DEF_TECVOLT_MAX)
    {
        pst_Tec->f_SetCtrlVolt = f_CtrlVolt;
        pst_Tec->f_SetTemper = Mod_CalTemper(pst_Tec->pst_Temper,pst_Tec->f_SetCtrlVolt);

        if(SaveToEeprom((INT32U)&pst_Tec->f_SetCtrlVolt) != TRUE)
            return FALSE;

        Bsp_Printf("    >>TEC温控电压设置为:%.4fV(%.4f℃)\r\n",pst_Tec->f_SetCtrlVolt,pst_Tec->f_SetTemper);
        Bsp_AD5663Set(eAD5563_CHB, Bsp_AD5663CHBVoltToHex(pst_Tec->f_SetCtrlVolt));
        return TRUE;
    }
    else
        return FALSE;
}

//==================================================================================
//| 函数名称 | Mod_TecEnable
//|----------|----------------------------------------------------------------------
//| 函数功能 | 开启TEC 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Tec:句柄   uin_TimeOut:超时时间单位S 0:不等待 -1:一直等待
//|----------|----------------------------------------------------------------------
//| 返回参数 | BOOL:指定时间到达设定温度返回TRUE 没有到达返回FALSE
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_TecEnable(Tec_t* pst_Tec, INT16S uin_TimeOut)
{
    INT16U  i = 0;
    Bsp_Printf("\r\n===========================TEC启动==========================\r\n");
    Bsp_Printf("    >>设置TEC温控电压\r\n");
    Mod_TecSetVolt(pst_Tec,pst_Tec->f_SetCtrlVolt);
    Bsp_Printf("    >>启动TEC\r\n");
    pst_Tec->cb_TecOps(eTecEnable);
    
    if(uin_TimeOut == -1)
    {
        Bsp_Printf("    >>TEC等待到达设定温度\r\n");
        while(1)
        {
            Bsp_DelayMs(1000);
            pst_Tec->f_FbTemper = Mod_GetTemper(pst_Tec->pst_Temper);
            Bsp_Printf("    >>第%u秒TEC温度:%.4f\r\n",i,pst_Tec->f_FbTemper);
            if(abs(pst_Tec->f_FbTemper - pst_Tec->f_SetTemper) <= 1.0)
            {
                Bsp_Printf("    >>TEC到达设定温度\r\n");
                return TRUE;
            }
        }
    }
    else
    {
        Bsp_Printf("    >>TEC等待到达设定温度\r\n");
        for(i =0; i < uin_TimeOut; i++)
        {
            Bsp_DelayMs(1000);
            pst_Tec->f_FbTemper = Mod_GetTemper(pst_Tec->pst_Temper);
            Bsp_Printf("    >>第%u秒TEC温度:%.4f\r\n",i,pst_Tec->f_FbTemper);
            /* 到达设定温度 */
            if(abs(pst_Tec->f_FbTemper - pst_Tec->f_SetTemper) <= 1.0)
            {
                Bsp_Printf("    >>TEC到达设定温度\r\n");
                return TRUE;
            }
        }
        Bsp_Printf("    >>TEC等待温度超时\r\n");        
    }

    return FALSE;
}
