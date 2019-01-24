#include "App_Include.h"

Device_t st_Device = {
    .e_State = e_DevInit,

};


BOOL App_DeviceInit(void)
{
    Bsp_Init();

    Mod_StdbusInit(&st_StdbusHost,0x20);
    Mod_StdbusSlaveInit();
    Mod_StdbusMasterInit();

    Mod_Usb4000Init();
    Mod_UsbHostInit();

    TRACE_DBG("==================================================================================\r\n");
    TRACE_DBG("| 程序名称 | GAS_LASER                                                            \r\n");
    TRACE_DBG("|----------|----------------------------------------------------------------------\r\n");
    TRACE_DBG("| 程序版本 | Ver 0.01                                                             \r\n");
    TRACE_DBG("==================================================================================\r\n");

    st_Device.e_State = e_DevSelfTest;

    return TRUE;
}

BOOL App_DeviceSelfTest(void)
{

    TRACE_DBG("\r\n=========================I/O初始化=========================\r\n");
    TRACE_DBG("  >>LD_PROTECT=1,开启激光器保护\n");
    TRACE_DBG("  >>LD_SOFTON=1,激光器软起动功能关闭\n");
    TRACE_DBG("  >>Tec1_EN=1,激光器TEC模块禁能\n");
    TRACE_DBG("  >>Tec2_EN=1,传感器TEC模块禁能\n");

    TRACE_DBG("\r\n=========================外部SRAM自检=========================\r\n");
    if(Bsp_SramSelfTest() == FALSE)
        TRACE_DBG("  >>外部SRAM 自检失败\n");
    else
        TRACE_DBG("  >>外部SRAM 自检成功\n");

    TRACE_DBG("\r\n=========================EEPROM自检=========================\r\n");
        TRACE_DBG("  >>外部EEPROM 自检失败\n");
        TRACE_DBG("  >>外部EEPROM 自检成功\n");


    TRACE_DBG("\r\n=========================3.3V电压自检=========================\r\n");
    TRACE_DBG("  >>3.3V电压自检成功\n");
    TRACE_DBG("\r\n=========================1.2V电压自检=========================\r\n");
    TRACE_DBG("  >>1.2V电压自检成功\n");
    TRACE_DBG("\r\n========================REF2.5电压自检========================\r\n");
    TRACE_DBG("  >>REF2.5V电压自检成功\n");
    TRACE_DBG("\r\n=========================PCB温度自检==========================\r\n");
    TRACE_DBG("\r\n========================激光器温度自检========================\r\n");
    TRACE_DBG("\r\n========================传感器温度自检========================\r\n");

    st_Device.e_State = e_DevStart;

    return TRUE;
}

BOOL App_DeviceStart(void)
{
    Mod_GenerateModWave(&st_ModWave);      //生成调制波数组

    Mod_LaserEnable(&st_Laser);
    st_Device.e_State = e_DevWork;
    return TRUE;
}

BOOL App_DevicePoll(void)
{
    switch (st_Device.e_State)
    {
    case e_DevSelfTest:

        App_DeviceSelfTest();

        break;

    case e_DevStart:

        App_DeviceStart();

        break;

    case e_DevWork:

        Mod_UsbHostPoll();
        Mod_Usb4000Poll();
        Mod_StdbusSlavePoll();
        Mod_StdbusMasterPoll();

        break;

    case e_DevError:

        break;

    default:
        break;

    }
    return TRUE;

}
