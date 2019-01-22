
#include "Bsp.h"

void Bsp_AD7622Disable(void)
{
	GPIO_WritePin(66, 1);		//
}

BOOL Bsp_AD7622IsBusy(void)
{
	return ((BOOL)GPIO_ReadPin(65));	//
}

void Bsp_AD7622Enable(void)
{
	do
	{
		GPIO_WritePin(66, 0);		//
		Bsp_DelayUs(10);			//500ns
	}
	while(Bsp_AD7622IsBusy() == true);
}

void Bsp_AD7622Reset(void)
{
	Bsp_AD7622Disable();
	Bsp_DelayUs(10);	//3.3V 35ns
	Bsp_AD7622Enable();
}

INT16U Bsp_AD7622Read(void)
{
	while(Bsp_AD7622IsBusy() == true);
	return *((uint16_t*)(0x00380000));
}

void Bsp_AD7622Init(void)
{
	//Reset Pin
	GPIO_SetupPinMux(66, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(66, GPIO_OUTPUT, GPIO_PUSHPULL);
	//Busy Pin
	GPIO_SetupPinMux(65, GPIO_MUX_CPU1, 0);
	GPIO_SetupPinOptions(65, GPIO_INPUT, GPIO_PULLUP);

	Bsp_AD7622Reset();
}

