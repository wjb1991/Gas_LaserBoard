#include "Bsp.h"

I2cHandle_t Bsp_At24c512;

static void Bsp_E2pomSDAH(void) 
{
    GPIO_WritePin(2,1);
}

static void Bsp_E2pomSDAL(void) 
{
    GPIO_WritePin(2,0);
}

static void Bsp_E2pomSCLH(void)
{
    GPIO_WritePin(3,1);
}

static void Bsp_E2pomSCLL(void)
{
    GPIO_WritePin(3,0);
}

static INT8U Bsp_E2pomSDAValue(void)
{
    return (INT8U)(GPIO_ReadPin(2));
}


void  Bsp_I2cInit(I2cHandle_t* pst_Dev)
{
  
    if( pst_Dev == 0)
        return;
    
    if( pst_Dev == &Bsp_At24c512 )
    {
        /* IO初始化 */
    	//scl
    	GPIO_SetupPinMux(3, GPIO_MUX_CPU1, 0);
    	GPIO_SetupPinOptions(3, GPIO_OUTPUT, GPIO_PULLUP);
    	//sda
    	GPIO_SetupPinMux(2, GPIO_MUX_CPU1, 0);
    	GPIO_SetupPinOptions(2, GPIO_OUTPUT, GPIO_PULLUP);
        
        /* 注册IO操作函数 */
        pst_Dev->SDA_H = Bsp_E2pomSDAH;
        pst_Dev->SDA_L = Bsp_E2pomSDAL;
        pst_Dev->SCL_H = Bsp_E2pomSCLH;
        pst_Dev->SCL_L = Bsp_E2pomSCLL;
        pst_Dev->SDA_Read = Bsp_E2pomSDAValue; 
        
        Bsp_I2cStop(pst_Dev);
    }

}

//==================================================================================================
//| 函数名称 | Bsp_I2cDelay
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C延时
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_I2cDelay(void) 
{ 
	Bsp_DelayUs(5);
}

//==================================================================================================
//| 函数名称 | Bsp_I2cStart
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C启动
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
INT8U Bsp_I2cStart(I2cHandle_t* pst_Dev)
{ 
    (pst_Dev->SDA_H)(); 
    (pst_Dev->SCL_H)();
    
    Bsp_I2cDelay();
    if ( (pst_Dev->SDA_Read)() == 0 )
        return 0; //SDA线为低电平则总线忙,退出 
    
    
    (pst_Dev->SDA_L)(); 
    Bsp_I2cDelay(); 
    if ( (pst_Dev->SDA_Read)() != 0 )
        return 0; //SDA线为高电平则总线出错,退出 
    
    (pst_Dev->SCL_L)();
    Bsp_I2cDelay(); 
    return 1; 
} 

//==================================================================================================
//| 函数名称 | Bsp_I2cStop
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C1停止位
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_I2cStop(I2cHandle_t* pst_Dev) 
{ 
    (pst_Dev->SCL_L)();
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_L)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SCL_H)();
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_H)();
    Bsp_I2cDelay(); 
} 

//==================================================================================================
//| 函数名称 | Bsp_I2cAck
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C1应答
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_I2cAck(I2cHandle_t* pst_Dev) 
{ 
    (pst_Dev->SCL_L)();
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_L)();
    Bsp_I2cDelay(); 
    (pst_Dev->SCL_H)();
    Bsp_I2cDelay(); 
    (pst_Dev->SCL_L)();
    Bsp_I2cDelay(); 
} 

//==================================================================================================
//| 函数名称 | Bsp_I2cNoAck
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C1不应答
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_I2cNoAck(I2cHandle_t* pst_Dev) 
{ 
    (pst_Dev->SCL_L)();
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_H)();
    Bsp_I2cDelay(); 
    (pst_Dev->SCL_H)();
    Bsp_I2cDelay(); 
    (pst_Dev->SCL_L)();
    Bsp_I2cDelay(); 
}

//==================================================================================================
//| 函数名称 | Bsp_I2cWaitAck
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C1等待应答
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
INT8U Bsp_I2cWaitAck(I2cHandle_t* pst_Dev)
{ 
    (pst_Dev->SCL_L)();
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_H)();
    Bsp_I2cDelay(); 
    (pst_Dev->SCL_H)();
    Bsp_I2cDelay(); 
    if ((pst_Dev->SDA_Read)())
    {
        (pst_Dev->SCL_L)();
        return 0; 
    }
    else
    {
        (pst_Dev->SCL_L)();
        return 1;     
    }
} 

//==================================================================================================
//| 函数名称 | Bsp_I2cSendByte
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C发送数据
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void Bsp_I2cSendByte(I2cHandle_t* pst_Dev,INT8U SendByte)
{ 
    INT8U i = 8;
    while (i--)
    {
        (pst_Dev->SCL_L)();
        Bsp_I2cDelay(); 
        
        if ( SendByte & 0x80 )
            (pst_Dev->SDA_H)();
        else
            (pst_Dev->SDA_L)(); 
        
        SendByte<<=1;
        
        Bsp_I2cDelay(); 
        (pst_Dev->SCL_H)();
        Bsp_I2cDelay(); 
    } 
    (pst_Dev->SCL_L)();
} 

//==================================================================================================
//| 函数名称 | Bsp_I2cReceiveByte
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | I2C1接收数据
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
INT8U Bsp_I2cReceiveByte(I2cHandle_t* pst_Dev)
{  
    INT8U i=8;
    INT8U ReceiveByte=0;

    (pst_Dev->SDA_H)();
    while (i--)
    {
        ReceiveByte<<=1;       
        (pst_Dev->SCL_L)();
        Bsp_I2cDelay(); 
        (pst_Dev->SCL_H)();
        Bsp_I2cDelay(); 
        if ((pst_Dev->SDA_Read)())
        {
            ReceiveByte|=0x01; 
        }
    } 
    (pst_Dev->SCL_L)();
    return ReceiveByte; 
} 
#if 0
//==================================================================================================
//| 函数名称 | I2C1_WriteByte
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 写入1字节数据
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
INT8U I2C1_BufferWrite(I2cHandle_t* pst_Dev, INT8U* pBuffer, uint16_t length,uint16_t WriteAddress, INT8U DeviceAddress)
{
    //关中断
	__disable_irq();
	
	if(!Bsp_I2cStart(pst_Dev))
	{
	    //开中断
	    __enable_irq();
		return 0;
	}
    Bsp_I2cSendByte(pst_Dev,DeviceAddress);           //设置器件地址
    if(!Bsp_I2cWaitAck(pst_Dev))
	{
		Bsp_I2cStop(pst_Dev); 
		//开中断
	    __enable_irq();
		return 0;
	}

	Bsp_I2cSendByte(pst_Dev,(INT8U)(WriteAddress>>8));       //设置起始地址(高)
    Bsp_I2cWaitAck(pst_Dev);
	
	Bsp_I2cSendByte(pst_Dev,(INT8U)(WriteAddress));         //设置起始地址(低)
    Bsp_I2cWaitAck(pst_Dev);
	
    while(length--)
    {
      Bsp_I2cSendByte(pst_Dev,*pBuffer);
      Bsp_I2cWaitAck(pst_Dev);
      pBuffer++;
    }
    Bsp_I2cStop(pst_Dev);

	AT24C512_WRITE_PAGE_DLY();
	
	//开中断
	__enable_irq();

    return 1;
} 

//==================================================================================================
//| 函数名称 | I2C1_ReadByte
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 读出1串数据 
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================	
void I2C1_PageWrite(I2cHandle_t* pst_Dev,INT8U* pBuffer,uint16_t length,uint16_t WriteAddress,INT8U DeviceAddress)
{
	/*
    INT8U NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
    Addr  = WriteAddress % AT24C512_PGE_SZE;  //写入地址是开始页的第几位
    count = AT24C512_PGE_SZE - Addr;          //在开始页要写入的个数
    NumOfPage   =  length / AT24C512_PGE_SZE; //要写入的页数
    NumOfSingle =  length % AT24C512_PGE_SZE; //不足一页的个数

    if(Addr == 0)         //写入地址是页的开始
    {
      if(NumOfPage == 0)  //数据小于一页
      {
        I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress);   //写少于一页的数据
      }
      else                //数据大于等于一页
      {
        while(NumOfPage)//要写入的页数
        {
          I2C1_BufferWrite(pBuffer,AT24C512_PGE_SZE,WriteAddress,DeviceAddress);//写一页的数据
          WriteAddress +=  AT24C512_PGE_SZE;
          pBuffer      +=  AT24C512_PGE_SZE;
          NumOfPage--;
          AT24C512_WRITE_PAGE_DLY();
         }
        if(NumOfSingle!=0)//剩余数据小于一页
        {
          I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress); //写少于一页的数据
          AT24C512_WRITE_PAGE_DLY();
        }
       }
    }
    else                  //写入地址不是页的开始
    {
      if(NumOfPage== 0)   //数据小于一页
      {
        I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress);   //写少于一页的数据
      }
      else                //数据大于等于一页
      {
        length       -= count;
        NumOfPage     = length / AT24C512_PGE_SZE;  //重新计算要写入的页数
        NumOfSingle   = length % AT24C512_PGE_SZE;  //重新计算不足一页的个数

        if(count != 0)
        {
          I2C1_BufferWrite(pBuffer,count,WriteAddress,DeviceAddress);      //将开始的空间写满一页
          WriteAddress += count;
          pBuffer      += count;
         }

        while(NumOfPage--)  //要写入的页数
        {
          I2C1_BufferWrite(pBuffer,AT24C512_PGE_SZE,WriteAddress,DeviceAddress);//写一页的数据
          WriteAddress +=  AT24C512_PGE_SZE;
          pBuffer      +=  AT24C512_PGE_SZE;
        }
        if(NumOfSingle != 0)//剩余数据小于一页
        {
          I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress); //写少于一页的数据
        }
      }
    }
	*/
	I2C1_BufferWrite(pst_Dev, pBuffer,length,WriteAddress,DeviceAddress);
} 

//==================================================================================================
//| 函数名称 | I2C1_ReadByte
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 读出1串数据 
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================	
INT8U I2C1_ReadByte(I2cHandle_t* pst_Dev,INT8U* pBuffer,uint16_t length,uint16_t ReadAddress,INT8U DeviceAddress)
{       
    
	if (!Bsp_I2cStart(pst_Dev))
	    return 0;

    Bsp_I2cSendByte(pst_Dev,DeviceAddress);
	if (!Bsp_I2cWaitAck(pst_Dev))
    {
        Bsp_I2cStop(pst_Dev); 
		return 0;
    }
	Bsp_I2cSendByte(pst_Dev,(INT8U)(ReadAddress>>8));   //设置数据地址(高)
    Bsp_I2cWaitAck(pst_Dev);
	
	Bsp_I2cSendByte(pst_Dev,(INT8U)(ReadAddress));   //设置数据地址(低)
    Bsp_I2cWaitAck(pst_Dev);

	Bsp_I2cDelay();
	
    if (!Bsp_I2cStart(pst_Dev))
	    return 0;

	Bsp_I2cSendByte(pst_Dev,DeviceAddress|0x01);   //设置读地址      
    Bsp_I2cWaitAck(pst_Dev);

    while (length)
    {
        *pBuffer = Bsp_I2cReceiveByte(pst_Dev);
        if (length == 1)
		    Bsp_I2cNoAck(pst_Dev);
        else 
		    Bsp_I2cAck(pst_Dev); 
        pBuffer++;
        length--;
    }
    Bsp_I2cStop(pst_Dev);

    return 1;
}

#endif
