#include "Bsp.h"

void Bsp_AdcInit(void)
{
	EALLOW;

	//write configurations
	AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
	//Set pulse positions to late
	AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	//power up the ADCs
	AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	//delay for 1ms to allow ADC time to power up
	Bsp_DelayUS(1000);

	EDIS;
}

uint16_t Bsp_AdcSampleOne(uint8_t adc,uint8_t channel)
{
	uint16_t acqps;

	acqps = 14; //75ns	(n+1)*(1/200mhz)

//Select the channels to convert and end of conversion flag
    EALLOW;
    if( adc == ADC_ADCA ) 
    {
        //ADCA
        AdcaRegs.ADCSOC0CTL.bit.CHSEL = channel;        //SOC0 will convert pin A0
        AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps;          //sample window is acqps + 1 SYSCLK cycles
        AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0;    //end of SOC0 will set INT1 flag
        AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;            //enable INT1 flag
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          //make sure INT1 flag is cleared 
        
        AdcaRegs.ADCSOCFRC1.all = 0x0001;               //SOC0 StartConvert
        
    	while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);
    	AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
        
    	acqps = (AdcaResultRegs.ADCRESULT0);
    }
    else if( adc == ADC_ADCB ) 
    {
        //ADCB
        AdcbRegs.ADCSOC0CTL.bit.CHSEL = channel;        //SOC0 will convert pin B0
        AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps;          //sample window is acqps + 1 SYSCLK cycles
        AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0;    //end of SOC0 will set INT1 flag
        AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;            //enable INT1 flag
        AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          //make sure INT1 flag is cleared    
        
        AdcbRegs.ADCSOCFRC1.all = 0x0001;               //SOC0 StartConvert
        
    	while(AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0);
    	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
        
    	acqps = (AdcbResultRegs.ADCRESULT0);
    }
    else if( adc == ADC_ADCC ) 
    {
        //ADCC
        AdccRegs.ADCSOC0CTL.bit.CHSEL = channel;        //SOC0 will convert pin B0
        AdccRegs.ADCSOC0CTL.bit.ACQPS = acqps;          //sample window is acqps + 1 SYSCLK cycles
        AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0;    //end of SOC0 will set INT1 flag
        AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;            //enable INT1 flag
        AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;          //make sure INT1 flag is cleared
        
        AdccRegs.ADCSOCFRC1.all = 0x0001;               //SOC0 StartConvert   

    	while(AdccRegs.ADCINTFLG.bit.ADCINT1 == 0);
    	AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    	acqps = (AdccResultRegs.ADCRESULT0);
    }
	EDIS;
    return acqps;
}

uint16_t Bsp_AdcSampleAver(uint8_t ucAdc, uint8_t uiChannel,uint16_t uiAverTimes)
{                                                                                                          
	uint16_t  i;
	uint16_t  uiemp;
	uint32_t  ulTemp=0;
	for(i = 0; i < uiAverTimes; i++)
	{
	    uiemp = Bsp_AdcSampleOne(ucAdc,uiChannel);
        ulTemp += uiemp;
	}
	ulTemp = (uint16_t)(ulTemp/uiAverTimes);
	return(ulTemp);
}

double Bsp_AdcHexToVolt(uint16_t hex)
{
	return (hex / 4096.0f * 2.5f);
}

