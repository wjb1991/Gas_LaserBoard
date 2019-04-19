#include "App_Include.h"

#define     DEF_MEASURE_DBG_EN           TRUE

#if (DEF_MEASURE_DBG_EN == TRUE)
    #define MEASURE_DBG(...)            do {                            \
                                            Bsp_Printf(__VA_ARGS__);    \
                                        }while(0)
#else
    #define MEASURE_DBG(...)
#endif

void Mod_MeasureGasCO2Notification(FP32 f_Concentration);
void Mod_MeasureGasCONotification(FP32 f_Concentration);
void TimeOut(void* timer);

Measure_t st_Measure = {
    e_MeasureIdle,                  /* 测试状态 */
	FALSE,							/* 是否静态测量 */
    10,                             /* 死区时间 */
    1000,                           /* 测试时间 */
    2,								/* 无效点 */
    5,								/* 有效点 */
    {{0},0,DEF_SAMPLE_DOT_MAX},     /* CO采样点 */
    {{0},0,DEF_SAMPLE_DOT_MAX},     /* CO2采样点 */
	0.0,							/* 测试时间窗口内的平均浓度 */
	0.0,							/* 测试时间窗口内的平均浓度 */
};


SoftTimer_t st_TimeOut = {
    TRUE,                   //单次模式
    5000,                   //第一次的定时时间
    0,                      //周期定时时间
    &TimeOut     //回调函数
};

static void* s_Msg;

static void PostMsg(void* pv_Msg)
{
	s_Msg = pv_Msg;
}

static void* PendMeg(void)
{
    void * pv_Msg;
	DINT;
	pv_Msg = s_Msg;
    if(pv_Msg != NULL)
    	s_Msg = NULL;
	EINT;
    return pv_Msg;
}


void TimeOut(void* timer)
{
    switch (st_Measure.e_State)
    {
    case e_MeasureWait:
        PostMsg((void*)e_MeasureTimeOut);	//等待超时
        Mod_MeasurePoll(&st_Measure);       //立即处理
        break;
    case e_MeasureDead:
        PostMsg((void*)e_MeasureSample);	//死区结束开始采样
        Mod_MeasurePoll(&st_Measure);       //立即处理
        break;
    case e_MeasureSample:
        PostMsg((void*)e_MeasureCal);		//采样结束开始计算  等待到主程序处理
        break;
    }
}

static void StartTimeOutCheck(INT32U TimeOutMs)
{
    st_TimeOut.ul_Dly = TimeOutMs;
    Bsp_SoftTimerStart(&st_TimeOut);
}

static void StopTimeOutCheck(void)
{
    Bsp_SoftTimerStop(&st_TimeOut);
}


static void InitTimeOutCheck(void)
{
    Bsp_SoftTimerReg(&st_TimeOut);
}

static void SortBuff(FP32* pf_Buff,INT16U uin_Len)
{
	INT16U i,j;
	FP32 t;
	for(i = 0; i < uin_Len-1; i++)
	{
		for(j = i+1; j < uin_Len; j++)
		{
			if(pf_Buff[j] > pf_Buff[i])
			{
				t = pf_Buff[j];				//交换两个数
				pf_Buff[j] = pf_Buff[i];
				pf_Buff[i] = t;
			}
		}
	}
}

static FP32 WindowFilter(FP32* pf_Buff,INT16U uin_Use,INT16U uin_InvalidDots, INT16U uin_ActiveDots)
{
    INT16U  i;
    FP32 f = 0;
    FP32 af_Buff[DEF_SAMPLE_DOT_MAX] = {0};

    //memcpy(af_Buff,pf_Buff,sizeof(af_Buff[DEF_SAMPLE_DOT_MAX]));

    for(i = 0; i < DEF_SAMPLE_DOT_MAX; i++)
    {
        af_Buff[i] = pf_Buff[i];
    }


    SortBuff(af_Buff,uin_Use);

    /* 计算除去n1个最大值之后 剩余值中n2个有效值的平均值 */
    if (uin_Use > uin_InvalidDots)
    {
        /* 总点数 大于无效点数 */
        INT16U num = uin_Use - uin_InvalidDots;		//计算去掉N1个无效点后剩余的样品点数

        if (num  > uin_ActiveDots)					//判断剩余点数是否大于N2个有效点数
            num  = uin_ActiveDots;

        for( i = 0; i <	num; i++)
            f += af_Buff[uin_InvalidDots + i];	    //求和
        f /= num;								    //算平均
    }
    return f;
}

void Mod_MeasureDoStaticMeasure(Measure_t* pst_Meas)
{
	pst_Meas->b_IsStaticMeasure = TRUE;
    Mod_GasMeasDoDiffMeasure(&st_GasMeasForIr);                 //开始差分采样
	PostMsg((void*)e_MeasureSample);	//直接开始采样
}

void Mod_MeasureDoDynamicMeasure(Measure_t* pst_Meas)
{
	pst_Meas->b_IsStaticMeasure = FALSE;
    Mod_GasMeasDoDiffBackground(&st_GasMeasForIr);          //开始差分背景
	PostMsg((void*)e_MeasureIdle);		//切换到空闲模式
}


void Mod_MeasureInit(Measure_t* pst_Meas)
{
	InitTimeOutCheck();
    st_GasCO2.cb_Notification = Mod_MeasureGasCO2Notification;
    st_GasCO.cb_Notification  = Mod_MeasureGasCONotification;
}

void Mod_MeasurePoll(Measure_t* pst_Meas)
{
	INT32U i;
    INT32U  e_Msg = (INT32U)PendMeg();
    //FP32 f;
    switch(e_Msg)
    {
    case e_MeasureWait:
        pst_Meas->e_State = e_MeasureWait;
        StartTimeOutCheck(5000);                                        //开始超时检测 10Km/h 经过所需时间200ms 180Km/h经过所需时间10ms/h
        Mod_GasMeasDoDiffMeasure(&st_GasMeasForIr);                  //开始差分采样

        MEASURE_DBG(">>MEASURE DBG:   车辆进入 准备开始测量\r\n");
        break;
    case e_MeasureDead:
		pst_Meas->e_State = e_MeasureDead;
        StopTimeOutCheck();                                     	    //停止超时检测
        if ( pst_Meas->ul_DeadTime > 0 )
        {
			StartTimeOutCheck(pst_Meas->ul_DeadTime);                   //开始死区记时
	        MEASURE_DBG(">>MEASURE DBG:   车辆离去 死区延时\r\n");
        }
        else
        {
			PostMsg((void*)e_MeasureSample);	                        //死区结束开始采样
			Mod_MeasurePoll(pst_Meas);                                  //递归注意
        }
        break;

    case e_MeasureSample:
        pst_Meas->e_State = e_MeasureSample;
        pst_Meas->st_SampleCO.ul_Len = 0;
        pst_Meas->st_SampleCO2.ul_Len = 0;

        StopTimeOutCheck();                                     	//停止超时检测
        if ( pst_Meas->ul_MesureTime > 0 )
        {
			StartTimeOutCheck(pst_Meas->ul_MesureTime);            	//开始测量计时
        }
        else
        {
            PostMsg((void*)e_MeasureCal);							//死区结束开始采样
        }
        MEASURE_DBG(">>MEASURE DBG:   开始采样\r\n");
        break;
    case e_MeasureCal:
        MEASURE_DBG(">>MEASURE DBG:   开启计算\r\n");
        if(st_Measure.b_IsStaticMeasure == TRUE)
        {
			PostMsg((void*)e_MeasureSample);						//继续采样开始采样
        }
        else
        {
			Mod_GasMeasDoDiffBackground(&st_GasMeasForIr);          //开始差分背景
			pst_Meas->e_State = e_MeasureIdle;
        }

        pst_Meas->f_CO2 = WindowFilter (pst_Meas->st_SampleCO2.af_Buff,
                                        pst_Meas->st_SampleCO2.ul_Len,
                                        pst_Meas->uin_InvalidDots,
                                        pst_Meas->uin_ActiveDots);

        pst_Meas->f_CO  = WindowFilter (pst_Meas->st_SampleCO.af_Buff,
                                        pst_Meas->st_SampleCO.ul_Len,
                                        pst_Meas->uin_InvalidDots,
                                        pst_Meas->uin_ActiveDots);

#if 1
        {
            MEASURE_DBG(">>MEASURE DBG:================================================\r\n");

            MEASURE_DBG(">>MEASURE DBG: 一次测量完成\r\n");

            MEASURE_DBG(">> CO2 %d个采样点\r\n",(INT16U)pst_Meas->st_SampleCO2.ul_Len);
            for(i = 0; i < pst_Meas->st_SampleCO2.ul_Len; i++)
            {
            	MEASURE_DBG(">> CO2[%02d]: %f\r\n", (INT16U)i, pst_Meas->st_SampleCO2.af_Buff[i]);
            }

            MEASURE_DBG(">> CO->%d个采样点\r\n",(INT16U)pst_Meas->st_SampleCO.ul_Len);
            for(i = 0; i < pst_Meas->st_SampleCO.ul_Len; i++)
            {
            	MEASURE_DBG(">> CO[%02d]: %f\r\n", (INT16U)i, pst_Meas->st_SampleCO.af_Buff[i]);
            }
            MEASURE_DBG(">> CO2平均值: %f\r\n",pst_Meas->f_CO2 );
            MEASURE_DBG(">> CO平均值: %f\r\n",pst_Meas->f_CO);

            MEASURE_DBG(">>MEASURE DBG:   测量任务结束\r\n");
            MEASURE_DBG(">>MEASURE DBG:================================================\r\n");

        }
#endif

        break;
    case e_MeasureTimeOut:
        /* 恢复正常模式  */
        MEASURE_DBG(">>MEASURE DBG:   测量任务超时\r\n");

        StopTimeOutCheck();                                     //停止超时检测

        Mod_GasMeasDoDiffBackground(&st_GasMeasForIr);          //开始差分背景

        pst_Meas->e_State = e_MeasureIdle;
        break;

    default:
        break;
    }
}
void Mod_MeasureGasCO2Notification(FP32 f_Concentration)
{
    if(st_Measure.e_State == e_MeasureSample)
    {
        if(st_Measure.st_SampleCO2.ul_Len < st_Measure.st_SampleCO2.ul_Size)
        {
            st_Measure.st_SampleCO2.af_Buff[st_Measure.st_SampleCO2.ul_Len++] = f_Concentration;
        }
    }
}

void Mod_MeasureGasCONotification(FP32 f_Concentration)
{
    if(st_Measure.e_State == e_MeasureSample)
    {
        if(st_Measure.st_SampleCO.ul_Len < st_Measure.st_SampleCO.ul_Size)
        {
            st_Measure.st_SampleCO.af_Buff[st_Measure.st_SampleCO.ul_Len++] = f_Concentration;
        }
    }
}

void Bsp_GpioEvent(INT32U ul_Pin, BOOL b_IsRising)
{
#if 1
    if(ul_Pin == 55 && b_IsRising == TRUE )         //车头挡住第一个传感器
    {
        if(st_Measure.e_State == e_MeasureIdle)
        {
            //Mod_LaserDoStop(&st_Laser);           //停止激光器扫描
            PostMsg((void*)e_MeasureWait);
            Mod_MeasurePoll(&st_Measure);           //立即处理
        }
    }
    else if(ul_Pin == 56  && b_IsRising == FALSE )   //车位离开第二个传感器
    {
        if(st_Measure.e_State == e_MeasureWait)
        {
            //Mod_LaserExitIdle(&st_Laser);         //退出空闲模式 开启激光器扫描
            //Mod_TransmissionClear();             //透过率清楚
            PostMsg((void*)e_MeasureDead);
            Mod_MeasurePoll(&st_Measure);           //立即处理
        }
    }
#else
    //MEASURE_DBG("引脚:0x%x,边沿:%x\r\n", ul_Pin, b_IsRising);

	char buff[100];
    sprintf(buff,"引脚:0x%x,边沿:%x\r\n", ul_Pin, b_IsRising);
    s_SendStr(buff);
#endif
}





