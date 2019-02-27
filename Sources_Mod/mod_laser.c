//==================================================================================
//| 文件名称 | 激光发射接收处理
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#include "App_Include.h"

#if 0
#ifdef __cplusplus
#pragma DATA_SECTION("exsram")
#else
#pragma DATA_SECTION(auin_DoubleRecvBuff,"Exsram");
#endif
volatile INT16U auin_DoubleRecvBuff[2][DEF_SAMPLEDOT_MAX] = {0};    //

static DoubleBuff_t st_RecvDoubleBuff = {
    FALSE,                                  /* 接受缓冲区切换标志  */
    DEF_SAMPLEDOT_MAX,                      /* 接受缓冲区长度      */
    (void*)auin_DoubleRecvBuff[0],         /* 1接受缓冲区 乒乓切换 */
    (void*)auin_DoubleRecvBuff[1],         /* 处理缓冲区 乒乓切换 */
};
#endif


Laser_t    st_Laser = {
    eLaserOff,              /* 状态 */
    &st_ModWave,            /* 发送波形结构体 句柄 */
    &st_LaserTEC,           /* TEC句柄 */
};

static void Mod_RiseLevelInit(void* pv_Laser);
static void Mod_HigtLevelInit(void* pv_Laser);
static void Mod_FallLevelInit(void* pv_Laser);
static void Mod_LowLevelInit(void* pv_Laser);

//==================================================================================
//| 函数名称 | DMA_Handle
//|----------|----------------------------------------------------------------------
//| 函数功能 | DMA2 DAC输出DMA中断 切换四个状态
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Laser:句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void DMA_Handle(void)
{
    Bsp_Time0Stop();
    //TimeA上升 TimeB延时 TimeC下降 TimeD延时
    switch(st_Laser.e_State)
    {
    /* 上升沿结束 初始化高电平 */
    case eLaserRise:
        Mod_HigtLevelInit(&st_Laser);
        break;
    /* 高电平结束 初始化下降沿 */
    case eLaserHigh:
        Mod_FallLevelInit(&st_Laser);
        break;
    /* 下降沿结束 初始化低电平 */
    case eLaserFall:
        Mod_LowLevelInit(&st_Laser);
        break;
    /* 低电平结束 初始化上升沿 但不开启*/
    case eLaserLow:
        Mod_RiseLevelInit(&st_Laser);
        break;
    /* 下降状态结束 初始化上升沿 但不开启*/
    case eLaserStop:
        Mod_RiseLevelInit(&st_Laser);
        st_Laser.e_State = eLaserIdle;
        return; //低电平结束后不开启定时器
    default:
        Mod_RiseLevelInit(&st_Laser);
        break;
    }
    Bsp_Time0Start();
}

//==================================================================================
//| 函数名称 | DMA_Handle1
//|----------|----------------------------------------------------------------------
//| 函数功能 | DMA1 ADC接受DMA中断 调试用
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Laser:句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void DMA_Handle1(void)
{
    static int j = 0;
    if(j ==0 )
    {
        j = 1;
        Bsp_AlarmLed(eLedOff);
    }
    else
    {
        j = 0;
        Bsp_AlarmLed(eLedOn);
    }
}


//==================================================================================
//| 函数名称 | Mod_SetDcVolt
//|----------|----------------------------------------------------------------------
//| 函数功能 | 设置直流电压 
//|----------|----------------------------------------------------------------------
//| 输入参数 | f_DcVolt:直流电压(V)
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_SetDcVolt(FP32 f_DcVolt,FP32 f_AcVolt)
{
    static FP32 f_NowDcVolt = 0.0;
    static FP32 f_NowAcVolt = 0.0;
    INT16U    i,uin_Hex;
    FP32 f_Add;
    for(i = 1;i <= 100;i++)
    {
        f_Add = (f_DcVolt - f_NowDcVolt) / 100.0 * i;
        uin_Hex  = Bsp_AD5663CHAVoltToHex(f_NowDcVolt + f_Add);
        Bsp_AD5663Set(eAD5663_CHA,uin_Hex);

        f_Add = (f_AcVolt - f_NowAcVolt) / 100.0 * i;
        uin_Hex  = Bsp_AD5546VoltToHex(f_NowAcVolt + f_Add);
        Bsp_AD5546Set(uin_Hex);

        Bsp_DelayMs(2);
    }
    f_NowDcVolt = f_DcVolt;
    f_NowAcVolt = f_AcVolt;
}

FP32 Mod_LaserGetCurr(void* pv_Laser)
{
    Laser_t* p = pv_Laser;
    INT16U uin_Temp;
    FP32   f_Temp;
    uin_Temp = Bsp_Ltc1867SampleAvg(eLaserCurr,20);
    f_Temp = Bsp_Ltc1867HexToVolt(uin_Temp);
    p->f_Curr = f_Temp*1000 /1.5 /10;            //1.5是电压放大倍数 10是采样电阻
    return p->f_Curr;
}
//==================================================================================
//| 函数名称 | Mod_LaserEnable
//|----------|----------------------------------------------------------------------
//| 函数功能 | 开启激光器 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Laser:句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_LaserEnable(void* pv_Laser)
{
    INT16U i;

    Laser_t* p = pv_Laser;

    Bsp_Time0Stop();

    Mod_TecEnable(p->pst_Tec,20);

    Bsp_Printf("\r\n=========================激光器启动=========================\r\n");

    Bsp_Printf("    >>关闭Mos管钳位\r\n");
    Bsp_LaserPR(eLaserPrOff);
  
    Bsp_Printf("    >>软启动激光器电源\r\n");
    Bsp_SoftStart(eSofrtStartOn);
    
    Bsp_Printf("    >>设置直流偏置电压(100次分段)\r\n");
    Mod_SetDcVolt(p->pst_Wave->f_HwDcOffset,p->pst_Wave->f_HwAcOffset);

    Bsp_Printf("    >>打印10s激光器电流\r\n");
    for(i = 1; i <= 10 ; i++)
    {
        FP32 f_Curr = Mod_LaserGetCurr(p);
        Bsp_Printf("    >>第%02u秒激光器电流:%.4f(mA)\r\n",i,f_Curr);
        Bsp_DelayMs(1000);
    }

    Bsp_Printf("    >>启动定时器开启DMA输出\r\n");
    Mod_RiseLevelInit(p);
    Bsp_Time0Start();
}

//==================================================================================
//| 函数名称 | Mod_LaserDisable
//|----------|----------------------------------------------------------------------
//| 函数功能 | 关闭激光器 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Laser:句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_LaserDisable(void* pv_Laser)
{
    Bsp_Printf("\r\n=========================激光器关闭=========================\r\n");
    Bsp_Time0Stop();
    
    Bsp_Printf("    >>设置直流偏置电压为 (100次分段):0.0V \r\n");
    Mod_SetDcVolt(0.0,0.0);

    Bsp_Printf("    >>关闭激光器电流源...\r\n");
    Bsp_SoftStart(eSofrtStartOff);

    Bsp_Printf("    >>断开激光器保护继电器...\r\n");
    Bsp_LaserPR(eLaserPrOn);
}

void Mod_LaserDoStop(void* pv_Laser)
{
    Bsp_Time0Stop();
    Laser_t* p = pv_Laser;


    switch (p->e_State)
    {
    /* 上升沿 根据当前电压回到0位*/
    case eLaserRise:
        {
            TRACE_DBG("    >>上升沿 根据当前电压回到0位\r\n");

            INT16U i = Bsp_Dma1GetTranCont();
            INT16U hex = p->pst_Wave->puin_RiseBuff[i];
            for(i = 0; i < p->pst_Wave->uin_FallDot;i++)
            {
                if (hex < p->pst_Wave->puin_FallBuff[i])
                {
                    break;
                }
            }

            //TRACE_DBG("    >>i = %d, hex = %04x\r\n", i, hex);

            Bsp_Dma1Stop();
            Bsp_Dma2Stop();

            Bsp_Dma1Init((void*)&p->pst_Wave->puin_FallBuff[i],
                 (void*)DEF_AD5546ADDR,
                 1,
                 0,
                 p->pst_Wave->uin_FallDot - i);                   //后DA

            Bsp_Dma1HookRegister(&DMA_Handle);                  //注册DMA1回调函数
            Bsp_Dma1IntEnable();                                //开启DMA1

            Bsp_Dma1Start();                                    //开启DA
            Bsp_Dma2Stop();                                     //关闭AD接受
            p->e_State = eLaserStop;

            //TRACE_DBG("    >>p->pst_Wave->uin_FallDot - i = %d, hex = %04x\r\n", p->pst_Wave->uin_FallDot - i, p->pst_Wave->puin_FallBuff[i]);
        }
        break;
    /* 高电平 直接调用下降沿程序回到0位*/
    case eLaserHigh:
        TRACE_DBG("    >>高电平 直接调用下降沿程序回到0位\r\n");
        Bsp_Dma1Stop();
        Bsp_Dma2Stop();
        Mod_FallLevelInit(&st_Laser);
        p->e_State = eLaserStop;
        break;
    /* 下降沿 不改变 继续下降到0位*/
    case eLaserFall:
        TRACE_DBG("    >>下降沿 不改变 继续下降到0位\r\n");
        p->e_State = eLaserStop;
        break;
    /* 低电平 按照上升沿方式配置 但不开启定时器直接退出 */
    case eLaserLow:
        TRACE_DBG("    >>低电平 按照上升沿方式配置 但不开启定时器直接退出\r\n");
        Bsp_Dma1Stop();
        Bsp_Dma2Stop();
        Mod_RiseLevelInit(&st_Laser);
        p->e_State = eLaserIdle;
        return;
    }

    Bsp_Time0Start();
}

void Mod_LaserExitIdle(void* pv_Laser)
{
    Laser_t* p = pv_Laser;
    Bsp_Time0Start();
    p->e_State = eLaserRise;
}

void Mod_LaserPoll(void* pv_Laser)
{
    Laser_t* p = pv_Laser;
    if(p->pst_Wave->b_GenerateWave == TRUE)
    {
        Mod_LaserDisable(&st_Laser);                        /* 关闭激光器                 */
        Mod_GenerateModWave(&st_ModWave);                 /* 生成正弦波 填充数组        */
        Mod_LaserEnable(&st_Laser);                         /* 启动激光器                 */
    }
    else
    {
        if(p->e_State == eLaserLow)
        {
            while(st_Laser.e_State == eLaserLow){}
            //Bsp_Time0Start();
            //  <<在此处开启USB读取一张光谱
        }
        else if(p->e_State == eLaserRise)
        {
            //Bsp_Time0Start();
            //  <<在此处开启USB读取一张光谱
            TRACE_DBG("    >>低电平时间处理超时\r\n");
        }
    }
}



//==================================================================================
//| 函数名称 | Mod_RiseLevelInit
//|----------|----------------------------------------------------------------------
//| 函数功能 | 初始化下降沿区间 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Laser:句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_RiseLevelInit(void* pv_Laser)
{
    Laser_t* p = pv_Laser;
    //对应老程序TimeA
    p->e_State = eLaserRise;

    Bsp_Dma1Init((void*)p->pst_Wave->puin_RiseBuff,
        (void*)DEF_AD5546ADDR,
         1,
         0,
         p->pst_Wave->uin_RiseDot);                 //先DA

    Bsp_Dma2Init((void*)DEF_AD7622ADDR,
         (void*)p->pst_Wave->puin_RecvBuff,         //接受到使用缓冲区
         0,
         1,
         p->pst_Wave->uin_RiseDot);                 //后AD


    Bsp_Dma1HookRegister(&DMA_Handle);
    Bsp_Dma1IntEnable();

    Bsp_Dma2HookRegister(&DMA_Handle1);
    Bsp_Dma2IntEnable();
    Bsp_Dma1Start();
    Bsp_Dma2Start();

}

//==================================================================================
//| 函数名称 | Mod_HigtLevelInit
//|----------|----------------------------------------------------------------------
//| 函数功能 | 初始化高电平区间 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Laser:句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_HigtLevelInit(void* pv_Laser)
{
    Bsp_Dma2Stop();                                 //关闭AD接受

    Laser_t* p = pv_Laser;
    //对应老程序TimeB
    p->e_State = eLaserHigh;
    Bsp_Dma1Init((void*)&p->pst_Wave->puin_RiseBuff[p->pst_Wave->uin_RiseDot-1],      //最后一个点 维持低电平
         (void*)DEF_AD5546ADDR,
         0,
         0,
         p->pst_Wave->uin_HigtDot);        //后DA

    Bsp_Dma1HookRegister(&DMA_Handle);             //注册DMA1回调函数
    Bsp_Dma1IntEnable();                            //开启DMA1

    Bsp_Dma1Start();                                //开启DA
    
    /* 切换缓冲区 */
    //Mod_SwitchBuff(p->pst_DoubleRecvBuff);
    /*  发送到数据处理程序中 */

}
//==================================================================================
//| 函数名称 | Mod_FallLevelInit
//|----------|----------------------------------------------------------------------
//| 函数功能 | 初始化下降沿区间 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Laser:句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_FallLevelInit(void* pv_Laser)
{
    Laser_t* p = pv_Laser;
    //对应老程序TimeC
    p->e_State = eLaserFall;
    Bsp_Dma1Init((void*)p->pst_Wave->puin_FallBuff,
         (void*)DEF_AD5546ADDR,
         1,
         0,
         p->pst_Wave->uin_FallDot);        //后DA

    Bsp_Dma1HookRegister(&DMA_Handle);            //注册DMA2回调函数
    Bsp_Dma1IntEnable();                           //开启DMA2

    Bsp_Dma1Start();                                //开启DA
    Bsp_Dma2Stop();                                 //关闭AD接受
}

//==================================================================================
//| 函数名称 | Mod_LowLevelInit
//|----------|----------------------------------------------------------------------
//| 函数功能 | 初始化低电平区间 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Laser:句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Mod_LowLevelInit(void* pv_Laser)
{
    Laser_t* p = pv_Laser;
    //对应老程序TimeD
    p->e_State = eLaserLow;
    Bsp_Dma1Init((void*)&p->pst_Wave->puin_RiseBuff[0],   //第一个点 维持低电平
         (void*)DEF_AD5546ADDR,
         0,
         0,
         p->pst_Wave->uin_LowDot);        //后DA

    Bsp_Dma1HookRegister(&DMA_Handle);              //注册DMA2回调函数
    Bsp_Dma1IntEnable();                            //开启DMA2

    Bsp_Dma1Start();                                //开启DA
    Bsp_Dma2Stop();                                 //关闭AD接受
}



