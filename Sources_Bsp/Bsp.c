//==================================================================================================
//| 文件名称 | Bsp.c
//|----------|--------------------------------------------------------------------------------------
//| 文件描述 | Bsp.c 板级基本功能的实现
//|----------|--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|----------|-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版
//==================================================================================================
#include  "Bsp.h"


void  Bsp_Init (void)
{

    Bsp_DelayMs(100);

    Bsp_GpioInit();
    Bsp_SramInit();
    Bsp_SpibInit();       // init SPI

    Bsp_AD5546Init();   //并口DAC
    Bsp_AD5663Init();   //SPI DAC
    Bsp_AD7622Init();   //并口ADC

    /* 定时器1 软件定时器 系统时钟定时器 */
    Bsp_Time1Init(1000);
    Bsp_Time1IntEnable();
    Bsp_Time1Start();
    Bsp_Time1HookRegister(Bsp_SoftTimerPoll);


    /* 调试口  RS485 */
    COM1.ul_BaudRate   = 230400;
    COM1.ul_WordLength = UART_WORDLENGTH_8B;
    COM1.ul_StopBits   = UART_STOPBITS_1;
    COM1.ul_Parity     = UART_PARITY_NONE;
    COM1.ul_HwFlowCtl  = UART_HWCONTROL_NONE;
    COM1.ul_Mode       = UART_MODE_TX_RX;
    Bsp_UartOpen(&COM1);

    /* STDBUS 连接主板 */
    COM2.ul_BaudRate   = 230400;
    COM2.ul_WordLength = UART_WORDLENGTH_8B;
    COM2.ul_StopBits   = UART_STOPBITS_1;
    COM2.ul_Parity     = UART_PARITY_NONE;
    COM2.ul_HwFlowCtl  = UART_HWCONTROL_NONE;
    COM2.ul_Mode       = UART_MODE_TX_RX;
    Bsp_UartOpen(&COM2);

    /* STDBUS 连接上位机 */
    COM3.ul_BaudRate   = 230400;
    COM3.ul_WordLength = UART_WORDLENGTH_8B;
    COM3.ul_StopBits   = UART_STOPBITS_1;
    COM3.ul_Parity     = UART_PARITY_NONE;
    COM3.ul_HwFlowCtl  = UART_HWCONTROL_NONE;
    COM3.ul_Mode       = UART_MODE_TX_RX;
    Bsp_UartOpen(&COM3);


    Bsp_Pga2A0(1);
    Bsp_Pga2A1(1);
}

