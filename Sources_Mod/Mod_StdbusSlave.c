#include "App_Include.h"


#define DEF_STDBUS_SLAVE_PORT  &COM3
#define DEF_STDBUS_SLAVE_BUFF_LEN   1024
#define DEF_STDBUS_SLAVE_DEV_LEN    10


static INT8U auc_Buff[DEF_STDBUS_SLAVE_BUFF_LEN] = {0};
static StdbusDev_t* ast_DevList[DEF_STDBUS_SLAVE_DEV_LEN] = {0};

StdbusPort_t st_StdbusSlave ={
    (INT8U*)"从机端口",                             /* 端口名称 */
    e_StdbusIdle,                           /* 状态 */
    NULL,                                   /* 端口占用锁  OS情况下是一个信号量 */
    NULL,                                   /* 端口消息 OS情况下是一个消息队列*/
    DEF_STDBUS_SLAVE_PORT,                  /* 串口句柄 */
    NULL,                                   /* 主机句柄 */
    ast_DevList,                            /* 设备列表 */
    DEF_STDBUS_SLAVE_DEV_LEN,               /* 设备列表长度 */
    0,                                      /* 已使用的设备个数 */

    0,                                      /* 最后一个字节 */
    auc_Buff,                               /* 数据缓冲区 */
    DEF_STDBUS_SLAVE_BUFF_LEN,              /* 缓冲区大小 */
    0,                                      /* 有效数据的长度 */
    0,                                      /* 发送数组索引 */

    {
        {0,0,0,0},                          /*保留*/
        0,                                  /*地址列表长度*/
        0,                                  /*当前位置*/
        0,                                  /*改数组*/
        0,                                  /*功能命令*/
        0,                                  /*辅助命令 0x55 0x66 0xaa 0x99*/
        0,                                  /*数据载荷长度*/
        0,                                  /*数据载荷*/
    },
};


StdbusDev_t st_PC = {
    (INT8U*)"上位机",                              /* 设备名称 */
    {0,0x00},                               /*地址列表*/
    2,                                      /*地址列表长度*/
    NULL,                                   /*端口句柄*/
    NULL,                                   /*处理函数*/
};


static void Mod_SendComplete(void * pv_dev)
{
    Mod_StdbusPortSendOneByte(&st_StdbusSlave);
}

static void Mod_RecvReady(void * pv_dev, INT8U* puch_Buff, INT16U uin_Len)
{
    INT16U   i;
    for(i = 0; i < uin_Len; i++)
        Mod_StdbusPortRecvOneByte(&st_StdbusSlave,puch_Buff[i]);
}

static void Mod_ErrHandle(void * pv_dev)
{
    Mod_StdbusRscPack(&st_StdbusSlave);                  //释放本端口的数据
    Bsp_UartClose(pv_dev);
    Bsp_UartOpen(pv_dev);
}

void Mod_StdbusSlaveInit(void)
{
    Mod_StdbusRegPort(&st_StdbusHost, &st_StdbusSlave);      //注册端口到协议栈
    Mod_StdbusRegDev(&st_StdbusSlave,&st_PC);                //注册设备到端口
    ((Dev_SerialPort*)st_StdbusSlave.pv_Handle)->cb_SendComplete = Mod_SendComplete;
    ((Dev_SerialPort*)st_StdbusSlave.pv_Handle)->cb_RecvReady =  Mod_RecvReady;
    ((Dev_SerialPort*)st_StdbusSlave.pv_Handle)->cb_ErrHandle = Mod_ErrHandle;
}

void Mod_StdbusSlavePoll(void)
{
    Mod_StdbusPortPoll(&st_StdbusSlave);
}
