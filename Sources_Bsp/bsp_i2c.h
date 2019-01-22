#ifndef        __BSP_I2C_H__
#define        __BSP_I2C_H__

#include "bsp.h"


typedef struct  {
    void (*SDA_H)(void);
    void (*SDA_L)(void);
    INT8U (*SDA_Read)(void);
    void (*SCL_H)(void);
    void (*SCL_L)(void);
}I2cHandle_t;

extern I2cHandle_t Bsp_At24c512;

void Bsp_I2cDelay(void);

void  Bsp_I2cInit(I2cHandle_t* pst_Dev);

INT8U Bsp_I2cStart(I2cHandle_t* pst_Dev);

void Bsp_I2cStop(I2cHandle_t* pst_Dev);

void Bsp_I2cAck(I2cHandle_t* pst_Dev);

void Bsp_I2cNoAck(I2cHandle_t* pst_Dev);

INT8U Bsp_I2cWaitAck(I2cHandle_t* pst_Dev);

void Bsp_I2cSendByte(I2cHandle_t* pst_Dev,INT8U SendByte);

INT8U Bsp_I2cReceiveByte(I2cHandle_t* pst_Dev);

#endif          //__BSP_I2C_H__

