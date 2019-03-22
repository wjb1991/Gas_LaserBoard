//==================================================================================
//| 文件名称 | 软件锁相放大器
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#include "App_Include.h"
#include "fpu_filter.h"
#include "Mod_TestSense.h"
#include "Mod_FirCoeff1.h"      //1200K-1500-4900-1-100     1091
#include "Mod_FirCoeff2.h"      //120K-1200-2400-1-80       253

//调试参数
//400k_200_9000_1_100
//40k_10_500_1_100

#define     DEF_DLA_DBG_EN           FALSE

#if (DEF_DLA_DBG_EN == TRUE)
    #define DLA_DBG(...)            do {                            \
                                            Bsp_Printf(__VA_ARGS__);    \
                                        }while(0)
#else
    #define DLA_DBG(...)
#endif

/**/
#ifdef __cplusplus
#pragma DATA_SECTION("PrivateRam")
#else
#pragma DATA_SECTION(B1,"PrivateRam");
#endif
//低通 1200k_5000_18000_1_100

#ifdef __cplusplus
#pragma DATA_SECTION("PrivateRam")
#else
#pragma DATA_SECTION(B2,"PrivateRam");
#endif

#ifdef __cplusplus
#pragma DATA_SECTION("PublicRam")
#else
#pragma DATA_SECTION(af_Buff,"PublicRam");
#endif
static FP32 af_Buff[DEF_SAMPLEDOT_MAX] = {0};

DLia_t st_DLia = {
    10.0,                       /* 鉴相器频率 KHZ       */
    0.0,                        /* 鉴相器相位 角度      */
    200,                        /* 采样频率 KHZ         */
    DEF_SAMPLEDOT_MAX,          /* 最大点数 */
    
    af_Buff,                   /* 计算缓冲区 */
};


//==================================================================================================
//| 函数名称 | Mod_DLiaGeneratePsdWave
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 在缓冲区中 生成鉴相器波形
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_DLia: 锁相放大器句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
#pragma CODE_SECTION(Mod_DLiaGeneratePsdWave, ".TI.ramfunc");       //加载到Ram当中去运行 看情况使用
BOOL Mod_DLiaGeneratePsdWave(DLia_t* pst_DLia, INT16U* puin_InData, INT16U uin_InDataLen)
{
    INT16U i;
	FP64 f1;
	FP64 fdp,fdt,fw;
	FP64 mPI= 3.1415926535897932384626433832795;
/*
    //使用软件生成正弦波
	fdt = 1.0 / (pst_DLia->f_SampleFreq * 1000);        // 计算采样周期 S
	fdp = pst_DLia->f_PsdPhase / 180 * mPI;             // 转换角度为弧度
	fw = pst_DLia->f_PsdFreq * 1000;                    // 计算正弦波频率 HZ

	for(i = 0; i < pst_DLia->uin_SampleMaxDots; i++)
	{
		//f1 = __sin((fw * i * fdt + __mpy2pif32(pst_DLia->f_PsdPhase/360)));
        f1 = sin( 2 * mPI * (fw * i * fdt) + fdp);

		pst_DLia->pf_Buff[i] = (FP32)f1;
        //喂狗
	}
*/
	//使用硬件TMU生成正弦波
    fdt = 1.0 / (pst_DLia->f_SampleFreq * 1000);        // 计算采样周期 S
    fdp = pst_DLia->f_PsdPhase / 360;                   // 转换角度为弧度
    fw = pst_DLia->f_PsdFreq * 1000;                    // 计算正弦波频率 HZ
    for(i = 0; i < pst_DLia->uin_SampleMaxDots; i++)
    {
        f1 = __sinpuf32((fw * i * fdt + fdp));
        //pst_DLia->pf_Buff[i] = (aui_TestSenseRecvBuff[i]) * f1;       //使用调试数组计算
        pst_DLia->pf_Buff[i] = (puin_InData[i]) * f1;   //使用接受的数据计算
        //喂狗
    }

	return TRUE;
}

#pragma CODE_SECTION(Mod_DLiaGeneratePsdWave, ".TI.ramfunc");       //加载到Ram当中去运行 看情况使用
BOOL Mod_DLiaGeneratePsdWaveTwoCpu(DLia_t* pst_DLia)
{
/*
    while(IPCLtoRFlagBusy(IPC_FLAG11) != 0){}           //任务是否完成
    DLia_t* p = (void*)CPU01_TO_CPU02_PASSMSG;
    p->f_PsdFreq = pst_DLia->f_PsdFreq;
    p->f_PsdPhase = pst_DLia->f_PsdPhase;
    p->f_SampleFreq = pst_DLia->f_SampleFreq;
    p->pf_Buff = pst_DLia->pf_Buff;
    p->uin_SampleMaxDots = pst_DLia->uin_SampleMaxDots;
    IPCLtoRFlagSet(IPC_FLAG11);                          //开启CPU2任务 计算FIR后半部分
*/
    return TRUE;
}

//==================================================================================================
//| 函数名称 | Mod_DLiaSetPhase
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 设置锁相放大器相位
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_DLia: 锁相放大器句柄	f_Phase:角度 0~360 b_Init:是否重新生成数组标记
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 |
//==================================================================================================
BOOL Mod_DLiaSetPhase(DLia_t* pst_DLia,FP32 f_Phase,BOOL b_WriteEPROM)
{
	if(f_Phase >= 0.0 && f_Phase < 360)
	{
		pst_DLia->f_PsdPhase = f_Phase;
        if(b_WriteEPROM == TRUE)
            if(SaveToEeprom((INT32U)&pst_DLia->f_PsdPhase) != TRUE)
                return FALSE;

        return TRUE;
	}
	return FALSE;
}

//==================================================================================================
//| 函数名称 | Mod_DLiaCal
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 数字锁相放大器计算
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pst_DLia: 锁相放大器句柄
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
#pragma CODE_SECTION(Mod_DLiaCal, ".TI.ramfunc");       //加载到Ram当中去运行 看情况使用
BOOL Mod_DLiaCal(DLia_t* pst_DLia,INT16U* puin_InData, INT16U uin_InDataLen,FP32* pf_OutData,INT16U* puin_OutDataLen)
{
    INT16U i;
    INT16U FirOrder1 = sizeof(B1)/sizeof(FP32);
    INT16U FirOrder2 = sizeof(B2)/sizeof(FP32);

    
    /*  鉴相器 把采样信号和鉴相器先前生成的信号相乘 
        因为 SinA*SinB= [Cos(A+B)+Cos(A-B)]/2
        所以 VPP*Sin(A)*Sin(A+Phase) = VPP*<[Cos(A+A+Phase)+Cos(A-A-+Phase)]/2>
                                     = VPP*<[Cos(2*A+Phase)+Cos(+-Phase)]/2>
    */
    //Mod_DLiaGenerateTwoCpu(pst_DLia);
    Bsp_AlarmLed(eLedOn);
    DLA_DBG("鉴相器\r\n");
    Mod_DLiaGeneratePsdWave(pst_DLia, puin_InData, uin_InDataLen);


    /* 低通滤波器 滤除高频成分 提取包络线 
       把 VPP*<[Cos(2*A+Phase)+Cos(+-Phase)]/2> 通过高通滤波器后
       得到 VPP*[Cos(+-Phase)]/2 当两个信号相位相同时或接近是 Phase ~= 0
       +-Phase是应为在第一或第四象限都是正值 在第二或第三象限都是负值
       得到 VPP*[Cos(0)]/2 = VPP/2
    */
    Bsp_AlarmLed(eLedOff);
    DLA_DBG("一级FIR滤波\r\n");
    Mod_FIRFilter(pst_DLia->pf_Buff, uin_InDataLen, B1, FirOrder1, 1, 10);			//5.1ms
    Bsp_AlarmLed(eLedOn);
    DLA_DBG("二级FIR滤波\r\n");
    Mod_FIRFilter(pst_DLia->pf_Buff, (uin_InDataLen-FirOrder1)/10, B2, FirOrder2, 1, 4);			//0.9ms
    Bsp_AlarmLed(eLedOff);

    /* 复制数据到输出数组 */
    if(pf_OutData != NULL)
    {
        DLA_DBG("复制数据\r\n");
        for(i = 0; i < 200; i++)
        {
            pf_OutData[i] = pst_DLia->pf_Buff[i];
        }
    }

    if(puin_OutDataLen != NULL)
    {
        DLA_DBG("输出长度\r\n");
        *puin_OutDataLen = 200;
    }

	return TRUE;
}
