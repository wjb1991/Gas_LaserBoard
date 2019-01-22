
#include "bsp.h"
#include "bsp_spi.h"
#include "bsp_ad5663.h"


void Bsp_AD5663Cs(INT8U status)
{
    GPIO_WritePin(99,(INT16U)status);
}

void Bsp_AD5663WirteCmd(INT8U cmd,INT8U dev,INT16U val)
{
	//CS 5MHZ  5us
	uint32_t data = 0;

	Bsp_AD5663Cs(0);

	data |= ((uint32_t)(cmd & 0x07))<<19;
	data |= ((uint32_t)(dev & 0x07))<<16;
	data |= val;

	Bsp_SpibWriteByte((data>>16)&0xff);
	Bsp_SpibWriteByte((data>>8)&0xff);
	Bsp_SpibWriteByte(data&0xff);

	Bsp_AD5663Cs(1);
}

void Bsp_AD5663Set(Ad5563Channel_t channel,INT16U val)
{
	Bsp_AD5663WirteCmd(0x0,(INT8U)channel,val);	//
}

INT16U Bsp_AD5663CHAVoltToHex(FP64 volt)
{
	INT16U res = (INT16U)((volt / 2.5 * 65535)* 3/2);
	return res;
}

INT16U Bsp_AD5663CHBVoltToHex(FP64 volt)
{
	INT16U res = (INT16U)((volt / 2.5 * 65535));
	return res;
}

void Bsp_AD5663Init(void)
{

	GPIO_SetupPinMux(99, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(99, GPIO_OUTPUT, GPIO_PUSHPULL);

	Bsp_AD5663WirteCmd(0x5,eAD5563_CHAB,0x0001);	//
	Bsp_AD5663WirteCmd(0x6,eAD5563_CHAB,0x0003);	//

	Bsp_AD5663Set(eAD5563_CHAB,0);
}
