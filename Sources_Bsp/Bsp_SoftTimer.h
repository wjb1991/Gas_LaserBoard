#ifndef __BSP_SOFTTIMER_H__
#define __BSP_SOFTTIMER_H__

#ifdef __cplusplus
extern "C" {
#endif


#define     DEF_SOFT_TIMER_MAX      20

typedef struct {
    BOOL    b_IsOneShot;        //单次模式
    INT32U  ul_Dly;             //第一次的定时时间    
    INT32U  ul_Period;          //周期定时时间
    void    (*cb_CallBack)(void* pv_Timer);         //回调函数
    
    BOOL    b_Run;              //是否运行
    BOOL    b_First;            //第一次运行
    INT32U  ul_Cnt;             //计数值
}SoftTimer_t;

void Bsp_DelayUs(INT32U us);
void Bsp_DelayMs(INT32U ms);

BOOL Bsp_SoftTimerReg(SoftTimer_t* pst_Timer);

BOOL Bsp_SoftTimerStart(SoftTimer_t* pst_Timer);

BOOL Bsp_SoftTimerStop(SoftTimer_t* pst_Timer);

void Bsp_SoftTimerPoll(void);

#ifdef __cplusplus
} /* extern "C" */
#endif  /* __cplusplus */


#endif
