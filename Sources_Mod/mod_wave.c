//==================================================================================
//| 文件名称 | 调制波处理
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#include "App_Include.h"

/*********************************
 * 发送接受缓冲区 大数组定义
 *********************************/

#ifdef __cplusplus
#pragma DATA_SECTION("Exsram")
#else
//#pragma DATA_SECTION(auin_RiseBuff,"Exsram");
#endif
volatile INT16U auin_RiseBuff[DEF_SAMPLEDOT_MAX] = {0};	//

#ifdef __cplusplus
#pragma DATA_SECTION("Exsram")
#else
//#pragma DATA_SECTION(auin_FallBuff,"Exsram");
#endif
volatile INT16U auin_FallBuff[DEF_FALLDOT_MAX]={0};

#ifdef __cplusplus
#pragma DATA_SECTION("Exsram")
#else
//#pragma DATA_SECTION(auin_RecvBuff,"Exsram");
#endif
volatile INT16U auin_RecvBuff[DEF_SAMPLEDOT_MAX] = {0};



Wave_t    st_ModWave = {
    0.025,          /* 正弦波幅值 V */ 
    0.45,           /* 三角波抬升电压 V */
    0.512,          /* 直流偏置电压 V */ 
    
    10.0,           /* 正弦波频率 KHZ */
    500.0,          /* 采样频率 KHZ */
    10000.0,        /* 采样点数 */
    
    0.0,            /* 上升沿持续时间 Ms*/    
    4.0,            /* 高电平持续时间 Ms*/
    1.0,            /* 下降沿持续时间 Ms*/
    10.0,           /* 低电平持续时间 Ms*/
    
    0.0,            /* 每个点的间隔时间 US*/
    0,              /* 上升区段的数据缓冲长度 */
    2000,           /* 高电平时的数据缓冲长度 */
    1000,           /* 下降区段的数据缓冲长度 */
    7000,           /* 低电平时的数据缓冲长度 */
    auin_RiseBuff,  /* 上升区段的数据缓冲 */
    auin_FallBuff,  /* 下降区段的数据缓冲 */
    
    0,              /* 硬件直流偏置电压 */

    auin_RecvBuff,  /* 接受波形缓冲 */
};

//==================================================================================
//| 函数名称 | Mod_GenerateModWave
//|----------|----------------------------------------------------------------------
//| 函数功能 | 生成数据波形数据 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Wave:句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | TRUE:成功 FALSE:失败
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_GenerateModWave(void * pv_Wave)
{
    FP64 fmax = 0.0;
    FP64 mPI = 3.1415926535897932384626433832795;
    FP64 f1 = 0.0,f2 = 0.0,f3 = 0.0, f = 0.0;
    FP64 fdt = 0.0,fpt = 0.0,fw = 0.0;    
    INT16U i;
    Wave_t* p = pv_Wave;
    
    /* 计算采样周期 单位S 1/KHZ*1000 = 1/HZ = S */
    fdt = 1.0 / (p->f_SampleFreq*1000.0);
    /* 计算采样周期 单位us 用于给定时器初始化 */
    fpt = fdt * 1e6;
    Bsp_Time0Init(fpt);
    /* 正弦波的频率 单位HZ */   
    fw = p->f_SinFreq * 1000;
   

    /* 计算各个过程时间和点数 
    p->f_Interval = fpt;
    p->uin_RiseDot = p->uin_SampleDot;
    p->uin_HigtDot = p->f_HigtTime * 1000 /  fpt;
    p->uin_FallDot = p->f_FallTime * 1000 /  fpt;
    p->uin_LowDot  = p->f_LowTime * 1000 / fpt;
    p->f_RiseTime = p->uin_SampleDot * p->f_Interval / 1000;*/
    
    /* 计算各个过程时间和点数 通过点数算时间 所有点数可配置 */
    p->uin_RiseDot = p->uin_SampleDot;
    p->f_RiseTime = p->uin_RiseDot * p->f_Interval / 1000;
    p->f_HigtTime = p->uin_HigtDot * p->f_Interval / 1000;
    p->f_FallTime = p->uin_FallDot * p->f_Interval / 1000;
    p->f_LowTime = p->uin_LowDot * p->f_Interval / 1000;

    /* 记录一个最大值 */
	for(i = 0; i < p->uin_RiseDot; i++)		        //默认10000点 协议可更改
	{
	    f1 = p->f_DcOffset;					                    //直流分量
        f2 = p->f_TrgVpp * ( i / (FP64)(p->uin_RiseDot - 1));	//三角波	幅值 * 当前点数/总点数
		f3 = p->f_SinVpp * sin(2 * mPI * (fw * i * fdt));       //正弦波    i / (采样频率 / 正弦波的频率) = i * 1 / 采样频率 * 正弦波的频率
		f  = f1 + f2 + f3;
		if ( f > fmax )
			fmax = f;
        //喂狗
	}
    
	/* 串口DAC输出的直流分量 */
    p->f_HwDcOffset = fmax;


    /* 计算实际的并口电压分量  */
	for(i = 0; i < p->uin_RiseDot; i++)		                //默认10000点 协议可更改
	{
		/* sinvpp 是峰值 所以 sinvpp*2 + trgvpp <= 0.5V*/
	    f1 = p->f_DcOffset;					                    //直流分量
        f2 = p->f_TrgVpp * ( i / (FP64)(p->uin_RiseDot - 1));	//三角波	幅值 * 当前点数/总点数
		f3 = p->f_SinVpp * sin(2 * mPI * (fw * i * fdt));       //正弦波
		f  = f1 + f2 + f3 - fmax;

		p->puin_RiseBuff[i] = Bsp_AD5546VoltToHex(f);					//并口电压分量 转换 为Hex
        //喂狗
	}
    
    /* 下降区的400个点 后续是否可调? */
	f1 = p->puin_RiseBuff[p->uin_RiseDot-1];               //最后一个点
	f2 = p->puin_RiseBuff[0];                              //最开始的点
	for(i = 0; i < p->uin_FallDot; i++)					//线性下降
	{
        f = (f2 - f1) * i / (p->uin_FallDot - 1.0);
        p->puin_FallBuff[i] = f + f1; 
	}

	p->b_GenerateWave = FALSE;
	return TRUE;
}

//==================================================================================
//| 函数名称 | Mod_SetSinVpp
//|----------|----------------------------------------------------------------------
//| 函数功能 | 设置正弦波幅值 并更新数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Wave:句柄 
//|          | f_SinVpp:正弦波幅值
//|          | b_WriteEPROM:TRUE 写入EPROM FALSE FALSE:只保存在内存中
//|----------|----------------------------------------------------------------------
//| 返回参数 | TRUE:成功 FALSE:失败
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_SetSinVpp(void * pv_Wave,FP32 f_SinVpp,BOOL b_WriteEPROM)
{
	Wave_t* p = pv_Wave;
    if( p != NULL)
    {
        if(f_SinVpp >= DEF_SINVPP_MIN && f_SinVpp <= DEF_SINVPP_MAX && 
           f_SinVpp + p->f_TrgVpp + p->f_DcOffset <= DEF_WAVE_MAX)
        {
            p->f_SinVpp = f_SinVpp;
            
            if(b_WriteEPROM == TRUE)
            {
                if(SaveToEeprom((INT32U)&p->f_SinVpp) != TRUE)
                    return FALSE;
            }
            p->b_GenerateWave = TRUE;
            return TRUE;
        }          
    }
    return FALSE;
}

//==================================================================================
//| 函数名称 | Mod_SetTrgVpp
//|----------|----------------------------------------------------------------------
//| 函数功能 | 设置三角波幅值 并更新数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Wave:句柄 
//|          | f_TrgVpp:三角波幅值
//|          | b_WriteEPROM:TRUE 写入EPROM FALSE FALSE:只保存在内存中
//|----------|----------------------------------------------------------------------
//| 返回参数 | TRUE:成功 FALSE:失败
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_SetTrgVpp(void * pv_Wave,FP32 f_TrgVpp,BOOL b_WriteEPROM)
{
	Wave_t* p = pv_Wave;
    if( p != NULL)
    {
        if(f_TrgVpp >= DEF_TRGVPP_MIN && f_TrgVpp <= DEF_TRGVPP_MAX &&
            p->f_SinVpp +f_TrgVpp + p->f_DcOffset <= DEF_WAVE_MAX)
        {
            p->f_TrgVpp = f_TrgVpp;
            
            if(b_WriteEPROM == TRUE)
            {
                if(SaveToEeprom((INT32U)&p->f_TrgVpp) != TRUE)
                    return FALSE;
            }
            p->b_GenerateWave = TRUE;
            return TRUE;
        }          
    }
    return FALSE;
}

//==================================================================================
//| 函数名称 | Mod_SetDcOffset
//|----------|----------------------------------------------------------------------
//| 函数功能 | 设置直流偏置电压 
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Wave:句柄 
//|          | f_DcOffset:直流偏置电压 
//|          | b_WriteEPROM:TRUE 写入EPROM FALSE FALSE:只保存在内存中
//|----------|----------------------------------------------------------------------
//| 返回参数 | TRUE:成功 FALSE:失败
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_SetDcOffset(void * pv_Wave,FP32 f_DcOffset,BOOL b_WriteEPROM)
{
	Wave_t* p = pv_Wave;
    if( p != NULL)
    {
        if(f_DcOffset >= DEF_DCOFFSET_MIN && f_DcOffset <= DEF_DCOFFSET_MAX && 
           p->f_SinVpp + p->f_TrgVpp + f_DcOffset <= DEF_WAVE_MAX)
        {
            p->f_DcOffset = f_DcOffset;
            
            if(b_WriteEPROM == TRUE)
            {
                if(SaveToEeprom((INT32U)&p->f_DcOffset) != TRUE)
                    return FALSE;
            }
            p->b_GenerateWave = TRUE;
            return TRUE;
        }          
    }
    return FALSE;
}

//==================================================================================
//| 函数名称 | Mod_SetSinFreq
//|----------|----------------------------------------------------------------------
//| 函数功能 | 设置正弦波频率
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Wave:句柄 
//|          | f_SinFreq:正弦波频率 
//|          | b_WriteEPROM:TRUE 写入EPROM FALSE FALSE:只保存在内存中
//|----------|----------------------------------------------------------------------
//| 返回参数 | TRUE:成功 FALSE:失败
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_SetSinFreq(void * pv_Wave,FP32 f_SinFreq,BOOL b_WriteEPROM)
{
	Wave_t* p = pv_Wave;
    if( p != NULL)
    {
        if(f_SinFreq >= DEF_SINFREQ_MIN && f_SinFreq <= DEF_SINFREQ_MAX)
        {
            p->f_SinFreq = f_SinFreq;
            
            if(b_WriteEPROM == TRUE)
            {
                if(SaveToEeprom((INT32U)&p->f_SinFreq) != TRUE)
                    return FALSE;
            }
            p->b_GenerateWave = TRUE;
            return TRUE;
        }          
    }
    return FALSE;
}

//==================================================================================
//| 函数名称 | Mod_SetSampleFreq
//|----------|----------------------------------------------------------------------
//| 函数功能 | 设置采样频率
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Wave:句柄 
//|          | f_SampleFreq:采样频率
//|          | b_WriteEPROM:TRUE 写入EPROM FALSE FALSE:只保存在内存中
//|----------|----------------------------------------------------------------------
//| 返回参数 | TRUE:成功 FALSE:失败
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_SetSampleFreq(void * pv_Wave,FP32 f_SampleFreq,BOOL b_WriteEPROM)
{
	Wave_t* p = pv_Wave;
    if( p != NULL)
    {
        if(f_SampleFreq >= DEF_SAMPLEFREQ_MIN && f_SampleFreq <= DEF_SAMPLEFREQ_MAX)
        {
            p->f_SampleFreq = f_SampleFreq;
            
            if(b_WriteEPROM == TRUE)
            {
                if(SaveToEeprom((INT32U)&p->f_SampleFreq) != TRUE)
                    return FALSE;
            }
            p->b_GenerateWave = TRUE;
            return TRUE;
        }          
    }
    return FALSE;
}

//==================================================================================
//| 函数名称 | Mod_SetSampleDot
//|----------|----------------------------------------------------------------------
//| 函数功能 | 设置采样频率
//|----------|----------------------------------------------------------------------
//| 输入参数 | pv_Wave:句柄 
//|          | uin_SampleDot:采样点数
//|          | b_WriteEPROM:TRUE 写入EPROM FALSE FALSE:只保存在内存中
//|----------|----------------------------------------------------------------------
//| 返回参数 | TRUE:成功 FALSE:失败
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Mod_SetSampleDot(void * pv_Wave,INT16U uin_SampleDot,BOOL b_WriteEPROM)
{
	Wave_t* p = pv_Wave;
    if( p != NULL)
    {
        if(uin_SampleDot >= DEF_SAMPLEDOT_MIN && uin_SampleDot <= DEF_SAMPLEDOT_MAX)
        {
            p->uin_SampleDot = uin_SampleDot;
            
            if(b_WriteEPROM == TRUE)
            {
                if(SaveToEeprom((INT32U)&p->uin_SampleDot) != TRUE)
                    return FALSE;
            }
            p->b_GenerateWave = TRUE;
            return TRUE;
        }          
    }
    return FALSE;
}

