
#include "Bsp.h"

interrupt void xint1_isr(void);
interrupt void xint2_isr(void);
interrupt void xint3_isr(void);

void Bsp_GpioInit(void)
{

    Bsp_UsbPow(eUsbPowOff);
    Bsp_RunLed(eLedOff);
	Bsp_RunLed(eLedOff);
	Bsp_AlarmLed(eLedOn);
	Bsp_LaserPR(eLaserPrOn);
	Bsp_SoftStart(eSofrtStartOff);
	Bsp_Tec2Enable(eTecDisable);
	Bsp_Tec1Enable(eTecDisable);
	Bsp_SigSelect(eSenSigSel);
	Bsp_EepromWp(eEepromWpOff);
	Bsp_Pga0A0(0);
	Bsp_Pga0A1(0);
	Bsp_Pga2A0(0);
	Bsp_Pga2A1(0);

	//Run LED
	GPIO_SetupPinMux(64, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(64, GPIO_OUTPUT, GPIO_PUSHPULL);
	//ALARM LED
	GPIO_SetupPinMux(63, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(63, GPIO_OUTPUT, GPIO_PUSHPULL);
    //PGA0_A0
	GPIO_SetupPinMux(22, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(22, GPIO_OUTPUT, GPIO_PUSHPULL);
    //PGA0_A1
	GPIO_SetupPinMux(23, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(23, GPIO_OUTPUT, GPIO_PUSHPULL);
    //PGA2_A0
	GPIO_SetupPinMux(14, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(14, GPIO_OUTPUT, GPIO_PUSHPULL);
    //PGA2_A1
	GPIO_SetupPinMux(15, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(15, GPIO_OUTPUT, GPIO_PUSHPULL);
/**/
	//LASER_PR
	GPIO_SetupPinMux(20, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(20, GPIO_OUTPUT, GPIO_PUSHPULL);
    //SOFT_START
    GPIO_SetupPinMux(21, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(21, GPIO_OUTPUT, GPIO_PUSHPULL);

    //SWAP_SIG
	GPIO_SetupPinMux(93, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(93, GPIO_OUTPUT, GPIO_PUSHPULL);
	//TEC_EN2
	GPIO_SetupPinMux(94, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(94, GPIO_OUTPUT, GPIO_PUSHPULL);
	//TEC_EN1
	GPIO_SetupPinMux(33, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(33, GPIO_OUTPUT, GPIO_PUSHPULL);
	//24c512_WP
	GPIO_SetupPinMux(30, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(30, GPIO_OUTPUT, GPIO_PUSHPULL);
	//Rs485_DE
	GPIO_SetupPinMux(7, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(7, GPIO_OUTPUT, GPIO_PUSHPULL);
    //USBPower
    GPIO_SetupPinMux(16, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(16, GPIO_OUTPUT, GPIO_PUSHPULL);

	//I_Alarm
	GPIO_SetupPinMux(36, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(36, GPIO_INPUT, GPIO_PULLUP | GPIO_SYNC);

	//Sync1
	GPIO_SetupPinMux(56, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(56, GPIO_INPUT, GPIO_PULLUP | GPIO_SYNC);

	//Sync2
	GPIO_SetupPinMux(55, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(55, GPIO_INPUT, GPIO_PULLUP | GPIO_SYNC);

	GPIO_SetupXINT1Gpio(55);
	GPIO_SetupXINT2Gpio(56);
	GPIO_SetupXINT3Gpio(36);


// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;	// This is needed to write to EALLOW protected registers
   PieVectTable.XINT1_INT = &xint1_isr;
   PieVectTable.XINT2_INT = &xint2_isr;
   PieVectTable.XINT3_INT = &xint3_isr;
   EDIS;   // This is needed to disable write to EALLOW protected registers

// Step 4. User specific code, enable interrupts:

// Enable XINT1 and XINT2 in the PIE: Group 1 interrupt 4 & 5
// Enable INT1 which is connected to WAKEINT:
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
   PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Group 1 INT4
   PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // Enable PIE Group 1 INT5
   PieCtrlRegs.PIEIER12.bit.INTx1 = 1;          // Enable PIE Group 12 INT1
   IER |= M_INT1;                              // Enable CPU INT1
   IER |= M_INT12;                              // Enable CPU INT1
   EINT;                                       // Enable Global Interrupts


	XintRegs.XINT1CR.bit.POLARITY = 3;      // Falling edge interrupt
	XintRegs.XINT2CR.bit.POLARITY = 3;      // Falling edge interrupt
	XintRegs.XINT3CR.bit.POLARITY = 2;      // Falling edge interrupt

	XintRegs.XINT1CR.bit.ENABLE = 1;        // Enable XINT1
	XintRegs.XINT2CR.bit.ENABLE = 1;        // Enable XINT2
	//XintRegs.XINT3CR.bit.ENABLE = 1;        // Enable XINT3
}

interrupt void xint1_isr(void)
{

	// Acknowledge this interrupt to get more from group 1
	Bsp_GpioEvent(55,(BOOL)GPIO_ReadPin(55));

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


interrupt void xint2_isr(void)
{

	// Acknowledge this interrupt to get more from group 1
	Bsp_GpioEvent(56,(BOOL)GPIO_ReadPin(56));
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


interrupt void xint3_isr(void)
{
	// Acknowledge this interrupt to get more from group 1
	Bsp_GpioEvent(36,(BOOL)GPIO_ReadPin(36));
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}
