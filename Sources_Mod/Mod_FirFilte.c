#include "App_Include.h"

#define     DEF_FIR_DBG_EN           FALSE

#if (DEF_FIR_DBG_EN == TRUE)
    #define FIR_DBG(...)            do {                            \
                                            Bsp_Printf(__VA_ARGS__);    \
                                        }while(0)
#else
    #define FIR_DBG(...)
#endif

/* FIR接口结构体 */
typedef struct {
    FP32*   pf_InData;
    INT16U  uin_InLenth;
    FP32*   pf_OutData;
    INT16U  uin_OutLenth;
    INT16U  uin_Spand;
    FP32    af_Coeef[450];      //450*2 900个word
    INT16U  uin_Order;
}FirInterface;

#define CPU01_TO_CPU02_PASSMSG   0x0003FC00     // CPU01 TO CPU02 MSG RAM offsets for
#define CPU02_TO_CPU01_PASSMSG   0x0003F800     // CPU02 TO CPU01 MSG RAM offsets for

//==================================================================================================
//| 函数名称 | Mod_FIRFilter
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | FIR滤波器
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pf_Input: FIR采样点输入   uin_Lenth:采样点长度
//|          | pf_Factor: FIR系数        uin_Order: FIR阶数
//|          | uin_Avg: 平均滤波         uin_Spand: 结果数组缩放比例 5 = 1000个点缩放为200个点
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 |
//==================================================================================================
//#pragma CODE_SECTION(Mod_FIRFilterTwoCpu, ".TI.ramfunc");       //加载到Ram当中去运行 看情况使用
void Mod_FIRFilterTwoCpu(FP32 * pf_Input, INT16U uin_Lenth, const FP32* pf_Factor,INT16U uin_Order,INT16U uin_Avg,INT16U uin_Spand)
{
    INT16U i,j,k,l;
    FP32 f_tmp = 0;
    FP32 *pf_tmp = 0;

    /* Fir 双CPU同步计算  CPU2计算后半部分*/
    k = uin_Lenth / 2;          //CPU1计算的点数
    l = uin_Lenth / 2;          //CPU2计算的点数
    FirInterface* p = (void*)CPU01_TO_CPU02_PASSMSG;
    p->pf_InData = (void*)&pf_Input[k];
    p->uin_InLenth = l;
    p->uin_Order = uin_Order;
    p->pf_OutData = (void*)CPU02_TO_CPU01_PASSMSG;
    p->uin_Spand = uin_Spand;

    IPCLtoRFlagSet(IPC_FLAG10);                 //开启CPU2任务 计算FIR后半部分

    for(i = 0; i < uin_Order; i++)
        p->af_Coeef[i] = pf_Factor[i];          //将系数复制到MSG

    for(j = 0 ; j < k ; j++)                    //CPU1计算前半部分
    {
        pf_tmp = &pf_Input[j];
        f_tmp = 0;
        for(i = 0 ; i < uin_Order;i++)
        {
            f_tmp += pf_tmp[i]*pf_Factor[i];
        }
        pf_Input[j] = f_tmp;
    }

    /* 数组缩放 */
    k /= uin_Spand;
    l /= uin_Spand;

    if(uin_Spand > 1)
    {
        for(i = 0; i < k; i++)
        {
            pf_tmp = &pf_Input[i*uin_Spand];
            f_tmp = 0;
            for(j = 0 ; j < uin_Spand;j++)
            {
                f_tmp += pf_tmp[j];
            }
            pf_Input[i] = f_tmp/uin_Spand;
        }
    }

    while(IPCLtoRFlagBusy(IPC_FLAG10) != 0){
        Bsp_AlarmLed(eLedOn);
    }   //等待CPU2计算完成

    Bsp_AlarmLed(eLedOff);

    /* 复制CPU2计算的结果到 后半段*/
    pf_tmp = (void*)CPU02_TO_CPU01_PASSMSG;
    for(i = 0 ; i < l; i++)
    {
        pf_Input[i + k]= pf_tmp[i];
    }
}
//==================================================================================================
//| 函数名称 | Mod_FIRFilter
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | FIR滤波器
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pf_Input: FIR采样点输入   uin_Lenth:采样点长度
//|          | pf_Coeff: FIR系数         uin_Order: FIR阶数
//|          | uin_Avg: 每个点平均滤波   uin_Spand: 结果数组缩放比例 5 = 1000个点缩放为200个点
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
//#pragma CODE_SECTION(Mod_FIRFilter, ".TI.ramfunc");       		//加载到Ram当中去运行 看情况使用
void Mod_FIRFilter(FP32 * pf_Input, INT16U uin_Lenth, const FP32* pf_Coeff,INT16U uin_Order,INT16U uin_Avg,INT16U uin_Spand)
{
	//单核版本的FIR滤波器
#if 1
	INT16U len = uin_Lenth / uin_Spand;							//结果数组的长度
	INT16U n = (uin_Lenth - uin_Order) / uin_Spand;				//实际计算的点数
	INT16U i,j,k;
	FP32*  pf_tmp = NULL;
	FP32   f_tmp = 0.0f;

	for(i = 0; i < uin_Order ; i++ )
	    FIR_DBG(">>FIR_DBG: pf_Coeff[%d] = %f\r\n",i,pf_Coeff[i]);


	/* Fir计算 */
	for(i = 0 ; i < n; i++)										//实际计算点数循环
	{
		f_tmp = 0.0f;
		for(j = 0; j < uin_Avg; j++)							//向右N个点平均
		{
			pf_tmp = &pf_Input[i*uin_Spand+j];					//存储起始点的位置
			for(k = 0; k < uin_Order; k++)
			{
				f_tmp += pf_tmp[k]*pf_Coeff[k];					//算完第一个点之后继续累加第二个点的值
			}
		}
		pf_Input[i] = f_tmp / uin_Avg;							//计算平均
		//FIR_DBG(">>FIR_DBG: pf_Input[%d] = %f\r\n",i,pf_Input[i]);
	}

	/* 填充后面几个无效点 */
	for(i = n; i < len ; i++)
	{
		pf_Input[i] = pf_Input[n-1];
        //FIR_DBG(">>FIR_DBG: pf_Input[%d] = %f\r\n",i,pf_Input[i]);
	}
#else
    INT16U i,j;
    FP32 f_tmp = 0;
    FP32 *pf_tmp = 0;

    /* Fir */
    for(j = 0 ; j < uin_Lenth - uin_Order; j++)
    {
        pf_tmp = &pf_Input[j];
        f_tmp = 0;
        for(i = 0 ; i < uin_Order;i++)
        {
            f_tmp += pf_tmp[i]*pf_Coeff[i];
        }
        pf_Input[j] = f_tmp;
    }

    for(i = uin_Lenth - uin_Order; i < uin_Lenth; i++)
    {
        pf_Input[i]= pf_Input[uin_Lenth - uin_Order -1];
    }

    if(uin_Avg > 1)
    {
        for( i = 0; i<(uin_Lenth - uin_Avg); i++)
        {
            pf_tmp = &pf_Input[i];
            f_tmp = 0;
            for(j = 0; j < uin_Avg; j++)
            {
                f_tmp += pf_tmp[j];
            }
            pf_Input[i] = f_tmp/uin_Avg;
        }
    }

    if(uin_Spand > 1)
    {
        for(i = 0; i < uin_Lenth / uin_Spand; i++)
        {
            pf_tmp = &pf_Input[i*uin_Spand];
            f_tmp = 0;
            for(j = 0 ; j < uin_Spand;j++)
            {
                f_tmp += pf_tmp[j];
            }
            pf_Input[i] = f_tmp/uin_Spand;
        }
    }
#endif
}

#if 0
//==================================================================================================
//| 函数名称 | Mod_FIRFilter
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | FIR滤波器
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | pf_Input: FIR采样点输入   uin_Lenth:采样点长度
//|          | pf_Factor: FIR系数        uin_Order: FIR阶数
//|          | uin_Avg: 平均滤波         uin_Spand: 结果数组缩放比例 5 = 1000个点缩放为200个点
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 |
//==================================================================================================
void Mod_FIRFilterDsp(FP32 * pf_Input, INT16U uin_Lenth, const FP32* pf_Factor,INT16U uin_Order,INT16U uin_Avg,INT16U uin_Spand)
{
    INT16U i,j;
    FP32 f_tmp = 0;
    FP32* pf_tmp = NULL;

    float dbuffer[300];
    float yn;

    FIR_FP  firFP = FIR_FP_DEFAULTS;
    FIR_FP_Handle hnd_firFP = &firFP;

    // FIR Generic Filter Initialization
    hnd_firFP->order       = uin_Order;
    hnd_firFP->dbuffer_ptr = dbuffer;
    hnd_firFP->coeff_ptr   = (float *)pf_Factor;
    hnd_firFP->init(hnd_firFP);

    for(i=0; i < uin_Lenth; i++)
    {
        hnd_firFP->input = pf_Input[i];
        hnd_firFP->calc(&firFP);
        yn = hnd_firFP->output;
        pf_Input[i] = yn;
    }

    /* 填充最后几个无法正常Fir的点 */
    for(i = uin_Lenth - uin_Order; i < uin_Lenth; i++)
    {
        pf_Input[i]= pf_Input[uin_Lenth - uin_Order -1];
    }

    /* 平均滤波 */
    //mpy_SP_RVxRV_2(y, x1, x2, SIZE);
    if(uin_Avg > 1)
    {
        for( i = 0; i<(uin_Lenth - uin_Avg); i++)
        {
            pf_tmp = &pf_Input[i];
            f_tmp = 0;
            for(j = 0; j < uin_Avg; j++)
            {
                f_tmp += pf_tmp[j];
            }
            pf_Input[i] = f_tmp/uin_Avg;
        }
    }

    /* 数组缩放 */
    if(uin_Spand > 1)
    {
        for(i = 0; i < uin_Lenth / uin_Spand; i++)
        {
            pf_tmp = &pf_Input[i*uin_Spand];
            f_tmp = 0;
            for(j = 0 ; j < uin_Spand;j++)
            {
                f_tmp += pf_tmp[j];
            }
            pf_Input[i] = f_tmp/uin_Spand;
        }
    }
}
#endif

