#include "App_Include.h"

Device_t st_Device = {
    e_DevInit,
    {e_NoAlarm,e_NoAlarm,e_NoAlarm,e_NoAlarm,e_NoAlarm},
};


void SpectrumReady(FP32* pf_Spectrum,INT16U uin_SpectrumLen)
{
    Mod_GasMeasForIr(&st_GasMeasForIr, pf_Spectrum, uin_SpectrumLen);
}

BOOL App_DeviceInit(void)
{
    Bsp_Init();

    Mod_StdbusInit(&st_StdbusHost,0x20);
    Mod_StdbusSlaveInit();
    Mod_StdbusMasterInit();

    Mod_TemperInit(&st_LaserTemper);
    Mod_TemperInit(&st_PcbTemper);


    st_IrSpectrum.cb_SpectrumReady = SpectrumReady;

    TRACE_DBG("==================================================================================\r\n");
    TRACE_DBG("| 程序名称 | GAS_LASER                                                            \r\n");
    TRACE_DBG("|----------|----------------------------------------------------------------------\r\n");
    TRACE_DBG("| 程序版本 | Ver 0.01                                                             \r\n");
    TRACE_DBG("==================================================================================\r\n");

    st_Device.e_State = e_DevSelfTest;

    Mod_Usb4000Init();
    Mod_UsbHostInit();

    while(1)
    {
       Mod_UsbHostPoll();                  //USB主机接口处理
       Mod_Usb4000Poll();                  //光谱仪处理
    }

    return TRUE;
}


BOOL App_DeviceStart(void)
{
    Mod_MeasureInit(&st_Measure);
    Mod_GasMeasInit(&st_GasMeasForIr);
    Mod_GenerateModWave(&st_ModWave);               //生成调制波数组

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
    while(st_Laser.e_State != eLaserHigh){}
    //采集透过率高点
    Bsp_DelayUs(1);
    Mod_TransSmapleHigh();                  //3MS左右的样子

//==================================================================================
//                                  等待进入低电平
//==================================================================================
    while(st_Laser.e_State != eLaserLow){}
    //采集透过率高点
    Bsp_DelayUs(1);
    Mod_TransSmapleLow();                   //3MS左右的样子


    Mod_MeasurePoll(&st_Measure);
    Bsp_RunLed(eLedOn);
    Mod_SpectrumProcForIr(&st_IrSpectrum);     //吸收峰计算完成后 接受缓冲区就已经被释放了
    Bsp_RunLed(eLedOff);

    Mod_StdbusSlavePoll();                  //通讯处理
    Mod_StdbusMasterPoll();                 //通讯处理

    Mod_LaserPoll(&st_Laser);




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

