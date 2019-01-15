//==================================================================================
//| 文件名称 | bsp_usart.c
//|----------|----------------------------------------------------------------------
//| 文件功能 | 向上层提供统一的接口 需根据不同的平台更改底层
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#include   "Bsp.h"

#define     DEF_UART_CONFIG         115200,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_NONE,UART_HWCONTROL_NONE,UART_MODE_TX_RX
#define     DEF_UART_HOOK           0,0,0,0
#define     DEF_UART_BUFF_SIZE      100

static INT8U    auch_RxBuff[4][DEF_UART_BUFF_SIZE] = {0};
static INT8U 	auch_PrintfBuff[100] = {0};
/**/
Dev_SerialPort COM1 = {(const INT8S*)"COM1",                              //端口名
                        DEF_UART_CONFIG,                    //默认配置
                        
                        NULL,                               //发送缓冲区配置
                        0,
                        0,
                        
                        &auch_RxBuff[0][0],                 //接收缓冲区配置
                        DEF_UART_BUFF_SIZE,
                        0,                     
                        
                        DEF_UART_HOOK,                      //回调函数
						(void*)&SciaRegs};                 //底层句柄

Dev_SerialPort COM2 = {(const INT8S*)"COM2",                              //端口名
                        DEF_UART_CONFIG,                    //默认配置
                        
                        NULL,                               //发送缓冲区配置
                        0,
                        0,
                        
                        &auch_RxBuff[1][0],                 //接收缓冲区配置
                        DEF_UART_BUFF_SIZE,
                        0,                     
                        
                        DEF_UART_HOOK,                      //回调函数
						(void*)&ScibRegs};                 //底层句柄

Dev_SerialPort COM3 = {(const INT8S*)"COM3",                              //端口名
                        DEF_UART_CONFIG,                    //默认配置
                        
                        NULL,                               //发送缓冲区配置
                        0,
                        0,
                        
                        &auch_RxBuff[2][0],                 //接收缓冲区配置
                        DEF_UART_BUFF_SIZE,
                        0,                     
                        
                        DEF_UART_HOOK,                      //回调函数
                        (void*)&ScicRegs};                 //底层句柄

Dev_SerialPort COM4 = {(const INT8S*)"COM4",                              //端口名
                        DEF_UART_CONFIG,                    //默认配置
                        
                        NULL,                               //发送缓冲区配置
                        0,
                        0,
                        
                        &auch_RxBuff[3][0],                 //接收缓冲区配置
                        DEF_UART_BUFF_SIZE,
                        0,                     
                        
                        DEF_UART_HOOK,                      //回调函数
                        (void*)&ScidRegs};                 //底层句柄

void HAL_UART_MspInit(struct SCI_REGS *huart);
void HAL_UART_MspDeInit(struct SCI_REGS *huart);

                        
void Bsp_UsartRxEnable(Dev_SerialPort* pst_Dev)
{
	struct SCI_REGS * UartHandle = pst_Dev->pv_UartHandle;
    /* 开启接受完成中断 */  
	if(UartHandle == &SciaRegs)
	{
		PieCtrlRegs.PIEIER9.bit.INTx1 = 1;          // Enable PIE Group 9 INT1
	}
	else if(UartHandle == &ScibRegs)
	{
		PieCtrlRegs.PIEIER9.bit.INTx3 = 1;          // Enable PIE Group 9 INT3
	}
	else if(UartHandle == &ScicRegs)
	{
		PieCtrlRegs.PIEIER8.bit.INTx5 = 1;          // Enable PIE Group 8 INT5
	}
	else if(UartHandle == &ScidRegs)
	{
		PieCtrlRegs.PIEIER8.bit.INTx7 = 1;          // Enable PIE Group 8 INT7
	}
}

void Bsp_UsartRxDisable(Dev_SerialPort* pst_Dev)
{
	struct SCI_REGS * UartHandle = pst_Dev->pv_UartHandle;
    /* 开启接受完成中断 */
	if(UartHandle == &SciaRegs)
	{
		PieCtrlRegs.PIEIER9.bit.INTx1 = 0;          // Enable PIE Group 9 INT1
	}
	else if(UartHandle == &ScibRegs)
	{
		PieCtrlRegs.PIEIER9.bit.INTx3 = 0;          // Enable PIE Group 9 INT3
	}
	else if(UartHandle == &ScicRegs)
	{
		PieCtrlRegs.PIEIER8.bit.INTx5 = 0;          // Enable PIE Group 8 INT5
	}
	else if(UartHandle == &ScidRegs)
	{
		PieCtrlRegs.PIEIER8.bit.INTx7 = 0;          // Enable PIE Group 8 INT7
	}
}

void Bsp_UsartTxEnable(Dev_SerialPort* pst_Dev)
{
	struct SCI_REGS * UartHandle = pst_Dev->pv_UartHandle;
    /* 开启发送完成中断 */
	if(UartHandle == &SciaRegs)
	{
		PieCtrlRegs.PIEIER9.bit.INTx2 = 1;          // Enable PIE Group 9 INT1
	}
	else if(UartHandle == &ScibRegs)
	{
		PieCtrlRegs.PIEIER9.bit.INTx4 = 1;          // Enable PIE Group 9 INT3
	}
	else if(UartHandle == &ScicRegs)
	{
		PieCtrlRegs.PIEIER8.bit.INTx6 = 1;          // Enable PIE Group 8 INT5
	}
	else if(UartHandle == &ScidRegs)
	{
		PieCtrlRegs.PIEIER8.bit.INTx8 = 1;          // Enable PIE Group 8 INT7
	}
}

void Bsp_UsartTxDisable(Dev_SerialPort* pst_Dev)
{
	struct SCI_REGS * UartHandle = pst_Dev->pv_UartHandle;
    /* 关闭发送完成中断 */
	if(UartHandle == &SciaRegs)
	{
		PieCtrlRegs.PIEIER9.bit.INTx2 = 0;          // Enable PIE Group 9 INT1
	}
	else if(UartHandle == &ScibRegs)
	{
		PieCtrlRegs.PIEIER9.bit.INTx4 = 0;          // Enable PIE Group 9 INT3
	}
	else if(UartHandle == &ScicRegs)
	{
		PieCtrlRegs.PIEIER8.bit.INTx6 = 0;          // Enable PIE Group 8 INT5
	}
	else if(UartHandle == &ScidRegs)
	{
		PieCtrlRegs.PIEIER8.bit.INTx8 = 0;          // Enable PIE Group 8 INT7
	}
}

__STATIC_INLINE void Bsp_SendOneByte(Dev_SerialPort* pst_Dev, INT8U byte)
{
    struct SCI_REGS* UartHandle = pst_Dev->pv_UartHandle;
    while (UartHandle->SCIFFTX.bit.TXFFST != 0) {}
    UartHandle->SCITXBUF.all = byte;
}

__STATIC_INLINE INT8U Bsp_RecvOneByte(Dev_SerialPort* pst_Dev)
{
   struct SCI_REGS* UartHandle = pst_Dev->pv_UartHandle;
   //while(UartHandle.SCIFFRX.bit.RXFFST == 0) {} // wait for empty state
   return ((INT8U)UartHandle->SCIRXBUF.all);
}

__STATIC_INLINE void USARTxRecv_IRQHandler(Dev_SerialPort* pst_Dev)
{
    if(pst_Dev->cb_RecvReady != NULL)
    {
        INT8U tmp = Bsp_RecvOneByte(pst_Dev);
        pst_Dev->cb_RecvReady(pst_Dev, &tmp, 1);
    }
}
 __STATIC_INLINE void USARTxSend_IRQHandler(Dev_SerialPort* pst_Dev)
{
    if( pst_Dev->uin_TxCount < pst_Dev->uin_TxLen)
    {
        Bsp_SendOneByte(pst_Dev, pst_Dev->puch_TxBuff[pst_Dev->uin_TxCount++]);
    }
    else
    {
        pst_Dev->uin_TxCount = 0;
        pst_Dev->uin_TxLen = 0;
        if(pst_Dev->cb_SendComplete != NULL)
        {
            pst_Dev->cb_SendComplete(pst_Dev);
        }
    }
}

//==================================================================================
//| 函数名称 | USART1_IRQHandler
//|----------|----------------------------------------------------------------------
//| 函数功能 | USART1串口中断处理函数
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
interrupt void Bsp_SciaRecvIsr(void)
{
	struct SCI_REGS * UartHandle = COM1.pv_UartHandle;

    USARTxRecv_IRQHandler(&COM1);

    UartHandle->SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    UartHandle->SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

interrupt void Bsp_SciaSendIsr(void)
{
	struct SCI_REGS * UartHandle = COM1.pv_UartHandle;

    USARTxSend_IRQHandler(&COM1);

    UartHandle->SCIFFTX.bit.TXFFINTCLR=1;  // Clear SCI Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
//==================================================================================
//| 函数名称 | USART2_IRQHandler
//|----------|----------------------------------------------------------------------
//| 函数功能 | USART2串口中断处理函数
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
interrupt void Bsp_ScibRecvIsr(void)
{
	struct SCI_REGS * UartHandle = COM2.pv_UartHandle;

    USARTxRecv_IRQHandler(&COM2);

    UartHandle->SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    UartHandle->SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

interrupt void Bsp_ScibSendIsr(void)
{
	struct SCI_REGS * UartHandle = COM2.pv_UartHandle;

    USARTxSend_IRQHandler(&COM2);

    UartHandle->SCIFFTX.bit.TXFFINTCLR=1;  // Clear SCI Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
//==================================================================================
//| 函数名称 | USART3_IRQHandler
//|----------|----------------------------------------------------------------------
//| 函数功能 | USART3串口中断处理函数
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
interrupt void Bsp_ScicRecvIsr(void)
{
	struct SCI_REGS * UartHandle = COM3.pv_UartHandle;

    USARTxRecv_IRQHandler(&COM3);

    UartHandle->SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    UartHandle->SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

interrupt void Bsp_ScicSendIsr(void)
{
	struct SCI_REGS * UartHandle = COM3.pv_UartHandle;

    USARTxSend_IRQHandler(&COM3);

    UartHandle->SCIFFTX.bit.TXFFINTCLR=1;  // Clear SCI Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}
//==================================================================================
//| 函数名称 | UART4_IRQHandler
//|----------|----------------------------------------------------------------------
//| 函数功能 | UART4串口中断处理函数
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
interrupt void Bsp_ScidRecvIsr(void)
{
	struct SCI_REGS * UartHandle = COM4.pv_UartHandle;

    USARTxRecv_IRQHandler(&COM4);

    UartHandle->SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    UartHandle->SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

interrupt void Bsp_ScidSendIsr(void)
{
	struct SCI_REGS * UartHandle = COM4.pv_UartHandle;

    USARTxSend_IRQHandler(&COM4);

    UartHandle->SCIFFTX.bit.TXFFINTCLR=1;  // Clear SCI Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
//==================================================================================
//| 函数名称 | Bsp_UartOpen
//|----------|----------------------------------------------------------------------
//| 函数功能 | 配置并打开串口
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:串口设备结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE 打开失败, TRUE 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartOpen(Dev_SerialPort* pst_Dev)
{
    struct SCI_REGS* UartHandle = pst_Dev->pv_UartHandle;
    
    HAL_UART_MspInit(pst_Dev->pv_UartHandle);

    UartHandle->SCICCR.all = 0x0007; // 1 stop bit,  No loopback
                                    // No parity,8 char bits,
                                    // async mode, idle-line protocol
    UartHandle->SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
                                     // Disable RX ERR, SLEEP, TXWAKE
    UartHandle->SCICTL2.all = 0x0003;
    UartHandle->SCICTL2.bit.TXINTENA = 1;
    UartHandle->SCICTL2.bit.RXBKINTENA = 1;

    //
    // SCIA at 9600 baud
    // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
    // @LSPCLK = 30 MHz (120 MHz SYSCLK) HBAUD = 0x01 and LBAUD = 0x86.
    //
	INT16U uin_Temp = (INT16U)(200e6 / 4 / pst_Dev->ul_BaudRate / 8 - 1); //Baud = LSPCLK / ((BRR + 1) *8)

    UartHandle->SCIHBAUD.all = uin_Temp >> 8;
    UartHandle->SCILBAUD.all = uin_Temp & 0xff;

    UartHandle->SCICTL1.all = 0x0023;  // Relinquish SCI from Reset

    UartHandle->SCIFFTX.all = 0xA040;
    UartHandle->SCIFFRX.all = 0x2044;
    UartHandle->SCIFFCT.all = 0x0;
    
	if(UartHandle == &SciaRegs)
	{
        EALLOW;	// This is needed to write to EALLOW protected registers
        PieVectTable.SCIA_RX_INT = &Bsp_SciaRecvIsr;
        PieVectTable.SCIA_TX_INT = &Bsp_SciaSendIsr;
        EDIS;   // This is needed to disable write to EALLOW protected registers

        PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
        IER |= M_INT9;                              // Enable CPU INT9
	}
	else if(UartHandle == &ScibRegs)
	{
        EALLOW;	// This is needed to write to EALLOW protected registers
        PieVectTable.SCIB_RX_INT = &Bsp_ScibRecvIsr;
        PieVectTable.SCIB_TX_INT = &Bsp_ScibSendIsr;
        EDIS;   // This is needed to disable write to EALLOW protected registers

        PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
        IER |= M_INT9;                              // Enable CPU INT9
	}
	else if(UartHandle == &ScicRegs)
	{
        EALLOW;	// This is needed to write to EALLOW protected registers
        PieVectTable.SCIC_RX_INT = &Bsp_ScicRecvIsr;
        PieVectTable.SCIC_TX_INT = &Bsp_ScicSendIsr;
        EDIS;   // This is needed to disable write to EALLOW protected registers

        PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
        IER |= M_INT8;                              // Enable CPU INT8
	}
	else if(UartHandle == &ScidRegs)
	{
        EALLOW;	// This is needed to write to EALLOW protected registers
        PieVectTable.SCID_RX_INT = &Bsp_ScidRecvIsr;
        PieVectTable.SCID_TX_INT = &Bsp_ScidSendIsr;
        EDIS;   // This is needed to disable write to EALLOW protected registers

        PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
        IER |= M_INT8;                              // Enable CPU INT8
	}
	Bsp_UsartTxEnable(pst_Dev);
	Bsp_UsartRxEnable(pst_Dev);

    return TRUE;
}

//==================================================================================
//| 函数名称 | Bsp_UartClose
//|----------|----------------------------------------------------------------------
//| 函数功能 | 关闭串口
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:串口配置结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE 关闭失败, TRUE 关闭成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartClose(Dev_SerialPort* pst_Dev)
{
	HAL_UART_MspDeInit(pst_Dev->pv_UartHandle);
    return TRUE;
}

//==================================================================================
//| 函数名称 | Bsp_UartSendBlock
//|----------|----------------------------------------------------------------------
//| 函数功能 | 串口发送(阻塞的)
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:串口结构体 puch_Buff:发送缓冲区 uin_Len:发送缓冲区长度
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE:参数有误, TRUE:发送成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartSendBlock(Dev_SerialPort *pst_Dev, INT8U* puch_Buff, INT16U uin_Len)
{
    //INT16U i = 0;
#if 0
    if( pst_Dev == NULL || puch_Buff == NULL || uin_Len == 0 )
        return FALSE;
    
    /* 非阻塞发送完成 才能使用阻塞的发送 */
    if( pst_Dev->uin_TxCount != 0 || pst_Dev->uin_TxLen != 0)
        return FALSE;

    
    /* 需要关闭串口发送完成中断 */
    while(uin_Len--)
    {
        Bsp_SendOneByte(pst_Dev,puch_Buff[i++]);
    }
#endif
    if( pst_Dev == NULL || puch_Buff == NULL || uin_Len == 0 )
        return FALSE;

    if( pst_Dev->uin_TxCount < pst_Dev->uin_TxLen)
        return FALSE;

    pst_Dev->puch_TxBuff    = puch_Buff;
    pst_Dev->uin_TxLen      = uin_Len;
    pst_Dev->uin_TxCount    = 0;

    Bsp_SendOneByte(pst_Dev, puch_Buff[pst_Dev->uin_TxCount++]);

    while(pst_Dev->uin_TxLen != 0){}

    return TRUE;
}

//==================================================================================
//| 函数名称 | Bsp_UartSend
//|----------|----------------------------------------------------------------------
//| 函数功能 | 串口发送(非阻塞)
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:串口结构体 puch_Buff:发送缓冲区 uin_Len:发送缓冲区长度
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE:参数有误, TRUE:发送成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartSend(Dev_SerialPort *pst_Dev, INT8U* puch_Buff, INT16U uin_Len)
{
    if( pst_Dev == NULL || puch_Buff == NULL || uin_Len == 0 )
        return FALSE;
    
    if( pst_Dev->uin_TxCount < pst_Dev->uin_TxLen)
        return FALSE;

    pst_Dev->puch_TxBuff    = puch_Buff;
    pst_Dev->uin_TxLen      = uin_Len;
    pst_Dev->uin_TxCount    = 0;
    
    Bsp_SendOneByte(pst_Dev, puch_Buff[pst_Dev->uin_TxCount++]);

    return TRUE;
}

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(struct SCI_REGS *huart)
{
    if(huart == &SciaRegs)
    {
        // SCIA GPIO 初始化
#if 0
        GPIO_SetupPinMux(9, GPIO_MUX_CPU1, 6);
        GPIO_SetupPinOptions(9, GPIO_INPUT, GPIO_PUSHPULL);
        GPIO_SetupPinMux(8, GPIO_MUX_CPU1, 6);
        GPIO_SetupPinOptions(8, GPIO_OUTPUT, GPIO_ASYNC);
#else
        GPIO_SetupPinMux(64, GPIO_MUX_CPU1, 6);
        GPIO_SetupPinOptions(64, GPIO_INPUT, GPIO_PUSHPULL);
        GPIO_SetupPinMux(65, GPIO_MUX_CPU1, 6);
        GPIO_SetupPinOptions(65, GPIO_OUTPUT, GPIO_ASYNC);
#endif

    }
    else if(huart == &ScibRegs)
    {
        // SCIB GPIO 初始化
        GPIO_SetupPinMux(11, GPIO_MUX_CPU1, 6);
        GPIO_SetupPinOptions(11, GPIO_INPUT, GPIO_PUSHPULL);
        GPIO_SetupPinMux(10, GPIO_MUX_CPU1, 6);
        GPIO_SetupPinOptions(10, GPIO_OUTPUT, GPIO_ASYNC);
    }
    else if(huart == &ScicRegs)
    {

    }
    else if(huart == &ScidRegs)
    {

    }
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(struct SCI_REGS *huart)
{
    if(huart == &SciaRegs)
    {

    }
    else if(huart == &ScibRegs)
    {

    }
    else if(huart == &ScicRegs)
    {

    }
    else if(huart == &ScidRegs)
    {

    }
}

void Bsp_UartPrintf(const char * Format,...)
{
	Dev_SerialPort* p = &COM1;
	while(p->uin_TxLen != 0){}
	va_list pArgs;
	va_start(pArgs,Format);
	vsprintf((char *)auch_PrintfBuff,Format,pArgs);
	va_end(pArgs);

	/* scia 是 485接口*/
	if(p == &COM1)
	{
        //Bsp_Rs485de(eRs485Trans);
	}

	Bsp_UartSendBlock(p,auch_PrintfBuff,strlen((const char*)auch_PrintfBuff));
}


int fputc(int ch, FILE *f)
{
    //while (!LL_USART_IsActiveFlag_TXE(USART1)){}
    //LL_USART_TransmitData8(USART1, ch); 
    
    //while (!LL_USART_IsActiveFlag_TC(USART2)){}
    //LL_USART_ClearFlag_TC(USART2);
    return ch;
}

