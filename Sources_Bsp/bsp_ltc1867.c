#include "Bsp.h"

typedef enum {
    eLtc1867CH0 = 0x8400,
    eLtc1867CH1 = 0xC400,
    eLtc1867CH2 = 0x9400,
    eLtc1867CH3 = 0xD400,
    eLtc1867CH4 = 0xA400,
    eLtc1867CH5 = 0xE400,
    eLtc1867CH6 = 0xB400,
    eLtc1867CH7 = 0xF400,
}Ltc1867Channel_t;

void Bsp_Ltc1867Cs(INT8U status)
{
    GPIO_WritePin(27,(INT16U)status);
}

void Bsp_Ltc1867Init(void)
{
    GPIO_SetupPinMux(27, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(27, GPIO_OUTPUT, GPIO_PUSHPULL);

    Bsp_Ltc1867Cs(1);
}

Ltc1867Channel_t Bsp_Ltc1866Chnnel(INT16U ch)
{
    switch (ch)
    {
    case 0:
        return (eLtc1867CH0);
    case 1:
        return (eLtc1867CH1);
    case 2:
        return (eLtc1867CH2);
    case 3:
        return (eLtc1867CH3);
    case 4:
        return (eLtc1867CH4);
    case 5:
        return (eLtc1867CH5);
    case 6:
        return (eLtc1867CH6);
    case 7:
        return (eLtc1867CH7);
    default:
        return (eLtc1867CH0);
    }
}

INT16U Bsp_Ltc1867SampleOne(INT16U ch)
{
    INT16U msb = 0;
    INT16U lsb = 0;
    Ltc1867Channel_t chnnel;
    Bsp_Ltc1867Cs(0);

    chnnel = Bsp_Ltc1866Chnnel(ch);
    Bsp_DelayUs(0.5);
    Bsp_IntDis();
    msb = Bsp_SpibWriteByte(chnnel >> 8);
    lsb = Bsp_SpibWriteByte(chnnel & 0x00ff);
    Bsp_IntEn();
    Bsp_DelayUs(0.5);
    Bsp_Ltc1867Cs(1);
    Bsp_DelayUs(3.5);
    return ((msb << 8) | lsb);
}

INT16U Bsp_Ltc1867SampleAvg(INT16U ch,INT16U avg)
{
    uint32_t sum = 0;
    INT16U i = 0;
    Bsp_Ltc1867SampleOne(ch);

    for(i = 0; i < avg; i++)
    {
        sum += Bsp_Ltc1867SampleOne(ch);
    }
    sum /= avg;
    return sum;
}


FP64 Bsp_Ltc1867HexToVolt(FP64 hex)
{
    return ( hex * 4.096 / 65535.0);
}


