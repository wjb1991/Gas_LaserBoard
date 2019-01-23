#include "Bsp.h"


void Bsp_AD5546Disable(void)
{
	GPIO_WritePin(67, 0);		//
}

void Bsp_AD5546Enable(void)
{
	GPIO_WritePin(67, 1);		//
}

void Bsp_AD5546Reset(void)
{
	Bsp_AD5546Disable();
	Bsp_DelayUs(10);	//3.3V 35ns
	Bsp_AD5546Enable();
}

void Bsp_AD5546Set(INT16U  data)
{
	Bsp_IntDis();
	*((INT16U*)(DEF_AD5546ADDR)) = data;
	Bsp_IntEn();
}

INT16U Bsp_AD5546VoltToHex(FP64 volt)
{
	INT16U res = 0;
	if(volt < 0)
	{
		res = (INT16U)((volt / DEF_AD5546_REF_VOLTA * 65535) * DEF_AD5546_SCALE);
	}
	else
		res = 0;
	return res;
}

FP32 Bsp_AD5546HexToVolt(INT16U hex)
{
	return((INT16U)((hex / 65535 * DEF_AD5546_REF_VOLTA) / DEF_AD5546_SCALE));
}

void Bsp_AD5546Init(void)
{
	//RESET
	GPIO_SetupPinMux(67, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(67, GPIO_OUTPUT, GPIO_OPENDRAIN);
	//LDAC 
	GPIO_SetupPinMux(68, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(68, GPIO_OUTPUT, GPIO_OPENDRAIN);
	GPIO_WritePin(68, 0);

	Bsp_AD5546Reset();

	//self test
	Bsp_AD5546Set(0);					//DAC Êä³ö0V
}
