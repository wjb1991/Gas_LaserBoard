#include "App_Include.h"

#define DEF_DEV_DBG_EN           TRUE

#if (DEF_DEV_DBG_EN == TRUE)
    #define DEV_DBG(...)             do {                            \
                                            Bsp_Printf(__VA_ARGS__);    \
                                        }while(0)
#else
    #define DEV_DBG(...)
#endif

Device_t st_Device = {
    e_DevInit,
    {e_NoAlarm,e_NoAlarm,e_NoAlarm,e_NoAlarm,e_NoAlarm},
};


BOOL App_DeviceInit(void)
{
    Bsp_Init();

    Mod_StdbusInit(&st_StdbusHost,0x20);
    Mod_StdbusSlaveInit();
    Mod_StdbusMasterInit();

    Mod_TemperInit(&st_LaserTemper);
    Mod_TemperInit(&st_PcbTemper);


    TRACE_DBG("==================================================================================\r\n");
    TRACE_DBG("| 程序名称 | GAS_LASER                                                            \r\n");
    TRACE_DBG("|----------|----------------------------------------------------------------------\r\n");
    TRACE_DBG("| 程序版本 | Ver 0.01                                                             \r\n");
    TRACE_DBG("==================================================================================\r\n");

    st_Device.e_State = e_DevSelfTest;

    //Mod_Usb4000Init();
    //Mod_UsbHostInit();

    while(0)
    {
       //Mod_UsbHostPoll();                  //USB主机接口处理
       //Mod_Usb4000Poll();                  //光谱仪处理
    }

    return TRUE;
}


BOOL App_DeviceStart(void)
{
    Mod_MeasureInit(&st_Measure);
    Mod_GasMeasInit(&st_GasMeasForIr);
    Mod_GenerateModWave(&st_ModWave);                //生成调制波数组
    Mod_GainInit(&st_Gain);                           //根据EPROM存储的数据 更改运放的放大倍数

    st_DLia.f_PsdFreq = st_ModWave.f_SinFreq * 2;      // 放大正弦波的二次谐波
    st_DLia.f_SampleFreq = st_ModWave.f_SampleFreq;    // 采样频率

    Mod_LaserEnable(&st_Laser);
    st_Device.e_State = e_DevRun;
    return TRUE;
}

BOOL App_DevicrRun(void)
{
//==================================================================================
//                                  等待进入高电平
//==================================================================================
    while(st_Laser.e_State != eLaserHigh && st_Laser.e_State != eLaserStop){}
    if(st_Laser.e_State == eLaserHigh)
    {
        //采集透过率高点
        Bsp_DelayUs(240);

        Bsp_RunLed(eLedOn);
        Mod_TransSmapleHigh();
        Bsp_RunLed(eLedOff);
    }

//==================================================================================
//                                  等待进入低电平
//==================================================================================
    while(st_Laser.e_State != eLaserLow && st_Laser.e_State != eLaserStop && st_Laser.e_State != eLaserIdle){}
    if(st_Laser.e_State == eLaserLow)
    {
        //采集透过率高点
        Bsp_DelayUs(240);

        Bsp_RunLed(eLedOn);
        Mod_TransSmapleLow();
        Bsp_RunLed(eLedOff);

        if(st_Laser.e_State != eLaserIdle)            //如果在采样透过率下限时发生触发则不发送数据
        {
            //DEV_DBG("DEV_DBG:发送数据\r\n");
            Mod_SpectrumPost((INT16U*)st_ModWave.puin_RecvBuff, st_ModWave.uin_SampleDot);   //接受缓冲区已经被释放了
            //Mod_SpectrumPost((INT16U*)aui_TestSenseRecvBuff, st_ModWave.uin_SampleDot);     //使用调试数组计算
        }
    }

//==================================================================================
//                                  其他逻辑处理
//==================================================================================
    Mod_StdbusSlavePoll();                         //通讯处理
    Mod_StdbusMasterPoll();                        //通讯处理
    Mod_MeasurePoll(&st_Measure);                   //气体浓度分析

    Mod_LaserPoll(&st_Laser);                       //等待进入下一个周期

//==================================================================================
//                                   核心计算
//==================================================================================

    Mod_GasMeasPoll(&st_GasMeasForIr);


    /**
    Bsp_DelayMs(38);
    Mod_LaserDoStop(&st_Laser);
    Bsp_DelayMs(10);
    Mod_LaserExitIdle(&st_Laser);

    DEV_DBG(">>GASMEASIR_DBG: %s浓度 = %f, K = %f, B = %f, R = %f\r\n",
                  st_GasCO2.puch_Name,
                  st_GasCO2.f_Con,
                  st_GasCO2.f_K,
                  st_GasCO2.f_B,
                  st_GasCO2.f_R);

    DEV_DBG(">>GASMEASIR_DBG: %s浓度 = %f, K = %f, B = %f, R = %f\r\n",
                  st_GasCO.puch_Name,
                  st_GasCO.f_Con,
                  st_GasCO.f_K,
                  st_GasCO.f_B,
                  st_GasCO.f_R);
                  */
    //Mod_UsbHostPoll();                  //USB主机接口处理
    //Mod_Usb4000Poll();                  //光谱仪处理


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

    case e_DevRun:

        App_DevicrRun();
        break;

    case e_DevError:

        break;

    default:
        break;

    }
    return TRUE;

}

void App_AddOneAlarm(DevAlarmCode_t uch_AlarmCode)
{
    INT8U i;

    /* 搜索这个报警码 是否存在 */
    for(i = 0; i < 5; i++)
    {
        if (st_Device.ae_Alarm[i] == uch_AlarmCode)
            return; //存在则不添加
    }

    /* 添加这个报警码 */
    for(i = 0; i < 5; i++)
    {
        if (st_Device.ae_Alarm[i] == e_NoAlarm)
        {
            st_Device.ae_Alarm[i] = (DevAlarmCode_t)uch_AlarmCode;
            return;
        }
    }

    /* 如果是故障代码则会删除一个警告 */
    if(uch_AlarmCode > e_FaultFlag)
    {
        for(i = 0; i < 5; i++)
        {
            if (st_Device.ae_Alarm[i] <= e_WarningFlag)
            {
                st_Device.ae_Alarm[i] = (DevAlarmCode_t)uch_AlarmCode;
                return;
            }
        }
    }
}


void App_ClrOneAlarm(DevAlarmCode_t uch_AlarmCode)
{
    INT8U i;
    /* 搜索这个报警码 是否存在 */
     for(i = 0; i < 5; i++)
     {
         st_Device.ae_Alarm[i] = e_NoAlarm;
     }
}

