//==================================================================================================
//| 文件名称 | Bsp.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Bsp.c所对应的头文件
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版V1.0
//|  V1.01   | 2018.10.31  |  wjb      | 在初版上添加注释 添加开关中断 和 打印调试 V1.01    
//==================================================================================================
#ifndef  __BSP_H__
#define  __BSP_H__


#ifdef __cplusplus
extern  "C" {                                  /* See Note #1.                                         */
#endif
  
#include  <stdarg.h>
#include  <stdint.h>
#include  <stdbool.h>
#include  <stdio.h>
#include  <string.h>
#include  <stdlib.h>                        //atof转换函数
#include  <math.h>


#include  "F28x_Project.h"
#include  "inc/hw_memmap.h"
#include  "inc/hw_types.h"
#include  "inc/hw_ints.h"
#include  "driverlib/interrupt.h"
#include  "driverlib/sysctl.h"
#include  "driverlib/systick.h"
#include  "driverlib/uart.h"
#include  "driverlib/usb.h"
#include  "driverlib/usb_hal.h"
#include  "usblib/usblib.h"
#include  "usblib/host/usbhost.h"
#include  "usbhspe.h"

#include  "utils/uartstdio.h"



#ifdef      OS_SUPPORT
#include  "bsp_os.h"
#include  <cpu_core.h>
#include  <cpu.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <os_app_hooks.h>
#endif
  
  
#include  "Bsp_DataType.h"
#include  "Bsp_Converter.h"
#include  "Bsp_SoftTimer.h"

#include  "Bsp_Timer.h"
#include  "Bsp_Usart.h"



#if 0
#include  "bsp_sram.h"
#include  "bsp_led.h"

#include  "bsp_clock.h"

#include  "bsp_spi.h"
#include  "bsp_i2c.h"
#include  "bsp_at24c512.h"

#include  "Bsp_Gpio.h"
#include  "Bsp_Dma.h"
  
#include  "Bsp_W25Qxx.h"
#include  "Bsp_LTC1867.h"

/*__CMSIS_IAR_H__ 中有常用的宏定义 */
  
void Bsp_Init(void);

void Bsp_Init(void);

__STATIC_INLINE void Bsp_Nop(void){;}
__STATIC_INLINE void Bsp_IntDis(void){ DINT }
__STATIC_INLINE void Bsp_IntEn(void){ EINT }
#endif

void Bsp_Init(void);

#define	 Bsp_Printf		Bsp_UartPrintf

//#define	TRACE_DBG 	    //Bsp_UartPrintf

#ifdef __cplusplus
}                                              /* End of 'extern'al C lang linkage.                    */
#endif

#endif                                                          /* End of module include.                               */

