#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include "Bsp.h"


#define     BSP_0V_M            ADC_ADCA
#define     BSP_0V_CH           0

#define     BSP_I_TEC_M         ADC_ADCB
#define     BSP_I_TEC_CH        0

#define     BSP_1_2V_M          ADC_ADCA
#define     BSP_1_2V_CH         3

#define     BSP_HF_REF2_5V_M    ADC_ADCB
#define     BSP_HF_REF2_5V_CH   3

#define     BSP_HF_3_3V_M       ADC_ADCB
#define     BSP_HF_3_3V_CH      2




#ifdef __cplusplus
extern "C" {
#endif

void Bsp_AdcInit(void);
uint16_t Bsp_AdcSampleOne(uint8_t adc,uint8_t channel);
uint16_t Bsp_AdcSampleAver(uint8_t ucAdc, uint8_t uiChannel,uint16_t uiAverTimes);
double Bsp_AdcHexToVolt(uint16_t hex);

#ifdef __cplusplus
} /* extern "C" */
#endif  /* __cplusplus */

#endif
