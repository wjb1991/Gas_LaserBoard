#include "App_Include.h"

#define DEF_STDBUS_MASTER_PORT  &COM2
#define DEF_STDBUS_MASTER_BUFF_LEN   1024
#define DEF_STDBUS_MASTER_DEV_LEN    1

static INT8U auc_Buff[DEF_STDBUS_MASTER_BUFF_LEN] = {0};

static StdbusDev_t* ast_DevList[DEF_STDBUS_MASTER_DEV_LEN] = {0};


StdbusPort_t st_StdbusMaster ={
                               (INT8U*)"�����˿�",                             /* �˿����� */
    e_StdbusIdle,                           /* ״̬ */
    NULL,                                   /* �˿�ռ����  OS�������һ���ź��� */
    NULL,                                   /* �˿���Ϣ OS�������һ����Ϣ����*/
    DEF_STDBUS_MASTER_PORT,                 /* ���ھ�� */
    NULL,                                   /* ������� */
    ast_DevList,                            /* �豸�б� */
    DEF_STDBUS_MASTER_DEV_LEN,              /* �豸�б����� */
    0,                                      /* ��ʹ�õ��豸���� */

    0,                                      /* ���һ���ֽ� */
    auc_Buff,                               /* ���ݻ����� */
    DEF_STDBUS_MASTER_BUFF_LEN,             /* ��������С */
    0,                                      /* ��Ч���ݵĳ��� */
    0,                                      /* ������������ */

    {
        {0,0,0,0},                          /*����*/
        0,                                  /*��ַ�б�����*/
        0,                                  /*��ǰλ��*/
        0,                                  /*������*/
        0,                                  /*��������*/
        0,                                  /*�������� 0x55 0x66 0xaa 0x99*/
        0,                                  /*�����غɳ���*/
        0,                                  /*�����غ�*/
    },
};


StdbusDev_t st_MainBoard = {
    (INT8U*)"β���������",                         /* �豸���� */
    {0,0x00},                               /*��ַ�б�*/
    2,                                      /*��ַ�б�����*/
    NULL,                                   /*�˿ھ��*/
    NULL,                                   /*��������*/
};


static void Mod_SendComplete(void * pv_dev)
{
    Mod_StdbusPortSendOneByte(&st_StdbusMaster);
}

static void Mod_RecvReady(void * pv_dev, INT8U* puch_Buff, INT16U uin_Len)
{
    INT16U   i;
    for(i = 0; i < uin_Len; i++)
        Mod_StdbusPortRecvOneByte(&st_StdbusMaster,puch_Buff[i]);
}

static void Mod_ErrHandle(void * pv_dev)
{
    Mod_StdbusRscPack(&st_StdbusMaster);                    //�ͷű��˿ڵ�����
    Bsp_UartClose(pv_dev);                                  //�رն˿�
    Bsp_UartOpen(pv_dev);                                   //���´򿪶˿�
}

void Mod_StdbusMasterInit(void)
{
    Mod_StdbusRegPort(&st_StdbusHost,&st_StdbusMaster);     //ע��˿ڵ�Э��ջ
    Mod_StdbusRegDev(&st_StdbusMaster,&st_MainBoard);              //ע���豸���˿�
    ((Dev_SerialPort*)st_StdbusMaster.pv_Handle)->cb_SendComplete = Mod_SendComplete;   //������ɻص�
    ((Dev_SerialPort*)st_StdbusMaster.pv_Handle)->cb_RecvReady =  Mod_RecvReady;        //������ɻص�
    ((Dev_SerialPort*)st_StdbusMaster.pv_Handle)->cb_ErrHandle = Mod_ErrHandle;         //�������ص�
}

void Mod_StdbusMasterPoll(void)
{
    Mod_StdbusPortPoll(&st_StdbusMaster);
}