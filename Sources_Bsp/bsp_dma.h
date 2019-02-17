#ifndef __BSP_DMA_H__
#define	__BSP_DMA_H__

#include "bsp.h"


void Bsp_Dma1Init(void * source, void * dest, uint16_t srcSetp, uint16_t destSetp, uint16_t len);

void Bsp_Dma1Start(void);

void Bsp_Dma1Stop(void);

void Bsp_Dma1IntEnable(void);

void Bsp_Dma1IntDisable(void);

void Bsp_Dma1HookRegister( void (*hook)(void));

INT16U Bsp_Dma1GetTranCont(void);


void Bsp_Dma2Init(void * source, void * dest, uint16_t srcSetp, uint16_t destSetp, uint16_t len);

void Bsp_Dma2Start(void);

void Bsp_Dma2Stop(void);

void Bsp_Dma2IntEnable(void);

void Bsp_Dma2IntDisable(void);

void Bsp_Dma2HookRegister( void (*hook)(void));

void Bsp_Dma3Init(void * source, void * dest, uint16_t srcSetp, uint16_t destSetp, uint16_t len);

void Bsp_Dma3Start(void);

void Bsp_Dma3Stop(void);

void Bsp_Dma3IntEnable(void);

void Bsp_Dma3IntDisable(void);

void Bsp_Dma3HookRegister( void (*hook)(void));

#endif
