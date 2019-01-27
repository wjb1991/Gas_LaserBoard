#include "App_Include.h"

#define DEF_MASTERDEAL_DBG_EN           TRUE

#if (DEF_MASTERDEAL_DBG_EN == TRUE)
    #define MASTERDEAL_DBG(...)        do {                            \
                                            TRACE_DBG(__VA_ARGS__);    \
                                        }while(0)
#else
    #define MASTERDEAL_DBG(...)
#endif



typedef enum
{
    CMD_R_CONNECT = 0x00,

    //红外相关命令
    CMD_RW_IR_SINVPP = 0x20,
    CMD_RW_IR_SINFREQ,
    CMD_RW_IR_TRGVPP,
    CMD_RW_IR_SAMPLEFREQ,
    CMD_RW_IR_DCVOLT,
    CMD_RW_IR_SINPHASE,
    CMD_RW_IR_TECVOLAT = 0x26,

    CMD_RW_SYS_LASER_TEMP = 0x2a,
    CMD_RW_SYS_PCB_TEMP = 0x2c,

    //紫外相关命令
    CMD_R_SPE_STATE = 0x40,
    CMD_R_SPE_SERIALNUM,
    CMD_R_SPE_WLCCOEFF,
    CMD_R_SPE_NLCCOEFF,
    CMD_R_SPE_NLCORDER,
    CMD_R_SPE_PIXELS,
    CMD_RW_SPE_INTERGRALTIME,
    CMD_RW_SPE_EDC,
    CMD_RW_SPE_NLC,
    CMD_RW_SPE_SCANAVG,
    CMD_RW_SPE_BOXCAR,
    CMD_R_SPE_SPECTRUM,

    //读取光谱相关
    CMD_R_IR_ACWAVE = 0xC0,
    CMD_R_IR_DCWAVE,
    CMD_R_IR_RAW_SPECTRUM,      //原始吸收峰
    CMD_R_IR_PROC_SPECTRUM,     //AC/DC吸收峰
    CMD_R_IR_ZERO_SPECTRUM,     //调零 吸收峰
    CMD_R_IR_GALIBGAS1_SPECTRUM,//标定气体1 吸收峰
    CMD_R_IR_GALIBGAS2_SPECTRUM,//标定气体2 吸收峰
    CMD_R_IR_BKG_SPECTRUM,      //背景 吸收峰
    CMD_R_IR_DIFF_SPECTRUM,     //差分 吸收峰
}eLasterBoardCmd;


typedef union {

    INT16U auin_Buff[10000];
    FP32   af_Buff[5000];
}ComTemp_t;


#ifdef __cplusplus
#pragma DATA_SECTION("Exsram")
#else
//#pragma DATA_SECTION(un_Temp,"Exsram");
#endif
ComTemp_t un_Temp;

BOOL App_StdbusMasterDealFram(StdbusFram_t* pst_Fram)
{
    INT16U i;
    BOOL res = FALSE;
    switch(pst_Fram->uch_Cmd)
    {
#if TRUE //系统控制相关
//==================================================================================
//                            心跳包 没有任何数据确定是否连接
//==================================================================================
    case CMD_R_CONNECT:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 0;
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 接收到心跳包\r\n");
        }
        break;
//==================================================================================
//                                  读取激光器温度
//==================================================================================
    case CMD_RW_SYS_LASER_TEMP:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],st_LaserTEC.f_FbTemper,FALSE);
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                  读取pcb温度
//==================================================================================
    case CMD_RW_SYS_PCB_TEMP:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            FP32 f_temp = Mod_GetTemper(&st_PcbTemper);

            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],f_temp,FALSE);
            res = TRUE;    //应答
        }
        break;
#endif

#if TRUE //紫外相关
//==================================================================================
//                            获取光谱仪状态
//==================================================================================
    case CMD_R_SPE_STATE:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = (INT8U)st_Usb4000.e_State;
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 获取光谱仪连接状态\r\n");
        }
        break;
//==================================================================================
//                            获取光谱仪序列号
//==================================================================================
    case CMD_R_SPE_SERIALNUM:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = strlen((const char*)st_Usb4000.auc_SerialNumber);
            memcpy(&pst_Fram->puc_PayLoad[0],st_Usb4000.auc_SerialNumber,pst_Fram->uin_PayLoadLenth);
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 获取光谱仪序列号 = %s\r\n", st_Usb4000.auc_SerialNumber);
        }
        break;
//==================================================================================
//                            获取光谱仪波长拟合因子
//==================================================================================
    case CMD_R_SPE_WLCCOEFF:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 4*4;

            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0], st_Usb4000.af_WlcCoeff[0], FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[4], st_Usb4000.af_WlcCoeff[1], FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[8], st_Usb4000.af_WlcCoeff[2], FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[12], st_Usb4000.af_WlcCoeff[3], FALSE);
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 获取光谱仪波长拟合因子\r\n");
            for( i = 0; i < 4 ;i++)
                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 拟合因子%d = %e\r\n", i, st_Usb4000.af_WlcCoeff[i]);
        }
        break;
//==================================================================================
//                            获取光谱仪非线性矫正因子
//==================================================================================
    case CMD_R_SPE_NLCCOEFF:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 8*4;

            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0], st_Usb4000.af_NlcCoeff[0], FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[4], st_Usb4000.af_NlcCoeff[1], FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[8], st_Usb4000.af_NlcCoeff[2], FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[12], st_Usb4000.af_NlcCoeff[3], FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[16], st_Usb4000.af_NlcCoeff[4], FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[20], st_Usb4000.af_NlcCoeff[5], FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[24], st_Usb4000.af_NlcCoeff[6], FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[28], st_Usb4000.af_NlcCoeff[7], FALSE);

            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 获取光谱仪非线性矫正因子\r\n");
            for( i = 0; i < 8 ;i++)
                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 矫正因子%d = %e\r\n", i, st_Usb4000.af_NlcCoeff[i]);
        }
        break;
//==================================================================================
//                            获取光谱仪非线性矫正阶数
//==================================================================================
    case CMD_R_SPE_NLCORDER:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_Usb4000.uch_NlcOrder;
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 获取光谱仪拟合阶数 = %d\r\n",st_Usb4000.uch_NlcOrder);
        }
        break;
//==================================================================================
//                            获取光谱仪像素
//==================================================================================
    case CMD_R_SPE_PIXELS:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 2;
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Usb4000.uin_Pixels,FALSE);
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 获取光谱仪像素 = %d\\r\n",st_Usb4000.uin_Pixels);
        }
//==================================================================================
//                            设置/获取光谱仪积分时间
//==================================================================================
    case CMD_RW_SPE_INTERGRALTIME:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[0],st_Usb4000.ul_IntegralTime,FALSE);
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 获取光谱仪积分时间 = %d\r\n",st_Usb4000.ul_IntegralTime);
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //写命令
            if(pst_Fram->uin_PayLoadLenth == 4)
                st_Usb4000.ul_IntegralTime = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[0],FALSE);
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 设置光谱仪积分时间  = %d\r\n",st_Usb4000.ul_IntegralTime);
        }
        break;
//==================================================================================
//                            设置/获取光谱仪暗噪声修正
//==================================================================================
    case CMD_RW_SPE_EDC:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_Usb4000.b_EdcEnable;
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 获取光谱仪暗噪声修正 = %d\r\n",st_Usb4000.b_EdcEnable);
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //写命令
            if(pst_Fram->uin_PayLoadLenth == 1)
                st_Usb4000.b_EdcEnable = pst_Fram->puc_PayLoad[0];
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 设置光谱仪暗噪声修正  = %d\r\n",st_Usb4000.b_EdcEnable);
        }
        break;
//==================================================================================
//                            设置/获取光谱仪非线性修正
//==================================================================================
    case CMD_RW_SPE_NLC:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_Usb4000.b_EdcEnable;
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 获取光谱仪非线性修正 = %d\r\n",st_Usb4000.b_NlcEnable);
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //写命令
            if(pst_Fram->uin_PayLoadLenth == 1)
                st_Usb4000.b_EdcEnable = pst_Fram->puc_PayLoad[0];
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 设置光谱仪非线性修正  = %d\r\n",st_Usb4000.b_NlcEnable);
        }
        break;
//==================================================================================
//                            设置/获取光谱仪扫描平均次数
//==================================================================================
    case CMD_RW_SPE_SCANAVG:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_Usb4000.uch_ScansToAverage;
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 获取光谱仪扫描平均次数 = %d\r\n",st_Usb4000.uch_ScansToAverage);
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //写命令
            if(pst_Fram->uin_PayLoadLenth == 1)
                st_Usb4000.uch_ScansToAverage = pst_Fram->puc_PayLoad[0];
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 设置光谱仪扫描平均次数  = %d\r\n",st_Usb4000.uch_ScansToAverage);
        }
        break;
//==================================================================================
//                            设置/获取光谱仪滑动滤波
//==================================================================================
    case CMD_RW_SPE_BOXCAR:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_Usb4000.uch_Boxcar;
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 获取光谱仪滑动滤波 = %d\r\n",st_Usb4000.uch_Boxcar);
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //写命令
            if(pst_Fram->uin_PayLoadLenth == 1)
                st_Usb4000.uch_Boxcar = pst_Fram->puc_PayLoad[0];
            res = TRUE;    //应答

            MASTERDEAL_DBG(">>MASTERDEAL_DBG: 设置光谱仪扫滑动滤波  = %d\r\n",st_Usb4000.uch_Boxcar);
        }
        break;
//==================================================================================
//                            获取光谱仪光谱(原始光谱)
//==================================================================================
    case CMD_R_SPE_SPECTRUM:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                //加载光谱到内存
                for(i = 0; i < st_Usb4000.uin_Pixels; i++)
                    un_Temp.auin_Buff[i] = (INT16U)st_Usb4000.af_ProcessSpectrum[i];

                //读取第一页返回数组长度
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0], st_Usb4000.uin_Pixels, FALSE);
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;    //应答

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 加载光谱到内存\r\n");
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                //第一二个字节是ReadAddress 第二三个字节是ReadLenth
                INT16U uin_Offset = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0], FALSE);
                INT16U uin_Lenth = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[2], FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 2;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[i*2+4],un_Temp.auin_Buff[uin_Offset+i],FALSE);
                }
                res = TRUE;    //应答
            }
        }
        break;
#endif

#if TRUE //红外相关
//==================================================================================
//                                  设置正弦波幅值
//==================================================================================
    case CMD_RW_IR_SINVPP:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                FP32 f_Temp = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[0],FALSE);

                if (Mod_SetSinVpp(&st_ModWave,f_Temp,TRUE) == TRUE)
                {
                    res = TRUE;    //应答
                }
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],st_ModWave.f_SinVpp,FALSE);
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                  设置正弦波频率
//==================================================================================
    case CMD_RW_IR_SINFREQ:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                FP32 f_Temp = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[0],FALSE);

                if (Mod_SetSinFreq(&st_ModWave,f_Temp,TRUE) == TRUE)
                {
                    st_DLia.f_PsdFreq = st_ModWave.f_SinFreq * 2;       /* 放大正弦波的二次谐波   */
                    res = TRUE;    //应答
                }
            }

        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],st_ModWave.f_SinFreq,FALSE);
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                  设置三角波幅值
//==================================================================================
    case CMD_RW_IR_TRGVPP:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                FP32 f_Temp = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[0],FALSE);

                if (Mod_SetTrgVpp(&st_ModWave,f_Temp,TRUE) == TRUE)
                {
                    res = TRUE;    //应答
                }
            }

        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],st_ModWave.f_TrgVpp,FALSE);
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                  设置采样频率
//==================================================================================
    case CMD_RW_IR_SAMPLEFREQ:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                FP32 f_Temp = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[0],FALSE);

                if (Mod_SetSampleFreq(&st_ModWave,f_Temp,TRUE) == TRUE)
                {
                    st_DLia.f_SampleFreq = st_ModWave.f_SampleFreq;    /* 采样频率    */
                    res = TRUE;    //应答
                }
            }

        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],st_ModWave.f_SampleFreq,FALSE);
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                  设置直流偏置
//==================================================================================
    case CMD_RW_IR_DCVOLT:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                FP32 f_Temp = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[0],FALSE);

                if (Mod_SetDcOffset(&st_ModWave,f_Temp,TRUE) == TRUE)
                {
                    res = TRUE;    //应答
                }
            }

        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],st_ModWave.f_DcOffset,FALSE);
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                                  设置正弦波相位
//==================================================================================
    case CMD_RW_IR_SINPHASE:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                FP32 f_Temp = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[0],FALSE);

                if (Mod_DLiaSetPhase(&st_DLia,f_Temp) == TRUE)
                {
                    res = TRUE;    //应答
                }
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 4;
            //Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],st_DLia.f_PsdPhase,FALSE);
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                                  设置TEC电压
//==================================================================================
    case CMD_RW_IR_TECVOLAT:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                FP32 f_Temp = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[0],FALSE);
                if(Mod_TecSetVolt(&st_LaserTEC, f_Temp) == TRUE)
                {
                    res = TRUE;    //应答
                }
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],st_LaserTEC.f_SetCtrlVolt,FALSE);
            res = TRUE;    //应答
        }
        break;
#if 0
//==================================================================================
//                                  设置波形平均次数
//==================================================================================
    case 0x27:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 2)
            {
                INT16U uin_DataTemp;
                uin_DataTemp = ((INT32U)pst_Fram->puc_PayLoad[1]<<8) + ((INT32U)pst_Fram->puc_PayLoad[0]);

                if ((uin_DataTemp>=1)&&(uin_DataTemp<=9999))
                {
                    uin_SampleWaveTotalNum = uin_DataTemp;
                    str_Sample.uin_WaveNum = 0;
                    s_WriteOneIntToEeprom(uin_DataTemp,0xc8);
                    uch_ThrowWaveEn = 1;//用于丢波形的功能开启  by guxiaohua
                    uin_100usCount = 0;
                    uin_1sCount = 0;
                    s_ClrGlobalArray();  //s_DoubleGas_ClrGasConcenStruct();
                    sprintf(buf,"浓度计算平均次数已被设置为: %u\r\n",uin_SampleWaveTotalNum);
                    TRACE_DBG(buf);
                }

                res = TRUE;    //应答
            }

        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 2;

            INT16U j = uin_SampleWaveTotalNum;

            pst_Fram->puc_PayLoad[1] = (INT8U)(j>>8);
            pst_Fram->puc_PayLoad[0] = (INT8U)(j&0xff);

            res = TRUE;    //应答
        }
        break;
#endif
#endif

#if TRUE //读取光谱相关
//==================================================================================
//                                   获取传感器交流波形
//==================================================================================
    case CMD_R_IR_ACWAVE:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
             //第一个字节是PageIndex  第二三个字节是ReadAddress 第四五个字节是ReadLenth
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                //读取第一页返回数组长度
                pst_Fram->uin_PayLoadLenth = 2;
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_ModWave.uin_SampleDot,FALSE);
                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱*/
                Bsp_IntDis();
                for(i = 0; i < st_ModWave.uin_SampleDot; i++)
                {
                    un_Temp.auin_Buff[i] = i;//st_ModWave.puin_RecvBuff[i];
                }
                Bsp_IntEn();

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 加载传感器交流波形到缓冲区 %d个点\r\n",i);
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                uint16_t i = 0;
                uint16_t uin_Offset = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad,FALSE);
                uint16_t uin_Lenth = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad + 2,FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 2;
                for(i = 0; i<uin_Lenth;i++)
                {
                    INT16U j = un_Temp.auin_Buff[uin_Offset + i];

                    Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[i * 2 + 4],j,FALSE);
                }

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 发送缓冲区数据 Offset = %d 长度 = %d \r\n",uin_Offset,i);
            }
            res = TRUE;    //应答
        }
        break;
#endif

    default:
        break;
    }

#if 0
    switch(pst_Fram->uch_Cmd)
    {
//==================================================================================
//                                获取光谱仪状态 连上or未连上
//==================================================================================
    case 0x10:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.b_IsConnect;
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                                设置光谱仪积分时间
//==================================================================================
    case 0x11:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                INT32U  i;

                TRACE_DBG(">>DBG>>      设置光谱仪积分时间\n\r");

                i = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[0],FALSE);

                if( i > 65000000)
                    i = 65000000;

                USB4000.b_SetFlag = TRUE;
                USB4000.ul_SetIntegralTime = i;
                SaveToEeprom((INT32U)&USB4000.ul_SetIntegralTime);
                res = TRUE;    //应答
            }

        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 4;

            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[0],USB4000.ul_SetIntegralTime,FALSE);
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                              设置光谱仪求和平均次数
//==================================================================================
    case 0x12:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.uch_ScansToAverage = pst_Fram->puc_PayLoad[0];
                SaveToEeprom((INT32U)&USB4000.uch_ScansToAverage);
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.uch_ScansToAverage;
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                              设置光谱仪滑动滤波次数
//==================================================================================
    case 0x13:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.uch_Boxcar = pst_Fram->puc_PayLoad[0];
                SaveToEeprom((INT32U)&USB4000.uch_Boxcar);
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.uch_Boxcar;
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                              设置光谱仪是否开启EDC(暗噪声补偿)
//==================================================================================
    case 0x14:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.b_EdcEnable = pst_Fram->puc_PayLoad[0];
                SaveToEeprom((INT32U)&USB4000.b_EdcEnable);
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.b_EdcEnable;
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                              设置光谱仪是否开启NLC(非线性补偿)
//==================================================================================
    case 0x15:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                USB4000.b_NlcEnable = pst_Fram->puc_PayLoad[0];
                SaveToEeprom((INT32U)&USB4000.b_NlcEnable);
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = USB4000.b_NlcEnable;
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                            切换动态静态测量模式
//==================================================================================
	case 0x18:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{

		}
		else if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
				if(pst_Fram->puc_PayLoad[0] == 0)
				{
					Mod_MeasureDoStaticMeasure(&st_Measure);
				}
				else
				{
					Mod_MeasureDoDynamicMeasure(&st_Measure);
				}
				res = TRUE;    //应答
            }
        }

		break;

//==================================================================================
//                          修改一个标定点/读取一个标定点
//==================================================================================
    case 0x22:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //写命令是修改一个标定点   byte0 byte0 点索引 byte1参数索引 byte2-byte9 double 参数
            if(pst_Fram->uin_PayLoadLenth == 11)
            {
                CalibPoint_t point;
                point.b_Use = pst_Fram->puc_PayLoad[2];
                point.f_X = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[3],FALSE);
                point.f_Y = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[7],FALSE);
                if( pst_Fram->puc_PayLoad[0] == 0 )
                    Mod_CalibPointListEditOnePoint(&st_CPList_GasNO,pst_Fram->puc_PayLoad[1],&point);
                res = TRUE;    //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是读取一个标定点
            //无输入        返回标定点数量
            //输入一个索引  返回指定索引的标定点的数据
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                //读取第一页返回数组长度
                if( pst_Fram->puc_PayLoad[0] == 0 )
                {
                    pst_Fram->puc_PayLoad[1] = DEF_CALIBPOINT_MAX;
                    pst_Fram->uin_PayLoadLenth = 2;
                    res = TRUE;    //应答
                }
 
            }
            else if(pst_Fram->uin_PayLoadLenth == 2)
            {
                CalibPoint_t point;
                if( pst_Fram->puc_PayLoad[0] == 0 )
                {   
                    Mod_CalibPointListReadOnePoint(&st_CPList_GasNO,pst_Fram->puc_PayLoad[1],&point);
                    pst_Fram->puc_PayLoad[2] = point.b_Use;
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[3],point.f_X,FALSE);
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[7],point.f_Y,FALSE);                
                    pst_Fram->uin_PayLoadLenth = 11;        
                    res = TRUE;    //应答
                }
            }

        }
        break;

//==================================================================================
//                                设置阶数/读取阶数
//==================================================================================
    case 0x23:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 2)
            {
                if ( pst_Fram->puc_PayLoad[1] > 3)
                   res = FALSE;

                if( pst_Fram->puc_PayLoad[0] == 0 )
                {
                    st_GasMeasure.pst_Gas1->uch_NiheOrder = pst_Fram->puc_PayLoad[1];
                    SaveToEeprom((INT32U)(&st_GasMeasure.pst_Gas1->uch_NiheOrder));
                    pst_Fram->uin_PayLoadLenth = 2;
                    res = TRUE;    //应答
                }
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                if( pst_Fram->puc_PayLoad[0] == 0 )
                {
                    pst_Fram->puc_PayLoad[1] = st_GasMeasure.pst_Gas1->uch_NiheOrder;
                    pst_Fram->uin_PayLoadLenth = 2;
                    res = TRUE;    //应答
                }
            }
        }

        break;

//==================================================================================
//                                  建立工作曲线
//==================================================================================
    case 0x24:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //写命令是建立工作曲线
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                Mod_GasMarkWorkLine(&st_GasMeasure,pst_Fram->puc_PayLoad[0]);
                res = TRUE;            //应答 不修改数据原始数据返回
            }
        }
        break;
//==================================================================================
//                          设置工作曲线系数/读取工作曲线系数
//==================================================================================
    case 0x25:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //写命令是建立工作曲线
            if(pst_Fram->uin_PayLoadLenth == 0)
            {

            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是读取拟合系数
            //输入两个索引 第一个索引选择哪组拟合系数 第二个索引是选择An
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                if(pst_Fram->puc_PayLoad[0] == 0)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[1],st_GasMeasure.pst_Gas1->af_NiheCoeff[0],FALSE);                
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[5],st_GasMeasure.pst_Gas1->af_NiheCoeff[1],FALSE);   
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[9],st_GasMeasure.pst_Gas1->af_NiheCoeff[2],FALSE);   
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[13],st_GasMeasure.pst_Gas1->af_NiheCoeff[3],FALSE);                
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[17],st_GasMeasure.pst_Gas1->af_NiheCoeff[4],FALSE);   
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[21],st_GasMeasure.pst_Gas1->af_NiheCoeff[5],FALSE);   
                    pst_Fram->uin_PayLoadLenth = 25;
                    res = TRUE;    //应答
                }
            }
        }
        break;

//==================================================================================
//                                   读取10路绿光电压
//==================================================================================
    case 0x30:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                pst_Fram->uin_PayLoadLenth = 40;

                for(i = 0; i < 10; i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4],st_Grey.pst_Channel[i].f_Volt,FALSE);
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   读取10路绿光幅值标定电压
//==================================================================================
    case 0x31:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                pst_Fram->uin_PayLoadLenth = 40;

                for(i = 0; i < 10; i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4],st_Grey.pst_Channel[i].f_AbsTransVolt,FALSE);
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   读取10路绿光背景电压
//==================================================================================
    case 0x32:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                pst_Fram->uin_PayLoadLenth = 40;

                for(i = 0; i < 10; i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4],st_Grey.pst_Channel[i].f_BkVolt,FALSE);
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   读取10路绿光透过率
//==================================================================================
    case 0x33:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                pst_Fram->uin_PayLoadLenth = 40;

                for(i = 0; i < 10; i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4],st_Grey.pst_Channel[i].f_Trans,FALSE);
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   读取10路绿光灰度
//==================================================================================
    case 0x34:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                uint16_t i = 0;
                pst_Fram->uin_PayLoadLenth = 40;

                for(i = 0; i < 10; i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i*4],st_Grey.pst_Channel[i].f_Grey,FALSE);
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   设置绿光工作状态
//==================================================================================
    case 0x3a:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_Grey.e_Status;
            res = TRUE;
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                switch(pst_Fram->puc_PayLoad[0])
                {
                case 0:
                    Mod_GreyGotoIdle(&st_Grey);
                    break;
                case 1:
                    Mod_GreyGotoMeas(&st_Grey);
                    break;
                case 2:
                    Mod_GreyGotoCalib(&st_Grey);
                    break;
                default:
                    return FALSE;
                }
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                         获取绿光工作状态/总透过率/总灰度
//==================================================================================
    case 0x3b:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 9;
            pst_Fram->puc_PayLoad[0] = st_Grey.e_Status;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[1],st_Grey.f_Trans,FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[5],st_Grey.f_Grey,FALSE);
            res = TRUE;
        }
        break;
//==================================================================================
//                                   读取光谱
//==================================================================================
    case 0x40:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                INT16U i = 0;
                INT16U len = 0;
                OS_ERR os_err;

                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱 */
                OSSchedLock(&os_err);
                for(i = st_GasMeasure.ul_UseLeftDot; i < st_GasMeasure.ul_UseRightDot; i++)
                    lf_Buff[len++] = st_GasMeasure.plf_Spectrum[i];
                OSSchedUnlock(&os_err);

                //读取第一页返回数组长度
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0], len, FALSE);
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                //第一二个字节是ReadAddress 第二三个字节是ReadLenth
                INT16U i = 0;
                INT16U uin_Offset = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0], FALSE);
                INT16U uin_Lenth = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[2], FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 8;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[i*8+4],lf_Buff[uin_Offset+i],FALSE);
                }
                res = TRUE;    //应答
            }
        }
        break;

//==================================================================================
//                                  读取标定光谱
//==================================================================================
    case 0x41:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                INT16U i = 0;
                INT16U len = 0;
                OS_ERR os_err;

                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱 */
                OSSchedLock(&os_err);
                for(i = st_GasMeasure.ul_UseLeftDot; i < st_GasMeasure.ul_UseRightDot; i++)
                    lf_Buff[len++] = st_GasMeasure.plf_AbsSpectrum[i];
                OSSchedUnlock(&os_err);

                //读取第一页返回数组长度
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0], len, FALSE);
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                //第一二个字节是ReadAddress 第二三个字节是ReadLenth
                INT16U i = 0;
                INT16U uin_Offset = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0], FALSE);
                INT16U uin_Lenth = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[2], FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 8;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[i*8+4],lf_Buff[uin_Offset+i],FALSE);
                }
                res = TRUE;    //应答
            }
        }
        break;

//==================================================================================
//                                  读取背景光谱
//==================================================================================
    case 0x42:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                INT16U i = 0;
                INT16U len = 0;
                OS_ERR os_err;

                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱 */
                OSSchedLock(&os_err);
                for(i = st_GasMeasure.ul_UseLeftDot; i < st_GasMeasure.ul_UseRightDot; i++)
                    lf_Buff[len++] = st_GasMeasure.plf_BkgSpectrum[i];
                OSSchedUnlock(&os_err);

                //读取第一页返回数组长度
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0], len, FALSE);
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                //第一二个字节是ReadAddress 第二三个字节是ReadLenth
                INT16U i = 0;
                INT16U uin_Offset = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0], FALSE);
                INT16U uin_Lenth = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[2], FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 8;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[i*8+4],lf_Buff[uin_Offset+i],FALSE);
                }
                res = TRUE;    //应答
            }
        }
        break;
//==================================================================================
//                                  读取差分光谱
//==================================================================================
    case 0x43:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                INT16U i = 0;
                INT16U len = 0;
                OS_ERR os_err;

                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱 */
                OSSchedLock(&os_err);
                for(i = st_GasMeasure.ul_UseLeftDot; i < st_GasMeasure.ul_UseRightDot; i++)
                    lf_Buff[len++] = st_GasMeasure.plf_DiffSpectrum[i];
                OSSchedUnlock(&os_err);

                //读取第一页返回数组长度
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0], len, FALSE);
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;    //应答
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                //第一二个字节是ReadAddress 第二三个字节是ReadLenth
                INT16U i = 0;
                INT16U uin_Offset = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0], FALSE);
                INT16U uin_Lenth = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[2], FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 8;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[i*8+4],lf_Buff[uin_Offset+i],FALSE);
                }
                res = TRUE;    //应答
            }
        }
        break;
//==================================================================================
//                                  切换工作模式
//==================================================================================
    case 0x4a:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            //byte0: 0:切换到调零状态
            //       1:切换到标定状态
            //       2:切换到工作状态
            //       3:切换到差分测量状态
            //byte1-5 float :切换到标定状态是时下发的标定浓度
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                switch(pst_Fram->puc_PayLoad[0])
                {
                case eGasAdjZero:
                    Mod_GasMeasureGotoAdjZero(&st_GasMeasure);           //切换到调0状态
                    res = TRUE;    //应答
                    break;
                case eGasAbsMeasure:
                    Mod_GasMeasureGotoAbsMeasure(&st_GasMeasure);        //切换到工作状态
                    res = TRUE;    //应答
                    break;
                case eGasDiffMeasure:
                    Mod_GasMeasureGotoDiffMeasure(&st_GasMeasure);        //切换到工作状态
                    res = TRUE;    //应答
                    break;
                case eGasWait:
                    Mod_GasMeasureGotoWait(&st_GasMeasure);               //切换到等待状态
                    res = TRUE;    //应答
                    break;
                case eGasCalibTrans:
                    Mod_GasMeasureGotoCalibTrans(&st_GasMeasure);         //切换到透过率标定（能量标定）        
                    res = TRUE;    //应答
                    break;
                default:
                    break;
                }
            }
            else if(pst_Fram->uin_PayLoadLenth == 17)
            {
                FP64 f1,f2;

                TRACE_DBG(">>DBG>>      接收到标定命令\n\r");

                f1 = Bsp_CnvArrToFP64(&pst_Fram->puc_PayLoad[1],FALSE);
                f2 = Bsp_CnvArrToFP64(&pst_Fram->puc_PayLoad[9],FALSE);
                
                if(pst_Fram->puc_PayLoad[0] == eGasCalibGas1 || pst_Fram->puc_PayLoad[0] == eGasCalibGas2 ||
                   pst_Fram->puc_PayLoad[0] == eGasCalibAll)
                {
                     Mod_GasMeasureGotoCalib(&st_GasMeasure,
                                            ((GasMeasureState_e)pst_Fram->puc_PayLoad[0]),
                                            f1,f2);
                     res = TRUE;    //应答
                }

                if(pst_Fram->puc_PayLoad[0] == eGasCalibCorrectionGas1 || pst_Fram->puc_PayLoad[0] == eGasCalibCorrectionGas2 ||
                   pst_Fram->puc_PayLoad[0] == eGasCalibCorrectionGasAll)
                {
                     Mod_GasMeasureGotoCalibCorrection(&st_GasMeasure,
                                            ((GasMeasureState_e)pst_Fram->puc_PayLoad[0]),
                                            f1,f2);
                     res = TRUE;    //应答
                }

            }

        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            //读命令是返回是否在调零
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_GasMeasure.e_State;
            res = TRUE;    //应答
        }
        break;  
//==================================================================================
//                                  读取紫外状态
//==================================================================================
    case 0x4b:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 37;
            pst_Fram->puc_PayLoad[0] = st_GasMeasure.e_State;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[1],st_GasMeasure.f_Trans,FALSE);
            if(st_GasMeasure.pst_Gas1 != NULL)
            {
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[5],st_GasMeasure.pst_Gas1->lf_PeakHight,FALSE);
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[13],st_GasMeasure.pst_Gas1->lf_Concentration,FALSE);
            }
            else
            {
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[5],0.0,FALSE);
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[13],0.0,FALSE);
            }
            if(st_GasMeasure.pst_Gas2 != NULL)
            {
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[21],st_GasMeasure.pst_Gas2->lf_PeakHight,FALSE);
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[29],st_GasMeasure.pst_Gas2->lf_Concentration,FALSE);
            }
            else
            {
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[21],0.0,FALSE);
                Bsp_CnvFP64ToArr(&pst_Fram->puc_PayLoad[29],0.0,FALSE);
            }
            res = TRUE;
        }
        break;      
//==================================================================================
//                                   继电器IO控制
//==================================================================================
    case 0x80:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 2)
            {
                BOOL b_State = (pst_Fram->puc_PayLoad[1] == FALSE) ? FALSE : TRUE ;
                Bsp_GpioWirte((GpioId_e)(e_IO_Relay0 + pst_Fram->puc_PayLoad[0]),b_State);
                res = TRUE;     //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                pst_Fram->puc_PayLoad[1] = Bsp_GpioReadOut((GpioId_e)(e_IO_Relay0 + pst_Fram->puc_PayLoad[0]));
                pst_Fram->uin_PayLoadLenth = 2;
                res = TRUE;     //应答
            }
        }
        break;
//==================================================================================
//                                   设置死区时间
//==================================================================================
    case 0xA0:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                st_Measure.ul_DeadTime = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[0],FALSE);
                SaveToEeprom((INT32U)(&st_Measure.ul_DeadTime));
                res = TRUE;     //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.ul_DeadTime,FALSE);
                pst_Fram->uin_PayLoadLenth = 4;
                res = TRUE;     //应答
            }
        }
        break;  
//==================================================================================
//                                   设置测量时间
//==================================================================================  
    case 0xA1:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                st_Measure.ul_MesureTime = Bsp_CnvArrToINT32U(&pst_Fram->puc_PayLoad[0],FALSE);
                SaveToEeprom((INT32U)(&st_Measure.ul_MesureTime));
                res = TRUE;     //应答
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.ul_MesureTime,FALSE);
                pst_Fram->uin_PayLoadLenth = 4;
                res = TRUE;     //应答
            }
        }
        break;  
//==================================================================================
//                                  读取NO采样点
//==================================================================================
	case 0xA2:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 0)
			{
				//读取第一页返回数组长度
				int i = 0;

				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.st_SampleNO.ul_Len,FALSE);

				for(i = 0; i < st_Measure.st_SampleNO.ul_Len; i++)
				{
					Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[2+i*4],st_Measure.st_SampleNO.af_Buff[i],FALSE);
				}

				pst_Fram->uin_PayLoadLenth = 2 + i * 4;

				res = TRUE;    //应答
			}
		}
		break;
//==================================================================================
//                                  读取HC采样点
//==================================================================================
	case 0xA3:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 0)
			{
				//读取第一页返回数组长度
				int i = 0;

				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.st_SampleHC.ul_Len,FALSE);

				for(i = 0; i < st_Measure.st_SampleHC.ul_Len; i++)
				{
					Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[2+i*4],st_Measure.st_SampleHC.af_Buff[i],FALSE);
				}

				pst_Fram->uin_PayLoadLenth = 2 + i * 4;

				res = TRUE;    //应答
			}
		}
		break;

//==================================================================================
//                                   设置无效点数N1
//==================================================================================
	case 0xA4:
		if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 2)
			{
				st_Measure.uin_InvalidDots = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0],FALSE);
                SaveToEeprom((INT32U)(&st_Measure.uin_InvalidDots));
				res = TRUE;     //应答
			}
		}
		else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 0)
			{
				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.uin_InvalidDots,FALSE);
				pst_Fram->uin_PayLoadLenth = 2;
				res = TRUE;     //应答
			}
		}
		break;
//==================================================================================
//                                   设置有效点数N2
//==================================================================================
	case 0xA5:
		if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 2)
			{
				st_Measure.uin_ActiveDots = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0],FALSE);
                SaveToEeprom((INT32U)(&st_Measure.uin_ActiveDots));
				res = TRUE;     //应答
			}
		}
		else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 0)
			{
				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.uin_ActiveDots,FALSE);
				pst_Fram->uin_PayLoadLenth = 2;
				res = TRUE;     //应答
			}
		}
		break;
//==================================================================================
//                                  读取平均的浓度
//==================================================================================
	case 0xA6:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth ==0)
			{
				pst_Fram->uin_PayLoadLenth = 8;
				Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],(FP32)st_Measure.lf_NO,FALSE);
				Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[4],(FP32)st_Measure.lf_HC,FALSE);
				res = TRUE;    //应答
			}
		}
		break;
//==================================================================================
//                                  读取烟度采样点
//==================================================================================
	case 0xAA:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 1)
			{
				//读取第一页返回数组长度
				int i = 0;
                int index = pst_Fram->puc_PayLoad[0];
				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.st_SampleGrey[index].ul_Len,FALSE);

				for(i = 0; i < st_Measure.st_SampleGrey[index].ul_Len; i++)
				{
					Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[2+i*4],st_Measure.st_SampleGrey[index].af_Buff[i],FALSE);
				}

				pst_Fram->uin_PayLoadLenth = 2 + i * 4;

				res = TRUE;    //应答
			}
		}
		break;     
        
    default:
        break;

    }
    
#endif
	return res;
}
