#ifndef __BSP_AD7622_H__
#define	__BSP_AD7622_H__

#include "Bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	DEF_AD7622ADDR	(0x00380000)

void Bsp_AD7622Disable(void);

BOOL Bsp_AD7622IsBusy(void);

void Bsp_AD7622Enable(void);

void Bsp_AD7622Reset(void);

INT16U Bsp_AD7622Read(void);

void Bsp_AD7622Init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif  /* __cplusplus */

#endif
