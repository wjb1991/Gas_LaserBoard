#ifndef __BSP_GPIO_H__
#define	 __BSP_GPIO_H__

#include "Bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    eLedOff = 1, eLedOn =  0,
} LedStatus_t;

typedef enum {
    eLaserPrOff = 0, eLaserPrOn =  1,
} LaserPr_t;

typedef enum {
    eSofrtStartOff = 1, eSofrtStartOn =  0,
} SoftStart_t;

typedef enum {
    eTecEnable = 1, eTecDisable =  0,
} TecStatus_t;

typedef enum {
    eRefSigIVSel = 1, eSenSigSel =  0,
} SwapSig_t;

typedef enum {
    eEepromWpOff = 0, eEepromWpOn =  1,
} EepromWp_t;

typedef enum {
    eRs485Trans = 1, eRs485Recv =  0,
} Rs485de_t;

typedef enum {
    eUsbPowOff = 0, eUsbPowOn =  1,
} UsbPow_t;

void Bsp_GpioInit(void);

void Bsp_GpioEvent(INT32U ul_Pin, BOOL b_IsRising);

//内联提升速度 

inline void Bsp_RunLed(LedStatus_t status )
{
    GPIO_WritePin(64,(uint16_t)status);
}

inline void Bsp_AlarmLed(LedStatus_t status )
{
    GPIO_WritePin(63,(uint16_t)status);
}

inline void Bsp_LaserPR( LaserPr_t status )
{
	GPIO_WritePin(20,status);
}

inline void Bsp_SoftStart( SoftStart_t status )
{
	GPIO_WritePin(21,status);
}

inline void Bsp_Tec2Enable( TecStatus_t status )
{
	GPIO_WritePin(94,status);
}

inline void Bsp_Tec1Enable( TecStatus_t status )
{
	GPIO_WritePin(33,status);
}

inline void Bsp_SigSelect( SwapSig_t status )
{
	GPIO_WritePin(93,status);
}

inline void Bsp_Pga0A0(uint16_t bitVal)
{
	GPIO_WritePin(22,bitVal);
}

inline void Bsp_Pga0A1(uint16_t bitVal)
{
	GPIO_WritePin(23,bitVal);
}

inline void Bsp_Pga2A0(uint16_t bitVal)
{
	GPIO_WritePin(14,bitVal);
}

inline void Bsp_Pga2A1(uint16_t bitVal)
{
	GPIO_WritePin(15,bitVal);
}

inline void Bsp_WachDogIn(uint16_t bitVal)
{
	GPIO_WritePin(62,bitVal);
}

inline void Bsp_EepromWp(EepromWp_t status)
{
	GPIO_WritePin(30,status);
}

inline void Bsp_Rs485de(Rs485de_t status)
{
	GPIO_WritePin(7,status);
}

inline void Bsp_UsbPow(UsbPow_t status )
{
    GPIO_WritePin(16,(uint16_t)status);
}

inline BOOL Bsp_IAlarm(void)
{
	return (BOOL)GPIO_ReadPin(36);
}

#ifdef __cplusplus
} /* extern "C" */
#endif  /* __cplusplus */

#endif
