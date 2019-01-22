#ifndef __BSP_SPI_H__
#define	__BSP_SPI_H__

#include "bsp.h"


void Bsp_SpibInit(void);

INT8U Bsp_SpibGetTxFifoStaus(void);

INT8U Bsp_SpibWriteByte(INT8U a);

void Bsp_SpibFifoInit(void);

#endif
