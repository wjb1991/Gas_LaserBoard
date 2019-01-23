#ifndef __BSP_SRAM_H__
#define __BSP_SRAM_H__

#include "bsp.h"

#ifdef __cplusplus
extern "C" {
#endif


#define DEF_EXSRAM_STA_ADDR    (0x100000u)
#define DEF_EXSRAM_END_ADDR    (0x17FFFFu)
#define DEF_EXSRAM_SIZE        (0x000100u)//(0x080000u)	//512K Word

BOOL Bsp_SramSelfTest(void);
void Bsp_SramInit(void);

#ifdef __cplusplus
} /* extern "C" */
#endif  /* __cplusplus */

#endif
