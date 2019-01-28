#include "App_Include.h"

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

    Mod_Usb4000Init();
    Mod_UsbHostInit();
    Mod_TemperInit(&st_LaserTemper);
    Mod_TemperInit(&st_PcbTemper);

    TRACE_DBG("==================================================================================\r\n");
    TRACE_DBG("| 程序名称 | GAS_LASER                                                            \r\n");
    TRACE_DBG("|----------|----------------------------------------------------------------------\r\n");
    TRACE_DBG("| 程序版本 | Ver 0.01                                                             \r\n");
    TRACE_DBG("==================================================================================\r\n");

    st_Device.e_State = e_DevSelfTest;

    return TRUE;
}


BOOL App_DeviceStart(void)
{
/*
    Bsp_AD5546Disable();
    Bsp_AD7622Disable();
    while(1)
    {

        Mod_UsbHostPoll();                  //USB主机接口处理
        Mod_Usb4000Poll();                  //光谱仪处理
        Mod_StdbusSlavePoll();              //通讯处理
        Mod_StdbusMasterPoll();             //通讯处理
        Mod_LaserPoll(&st_Laser);            //重新配置生成数组
    }
*/
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
//                                  等待进入上升沿
//==================================================================================
   while(st_Laser.e_State != eLaserRise){}
   //需要在此 让USB4000开始采集一张光谱


//==================================================================================
//                                  等待进入高电平
//==================================================================================
    while(st_Laser.e_State != eLaserHigh){
        Mod_UsbHostPoll();                  //USB主机接口处理
        Mod_Usb4000Poll();                  //光谱仪处理
        Mod_StdbusSlavePoll();              //通讯处理
        Mod_StdbusMasterPoll();             //通讯处理
    }
    //采集透过率高点



//==================================================================================
//                                  等待进入下降沿
//==================================================================================
    while(st_Laser.e_State != eLaserFall){}
//==================================================================================
//                                  等待进入低电平
//==================================================================================
    while(st_Laser.e_State != eLaserLow){}
    //采集透过率高点

    {
        INT16U  i;
        INT16U  uin_ResLen;
        FP32    pf_Res[200];



        Bsp_RunLed(eLedOn);

        for(i = 0; i < st_Laser.pst_Wave->uin_SampleDot;i++)
        {
            //st_Laser.pst_Wave->puin_RecvBuff[i] -= 32768UL;
            st_Laser.pst_Wave->puin_RecvBuff[i] = aui_TestSenseRecvBuff[i] - 32768UL;
        }

        Mod_DLiaCal(&st_DLia,
                     (INT16S*)st_Laser.pst_Wave->puin_RecvBuff,
                     st_Laser.pst_Wave->uin_SampleDot,
                     pf_Res,
                     &uin_ResLen);

        Bsp_RunLed(eLedOff);

        //打印吸收峰
        //for(i = 0; i < uin_ResLen; i++)
        //    TRACE_DBG("pf_Res[%04d] = %.6f\r\n",i,pf_Res[i]);

    }



    Mod_UsbHostPoll();                  //USB主机接口处理
    Mod_Usb4000Poll();                  //光谱仪处理
    Mod_StdbusSlavePoll();              //通讯处理
    Mod_StdbusMasterPoll();             //通讯处理
    Mod_LaserPoll(&st_Laser);            //重新配置生成数组

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

