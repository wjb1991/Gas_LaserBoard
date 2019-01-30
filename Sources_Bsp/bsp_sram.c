#include "Bsp.h"
//STM32F4开发版地址 0x68000000~ 0x680FFFFF   1M Byte
//尾气激光板 0x100000 512k

volatile INT16U* auin_ExsramSelfTest = (volatile INT16U*)0x100000u;

__STATIC_INLINE void HAL_SRAM_MspInit(void);

BOOL Bsp_SramSelfTest(void)
{
    INT32U i;
    INT32U error = 0;
    BOOL res = TRUE;

    Bsp_IntDis();

    //关闭其他并口外设 防止总线冲突 非常重要
   // Bsp_AD5546Disable();
   //Bsp_AD7622Disable();

    for(i = 0; i < 0x080000; i++)       //清除RAM中的数据
        auin_ExsramSelfTest[i] = 0;

    for(i = 0; i < 0x080000; i++)
    {
        volatile INT16U dat = (i%65000);
        volatile INT16U rdat;
        auin_ExsramSelfTest[i] = dat;
        //Bsp_DelayUs(1);
        rdat = 0;
        rdat = auin_ExsramSelfTest[i];
        //Bsp_DelayUs(1);
        if(rdat != dat)
        {
            TRACE_DBG("  >>ExsramSelfTest[%02X%04X]",(INT16U)(i/0x10000ul),(INT16U)(i%0x10000ul));
            TRACE_DBG("  WDAT = %04X, RDAT = %04X, DAT^RDAT = %04X\r\n",dat,rdat,rdat^dat);

            if(++error >= 10)
            {
                res = FALSE;
                break;
            }
        }
        //else
            //TRACE_DBG("  WDAT = %04X, RDAT = %04X, Success\r\n",dat,rdat);

    }
    //Bsp_AD5546Enable();
    //Bsp_AD7622Enable();

    Bsp_IntEn();

    return res;

/*
	//DEF_EXSRAM_SIZE
	volatile INT16U* puin_addr = (volatile INT16U*)DEF_EXSRAM_STA_ADDR;
	INT32U i;

	for(i = 0; i < DEF_EXSRAM_SIZE; i++)
	{
		puin_addr[i] = 0;
	}

	for(i = 0; i < DEF_EXSRAM_SIZE; i++)
	{
		puin_addr[i] = (INT16U)(i % 65000);
        TRACE_DBG("写入 puin_addr[%d] = 0x%x\r\n",i,(INT16U)(i % 65000));
	}

	for(i = 0; i < DEF_EXSRAM_SIZE; i++)
	{

        TRACE_DBG("读取 puin_addr[%d] = 0x%x\r\n",i,puin_addr[i]);
		if (puin_addr[i] != (INT16U)(i % 65000))
			return FALSE;
	}
*/
}

void Bsp_SramInit(void)
{
	int ErrCount = 0;
	//Configure to run EMIF1 on full Rate (EMIF1CLK = CPU1SYSCLK)
	EALLOW;
	ClkCfgRegs.PERCLKDIVSEL.bit.EMIF1CLKDIV = 0x1;
	EDIS;

	EALLOW;

	//Disable Access Protection (CPU_FETCH/CPU_WR/DMA_WR)
	Emif1ConfigRegs.EMIF1ACCPROT0.all = 0x0;
	if (Emif1ConfigRegs.EMIF1ACCPROT0.all != 0x0)
	{
		ErrCount++;
	}

	// Commit the configuration related to protection. Till this bit remains set
	// content of EMIF1ACCPROT0 register can't be changed.
	Emif1ConfigRegs.EMIF1COMMIT.all = 0x1;
	if(Emif1ConfigRegs.EMIF1COMMIT.all != 0x1)
	{
		ErrCount++;
	}

	// Lock the configuration so that EMIF1COMMIT register can't be changed any more.
	Emif1ConfigRegs.EMIF1LOCK.all = 0x1;
	if (Emif1ConfigRegs.EMIF1LOCK.all != 1)
	{
		ErrCount++;
	}

	EDIS;
/**/
	//
	//Configure the access timing for CS2 space	Sram
	//
	Emif1Regs.ASYNC_CS2_CR.all = (EMIF_ASYNC_ASIZE_16    | // 16Bit Memory
														  // Interface
								 EMIF_ASYNC_TA_4        | // Turn Around time
														  // of 2 Emif Clock
								 EMIF_ASYNC_RHOLD_2     | // Read Hold time
														  // of 1 Emif Clock
								 EMIF_ASYNC_RSTROBE_16   | // Read Strobe time
														  // of 4 Emif Clock
								 EMIF_ASYNC_RSETUP_8    | // Read Setup time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WHOLD_2     | // Write Hold time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WSTROBE_16   | // Write Strobe time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WSETUP_8    | // Write Setup time
														  // of 1 Emif Clock
								 EMIF_ASYNC_EW_DISABLE  | // Extended Wait
														  // Disable.
								 EMIF_ASYNC_SS_DISABLE    // Strobe Select Mode
														  // Disable.
								);

	//
	//Configure the access timing for CS3 space	AD5546 DAC  写入时间 4 8 1 用内部RAM传输数据无毛刺
	//
	Emif1Regs.ASYNC_CS3_CR.all = (EMIF_ASYNC_ASIZE_16    | // 16Bit Memory
														  // Interface
								 EMIF_ASYNC_TA_4        | // Turn Around time
														  // of 2 Emif Clock
								 EMIF_ASYNC_RHOLD_1     | // Read Hold time
														  // of 1 Emif Clock
								 EMIF_ASYNC_RSTROBE_1   | // Read Strobe time
														  // of 4 Emif Clock
								 EMIF_ASYNC_RSETUP_1    | // Read Setup time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WHOLD_4     | // Write Hold time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WSTROBE_8   | // Write Strobe time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WSETUP_1    | // Write Setup time       //没有地址总线不需要等待地址信号
														  // of 1 Emif Clock
								 EMIF_ASYNC_EW_DISABLE  | // Extended Wait
														  // Disable.
								 EMIF_ASYNC_SS_DISABLE    // Strobe Select Mode
														  // Disable.
								);

	//
	//Configure the access timing for CS4 space	AD7622
	//
	Emif1Regs.ASYNC_CS4_CR.all = (EMIF_ASYNC_ASIZE_16    | // 16Bit Memory
														  // Interface
								 EMIF_ASYNC_TA_4        | // Turn Around time
														  // of 2 Emif Clock
								 EMIF_ASYNC_RHOLD_8     | // Read Hold time
														  // of 1 Emif Clock
								 EMIF_ASYNC_RSTROBE_32   | // Read Strobe time
														  // of 4 Emif Clock
								 EMIF_ASYNC_RSETUP_8    | // Read Setup time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WHOLD_1     | // Write Hold time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WSTROBE_1   | // Write Strobe time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WSETUP_1    | // Write Setup time
														  // of 1 Emif Clock
								 EMIF_ASYNC_EW_DISABLE  | // Extended Wait
														  // Disable.
								 EMIF_ASYNC_SS_DISABLE    // Strobe Select Mode
														  // Disable.
								);
	HAL_SRAM_MspInit();

	EDIS;
}

__STATIC_INLINE void HAL_SRAM_MspInit(void)
{
	uint16_t cpu_sel = GPIO_MUX_CPU1;
	uint16_t i;

    for (i=28; i<=52; i++)
    {
        if ((i != 29) &&(i != 30) &&(i != 33) && (i != 42) && (i != 43))
        {
            GPIO_SetupPinMux(i,cpu_sel,2);
        }
    }
    for (i=69; i<=87; i++)
    {
        if (i != 84)
        {
            GPIO_SetupPinMux(i,cpu_sel,2);
        }
    }

    GPIO_SetupPinMux(88,cpu_sel,2);
    GPIO_SetupPinMux(89,cpu_sel,2);
    GPIO_SetupPinMux(90,cpu_sel,2);
    //GPIO_SetupPinMux(91,cpu_sel,3);
    GPIO_SetupPinMux(92,cpu_sel,3);
    //GPIO_SetupPinMux(93,cpu_sel,3);
    //GPIO_SetupPinMux(94,cpu_sel,2);

    //
    //setup async mode and enable pull-ups for Data pins
    //
    for (i=69; i<=85; i++)
    {
        if (i != 84)
        {
            GPIO_SetupPinOptions(i,0,0x31); // GPIO_ASYNC||GPIO_PULLUP
        }
    }
}


//__STATIC_INLINE void HAL_SRAM_MspDeInit(void)
//{
//
//}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
