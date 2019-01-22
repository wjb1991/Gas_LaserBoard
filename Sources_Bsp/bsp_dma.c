
#include "Bsp_Dma.h"


static void (*s_dma1hook)(void) = NULL;
static void (*s_dma2hook)(void) = NULL;
static void (*s_dma3hook)(void) = NULL;

__interrupt void Bsp_Dma1Isr(void)		// DMA Ch1
{
    if( s_dma1hook != NULL )
        (*s_dma1hook)();
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP7; // ACK to receive more interrupts from this PIE group
	return;
}

__interrupt void Bsp_Dma2Isr(void)		// DMA Ch2
{
    if( s_dma2hook != NULL )
        (*s_dma2hook)();
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP7; // ACK to receive more interrupts from this PIE group
	return;
}

__interrupt void Bsp_Dma3Isr(void)		// DMA Ch2
{
    if( s_dma3hook != NULL )
        (*s_dma3hook)();
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP7; // ACK to receive more interrupts from this PIE group
	return;
}

void Bsp_Dma1Init(void * source, void * dest, uint16_t srcSetp, uint16_t destSetp, uint16_t len)
{
	//EALLOW;
	//CpuSysRegs.SECMSEL.bit.PF2SEL = 1;
	//EDIS;

	DMACH1AddrConfig((volatile Uint16 *)dest,(volatile Uint16 *)source);
	DMACH1BurstConfig(0,0,0);			// Burst size, src step, dest step	Burst size 0 = 1次传输
	//DMACH1WrapConfig(0, 0, 0, 0);
	DMACH1TransferConfig(len-1,srcSetp,destSetp);		// transfer size, src step, dest step	transfer size 0 = 1次传输
	DMACH1ModeConfig(DMA_TINT0,PERINT_ENABLE,ONESHOT_DISABLE,CONT_DISABLE
			,SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,SIXTEEN_BIT,CHINT_END,CHINT_ENABLE);
            
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.DMA_CH1_INT= &Bsp_Dma1Isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
	IER |= M_INT7;						 // Enable CPU INT6
}

void Bsp_Dma1Start(void)
{
    EALLOW;
    DmaRegs.CH1.CONTROL.bit.RUN = 1;
    EDIS;
}

void Bsp_Dma1Stop(void)
{
    EALLOW;
    DmaRegs.CH1.CONTROL.bit.RUN = 0;
    EDIS;
}

void Bsp_Dma1IntEnable(void)
{
    EALLOW;
	PieCtrlRegs.PIEIER7.bit.INTx1 = 1;	 // Enable PIE Group 7, INT 1 (DMA CH1)
    EDIS;
}

void Bsp_Dma1IntDisable(void)
{
    EALLOW;
	PieCtrlRegs.PIEIER7.bit.INTx1 = 0;	 // Enable PIE Group 7, INT 1 (DMA CH1)
    EDIS;
}

void Bsp_Dma1HookRegister( void (*hook)(void))
{
	s_dma1hook = hook;
}

void Bsp_Dma2Init(void * source, void * dest, uint16_t srcSetp, uint16_t destSetp, uint16_t len)
{
	//EALLOW;
	//CpuSysRegs.SECMSEL.bit.PF2SEL = 1;
	//EDIS;

	DMACH2AddrConfig((volatile Uint16 *)dest,(volatile Uint16 *)source);
	DMACH2BurstConfig(0,0,0);			// Burst size, src step, dest step		Burst size 0 = 1次传输
	//DMACH1WrapConfig(0, 0, 0, 0);
	DMACH2TransferConfig(len-1,srcSetp,destSetp);		// transfer size, src step, dest step	transfer size 0 = 1次传输
	DMACH2ModeConfig(DMA_TINT0,PERINT_ENABLE,ONESHOT_DISABLE,CONT_DISABLE
			,SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,SIXTEEN_BIT,CHINT_END,CHINT_ENABLE);
            
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.DMA_CH2_INT= &Bsp_Dma2Isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers
    
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
	IER |= M_INT7;						 // Enable CPU INT6
}

void Bsp_Dma2Start(void)
{
    EALLOW;
    DmaRegs.CH2.CONTROL.bit.RUN = 1;
    EDIS;
}

void Bsp_Dma2Stop(void)
{
    EALLOW;
    DmaRegs.CH2.CONTROL.bit.RUN = 0;
    EDIS;
}

void Bsp_Dma2IntEnable(void)
{
    EALLOW;
    PieCtrlRegs.PIEIER7.bit.INTx2 = 1;	 // Enable PIE Group 7, INT 2 (DMA CH2)
    EDIS;
}

void Bsp_Dma2IntDisable(void)
{
    EALLOW;
    PieCtrlRegs.PIEIER7.bit.INTx2 = 1;	 // Enable PIE Group 7, INT 2 (DMA CH2)
    EDIS;
}

void Bsp_Dma2HookRegister( void (*hook)(void))
{
	s_dma2hook = hook;
}

void Bsp_Dma3Init(void * source, void * dest, uint16_t srcSetp, uint16_t destSetp, uint16_t len)
{
	//EALLOW;
	//CpuSysRegs.SECMSEL.bit.PF2SEL = 1;
	//EDIS;

	DMACH3AddrConfig((volatile Uint16 *)dest,(volatile Uint16 *)source);
	DMACH3BurstConfig(0,0,0);			// Burst size, src step, dest step		Burst size 0 = 1次传输
	//DMACH1WrapConfig(0, 0, 0, 0);
	DMACH3TransferConfig(len-1,srcSetp,destSetp);		// transfer size, src step, dest step	transfer size 0 = 1次传输
	DMACH3ModeConfig(DMA_TINT0,PERINT_ENABLE,ONESHOT_DISABLE,CONT_DISABLE
			,SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,SIXTEEN_BIT,CHINT_END,CHINT_ENABLE);

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.DMA_CH3_INT= &Bsp_Dma3Isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
	IER |= M_INT7;						 // Enable CPU INT6
}

void Bsp_Dma3Start(void)
{
    EALLOW;
    DmaRegs.CH3.CONTROL.bit.RUN = 1;
    EDIS;
}

void Bsp_Dma3Stop(void)
{
    EALLOW;
    DmaRegs.CH3.CONTROL.bit.RUN = 0;
    EDIS;
}

void Bsp_Dma3IntEnable(void)
{
    EALLOW;
    PieCtrlRegs.PIEIER7.bit.INTx2 = 1;	 // Enable PIE Group 7, INT 2 (DMA CH2)
    EDIS;
}

void Bsp_Dma3IntDisable(void)
{
    EALLOW;
    PieCtrlRegs.PIEIER7.bit.INTx2 = 1;	 // Enable PIE Group 7, INT 2 (DMA CH2)
    EDIS;
}

void Bsp_Dma3HookRegister( void (*hook)(void))
{
	s_dma3hook = hook;
}
