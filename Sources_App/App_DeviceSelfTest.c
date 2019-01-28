#include "App_Include.h"

BOOL App_DeviceSelfTest(void)
{
    INT16U i = 0;
    INT16U uin_Temp = 0;
    FP32 f_Temp = 0;
    FP32 f_Aver = 0;

    TRACE_DBG("\r\n=========================启动自测试=========================\r\n");

    TRACE_DBG("\r\nI/O初始化:\r\n");
    TRACE_DBG("  >>LD_PROTECT=1,开启激光器保护\n");
    TRACE_DBG("  >>LD_SOFTON=1,激光器软起动功能关闭\n");
    TRACE_DBG("  >>Tec1_EN=1,激光器TEC模块禁能\n");
    TRACE_DBG("  >>Tec2_EN=1,传感器TEC模块禁能\n");

    TRACE_DBG("\r\n=========================外部SRAM自检=========================\r\n");
    if(Bsp_SramSelfTest() == FALSE)
    {
        TRACE_DBG("  >>外部SRAM 自检失败\n");
        App_AddOneAlarm(e_Fault_ExramRam);
    }
    else
        TRACE_DBG("  >>外部SRAM 自检成功\n");

    TRACE_DBG("\r\n=========================EEPROM自检=========================\r\n");
    if(Bsp_At24c512SelfTest() == FALSE)
    {
        TRACE_DBG("  >>外部EEPROM 自检失败 重置EEPROM\n");
        InitParaFromEeprom(INIT_DEFAULT);
    }
    else
    {
        TRACE_DBG("  >>外部EEPROM 自检成功 从EEPROM中读取数\n");
        InitParaFromEeprom(INIT_NORMAL);
    }

    TRACE_DBG("\r\n======================3.3V电源电压检测======================\r\n");

    f_Aver = 0;
    for(i = 1; i <= 10; i++)
    {
        uin_Temp = Bsp_AdcSampleOne(BSP_HF_3_3V_M,BSP_HF_3_3V_CH);
        f_Temp = Bsp_AdcHexToVolt(uin_Temp);
        f_Aver += f_Temp;
        TRACE_DBG("  >>第%02d次采样值:%.4f(0x%04X)\n",i,f_Temp,uin_Temp);
        Bsp_DelayMs(100);
    }
    f_Aver = f_Aver/10;
    TRACE_DBG("  >>平均采样值:%.4f\n",f_Aver);
    f_Temp = 3.3/2;
    if(f_Aver>f_Temp*0.95 && f_Aver<f_Temp*1.05)
    {
        TRACE_DBG("  >>3.3V电源电压检测正常\n\n");
    }
    else
    {
        TRACE_DBG("  >>3.3V电源电压检测异常\n\n");
        while(1);
    }

    TRACE_DBG("\r\n======================1.2V电源电压检测======================\r\n");
    f_Aver = 0;
    for(i = 1; i <= 10; i++)
    {
        uin_Temp = Bsp_AdcSampleOne(BSP_1_2V_M,BSP_1_2V_CH);
        f_Temp = Bsp_AdcHexToVolt(uin_Temp);
        f_Aver += f_Temp;
        TRACE_DBG("  >>第%02d次采样值:%.4f(0x%04X)\n",i,f_Temp,uin_Temp);
        Bsp_DelayMs(100);
    }
    f_Aver = f_Aver/10;
    TRACE_DBG("  >>平均采样值:%.4f\n",f_Aver);
    f_Temp = 1.2;
    if(f_Aver>f_Temp*0.95 && f_Aver<f_Temp*1.05)
    {
        TRACE_DBG("  >>1.2V电源电压检测正常\n\n");
    }
    else
    {
        TRACE_DBG("  >>1.2V电源电压检测异常\n\n");
        while(1);
    }

    TRACE_DBG("\r\n======================REF2.5V参考电压检测=====================\r\n");

    f_Aver = 0;
    for(i=1; i<=10 ;i++)
    {
        uin_Temp = Bsp_AdcSampleOne(BSP_HF_REF2_5V_M,BSP_HF_REF2_5V_CH);
        f_Temp = Bsp_AdcHexToVolt(uin_Temp);
        f_Aver += f_Temp;
        TRACE_DBG("  >>第%02d次采样值:%.4f(0x%04X)\n",i,f_Temp,uin_Temp);
        Bsp_DelayMs(100);
    }
    f_Aver = f_Aver/10;
    TRACE_DBG("  >>平均采样值:%.4f\n",f_Aver);
    f_Temp = 2.5/2;
    if(f_Aver>f_Temp*0.95 && f_Aver<f_Temp*1.05 )
    {
        TRACE_DBG("  >>REF2.5V参考电压检测正常\n\n");
    }
    else
    {
        TRACE_DBG("  >>REF2.5V参考电压检测异常\n\n");
        while(1);
    }
    TRACE_DBG("\r\n=========================PCB温度自检==========================\r\n");
    for(i=1; i<=10 ;i++)
    {
        f_Temp = Mod_GetTemper(&st_PcbTemper);
        f_Aver += f_Temp;
        TRACE_DBG("  >>第%02d次采样值:%.4f\n",i,f_Temp);
        Bsp_DelayMs(100);
    }
    f_Aver = f_Aver/10;
    TRACE_DBG("  >>平均采样值:%.4f\n",f_Aver);

    Mod_GetTemper(&st_PcbTemper);
    TRACE_DBG("\r\n========================激光器温度自检========================\r\n");
    TRACE_DBG("\r\n========================传感器温度自检========================\r\n");

    st_Device.e_State = e_DevStart;

    return TRUE;
}
