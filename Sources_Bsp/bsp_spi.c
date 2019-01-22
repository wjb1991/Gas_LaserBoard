

#include "bsp_spi.h"
//
// Calculate BRR: 7-bit baud rate register value
// SPI CLK freq = 500 kHz
// LSPCLK freq  = CPU freq / 4  (by default)
// BRR          = (LSPCLK freq / SPI CLK freq) - 1
//
//500E4 = 5MHZ
#if CPU_FRQ_200MHZ
#define SPI_BRR        ((200E6 / 4) / 500E4) - 1
#endif

#if CPU_FRQ_150MHZ
#define SPI_BRR        ((150E6 / 4) / 500E3) - 1
#endif

#if CPU_FRQ_120MHZ
#define SPI_BRR        ((120E6 / 4) / 500E3) - 1
#endif


void Bsp_SpibInit(void)
{
	GPIO_SetupPinMux(24, GPIO_MUX_CPU1, 6);
	GPIO_SetupPinOptions(24, GPIO_INPUT, GPIO_ASYNC);
	GPIO_SetupPinMux(25, GPIO_MUX_CPU1, 6);
	GPIO_SetupPinOptions(24, GPIO_INPUT, GPIO_ASYNC);
	GPIO_SetupPinMux(26, GPIO_MUX_CPU1, 6);
	GPIO_SetupPinOptions(26, GPIO_INPUT, GPIO_ASYNC);

	//GPIO_SetupPinMux(27, GPIO_MUX_CPU1, 6);
	//GPIO_SetupPinOptions(27, GPIO_INPUT, GPIO_ASYNC);

    // Initialize SPI-B
    // Set reset low before configuration changes
    // Clock polarity (0 == rising, 1 == falling)
    // 8-bit character
    // Disable loop-back
	SpibRegs.SPICCR.bit.SPISWRESET = 0;
    SpibRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpibRegs.SPICCR.bit.SPICHAR = (8-1);
    SpibRegs.SPICCR.bit.SPILBK = 0;

    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpibRegs.SPICTL.bit.TALK = 1;
    SpibRegs.SPICTL.bit.CLK_PHASE = 0;
    SpibRegs.SPICTL.bit.SPIINTENA = 0;

    // Set the baud rate
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = SPI_BRR;

    // Set FREE bit
    // Halting on a breakpoint will not halt the SPI
    SpibRegs.SPIPRI.bit.FREE = 1;

    // Release the SPI from reset
    SpibRegs.SPICCR.bit.SPISWRESET = 1;
}

INT8U Bsp_SpibGetTxFifoStaus(void)
{
	return ((INT8U)SpibRegs.SPIFFTX.bit.TXFFST);
}

INT8U Bsp_SpibWriteByte(INT8U a)
{
	while(SpibRegs.SPISTS.bit.BUFFULL_FLAG);
	SpibRegs.SPITXBUF = (a<<8);

	while(!SpibRegs.SPISTS.bit.INT_FLAG);
	return SpibRegs.SPIRXBUF;

	//SpibRegs.SPITXBUF = (a<<8);
	//return 0;	//
}

void Bsp_SpibFifoInit(void)
{
// Initialize SPI FIFO registers
	SpibRegs.SPIFFRX.all=0x2044;			//

	SpibRegs.SPIFFTX.all=0xE060;            // FIFOs enabled, TX FIFO released,

	SpibRegs.SPIFFTX.bit.TXFFIENA = 1;
	SpibRegs.SPIFFTX.bit.TXFFIL = 0;  		// Set TX FIFO level

	SpibRegs.SPIFFCT.bit.TXDLY = 0x0;
}
