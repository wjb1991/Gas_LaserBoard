#include "Bsp.h"


uint8_t Bsp_At24c512Read(uint8_t *pBuffer,uint16_t uin_Addr, uint16_t uin_Size)
{
    //关中断
	Bsp_IntDis();
  
    /* 发送开始位 */
	if (!Bsp_I2cStart(&Bsp_At24c512))                   
	{
	    //开中断
	    Bsp_IntEn();
		return 0;
	}
    
    /* 发送设备地址 写命令 */
    Bsp_I2cSendByte(&Bsp_At24c512,AT24C512_ADDR);
	if (!Bsp_I2cWaitAck(&Bsp_At24c512))
    {
        Bsp_I2cStop(&Bsp_At24c512); 
	    //开中断
        Bsp_IntEn();
		return 0;
    }
    
    /* 发送内存地址 */
	Bsp_I2cSendByte(&Bsp_At24c512,(uint8_t)(uin_Addr>>8));   //设置数据地址(高)      
    Bsp_I2cWaitAck(&Bsp_At24c512);
	
	Bsp_I2cSendByte(&Bsp_At24c512,(uint8_t)(uin_Addr));   //设置数据地址(低)      
    Bsp_I2cWaitAck(&Bsp_At24c512);

	Bsp_I2cDelay();
	
    /* 发送开始位 */
    if (!Bsp_I2cStart(&Bsp_At24c512))
	{
	    //开中断
	    Bsp_IntEn();
		return 0;
	}
    
    /* 发送设备地址 读命令 */
	Bsp_I2cSendByte(&Bsp_At24c512,AT24C512_ADDR|0x01);   //设置读地址      
    Bsp_I2cWaitAck(&Bsp_At24c512);

    /* 读取数据 */
    while (uin_Size)
    {
        *pBuffer = Bsp_I2cReceiveByte(&Bsp_At24c512);
        if (uin_Size == 1)      
		    Bsp_I2cNoAck(&Bsp_At24c512);                //最后一个字节发送NACK结束读取
        else 
		    Bsp_I2cAck(&Bsp_At24c512);                  //发送ACK从机会继续输出数据
        pBuffer++;
        uin_Size--;
    }
    Bsp_I2cStop(&Bsp_At24c512);
    //开中断
    Bsp_IntEn();

    return 1;
}


uint8_t Bsp_At24c512Write(uint8_t *pBuffer,uint16_t uin_Addr, uint16_t uin_Size)
{
    //关中断
    Bsp_IntDis();
    
    /* 发送开始位 */
	if(!Bsp_I2cStart(&Bsp_At24c512))
	{
	    //开中断
	    Bsp_IntEn();
		return 0;
	}
    
    /* 发送设备地址 写命令 */
    Bsp_I2cSendByte(&Bsp_At24c512,AT24C512_ADDR);        
    if(!Bsp_I2cWaitAck(&Bsp_At24c512))
	{
		Bsp_I2cStop(&Bsp_At24c512); 
		//开中断
		Bsp_IntEn();
		return 0;
	}
    
    /* 发送内存地址 */
    Bsp_I2cSendByte(&Bsp_At24c512,(uint8_t)(uin_Addr>>8));       //设置起始地址(高)
    Bsp_I2cWaitAck(&Bsp_At24c512);
	
	Bsp_I2cSendByte(&Bsp_At24c512,(uint8_t)(uin_Addr));         //设置起始地址(低)
    Bsp_I2cWaitAck(&Bsp_At24c512);
    
    /* 写入数据 */
    while(uin_Size--)
    {
      Bsp_I2cSendByte(&Bsp_At24c512,*pBuffer);
      Bsp_I2cWaitAck(&Bsp_At24c512);
      pBuffer++;
    }
    Bsp_I2cStop(&Bsp_At24c512);
    
	//开中断
    Bsp_IntEn();
    Bsp_DelayMs(15);
    return 1;   
}
//==================================================================================================
//| 函数名称 | Bsp_At24c512Init
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_At24c512Init(void) 
{
    Bsp_I2cInit(&Bsp_At24c512);
    Bsp_At24c512SelfTest();
} 
//==================================================================================================
//| 函数名称 | Bsp_At24c512SelfTest
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | AT24C512自检
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
uint8_t Bsp_At24c512SelfTest(void) 
{
	uint16_t i;
	uint8_t Test[10];
	uint8_t Check[10];
    
    //自检EEPROM
	for(i=0; i<10;i++) 
	{
		Test[i] = (uint8_t)i+5;
	}

	Bsp_At24c512Write(Test,0x0,10);

	Bsp_At24c512Read(Check,0x0,10);

	for(i=0;i<10;i++)
	{
		if (Test[i] != Check[i]) 
		{
			break;
		}
	}

	if( i<10 ) 
	{
	    return FALSE;
	} 
	return TRUE;
} 
