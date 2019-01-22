#include "Bsp.h"
//STM32F4开发版地址 0x68000000~ 0x680FFFFF   1M Byte
//尾气激光板 0x100000 512k

__STATIC_INLINE void HAL_SRAM_MspInit(void);

BOOL Bsp_SramSelfTest(void)
{
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
	}

	for(i = 0; i < DEF_EXSRAM_SIZE; i++)
	{
		if (puin_addr[i] != (INT16U)(i % 65000))
			return FALSE;
	}

	return TRUE;
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

	//
	//Configure the access timing for CS2 space	Sram
	//
	Emif1Regs.ASYNC_CS2_CR.all = (EMIF_ASYNC_ASIZE_16    | // 16Bit Memory
														  // Interface
								 EMIF_ASYNC_TA_4        | // Turn Around time
														  // of 2 Emif Clock
								 EMIF_ASYNC_RHOLD_8     | // Read Hold time
														  // of 1 Emif Clock
								 EMIF_ASYNC_RSTROBE_32   | // Read Strobe time
														  // of 4 Emif Clock
								 EMIF_ASYNC_RSETUP_8    | // Read Setup time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WHOLD_8     | // Write Hold time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WSTROBE_32   | // Write Strobe time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WSETUP_8    | // Write Setup time
														  // of 1 Emif Clock
								 EMIF_ASYNC_EW_DISABLE  | // Extended Wait
														  // Disable.
								 EMIF_ASYNC_SS_DISABLE    // Strobe Select Mode
														  // Disable.
								);

	//
	//Configure the access timing for CS3 space	AD5546 DAC
	//
	Emif1Regs.ASYNC_CS3_CR.all = (EMIF_ASYNC_ASIZE_16    | // 16Bit Memory
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
								 EMIF_ASYNC_WSTROBE_32   | // Write Strobe time
														  // of 1 Emif Clock
								 EMIF_ASYNC_WSETUP_8    | // Write Setup time
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
