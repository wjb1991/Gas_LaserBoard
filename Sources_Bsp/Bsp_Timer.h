#ifndef __BSP_TIMER_H__
#define	 __BSP_TIMER_H__

#include "Bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

//定时器0初始化
void Bsp_Time0Init(float timeUs);
//定时器0开始
void Bsp_Time0Start(void);
//定时器0结束
void Bsp_Time0Stop(void);
//定时器0开启中断
void Bsp_Time0IntEnable(void);
//定时器0关闭中断
void Bsp_Time0IntDisable(void);
//定时器0回掉函数注册
void Bsp_Time0HookRegister(void (*hook)(void));

//定时器1初始化
void Bsp_Time1Init(float timeUs);
//定时器1开始
void Bsp_Time1Start(void);
//定时器1结束
void Bsp_Time1Stop(void);
//定时器1开启中断
void Bsp_Time1IntEnable(void);
//定时器1关闭中断
void Bsp_Time1IntDisable(void);
//定时器1回掉函数注册
void Bsp_Time1HookRegister(void (*hook)(void));


#ifdef __cplusplus
} /* extern "C" */
#endif  /* __cplusplus */

#endif
