#include "App_Include.h"

#define DEF_MASTERDEAL_DBG_EN           FALSE

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


    CMD_RW_IR_SET_MODE = 0x10,                  //设置红外工作模式
    CMD_RW_IR_SET_MEASURE_MODE = 0x12,          //设置红外测量模式

    //红外相关命令
    CMD_RW_IR_SINVPP = 0x20,
    CMD_RW_IR_SINFREQ,
    CMD_RW_IR_TRGVPP,
    CMD_RW_IR_SAMPLEFREQ,
    CMD_RW_IR_DCVOLT,
    CMD_RW_IR_SINPHASE,
    CMD_RW_IR_TECVOLAT,
    CMD_RW_IR_SCANAVG,
    CMD_RW_IR_TRANSCOEFF,

    CMD_RW_SYS_LASER_TEMP = 0x2a,
    CMD_RW_SYS_PCB_TEMP = 0x2c,
    CMD_RW_IR_ACGAIN,
    CMD_RW_IR_DCGAIN,



    //读取光谱相关
    CMD_R_IR_PROC_SPECTRUM = 0x3B,      //AC/DC吸收峰
    CMD_R_IR_ZERO_SPECTRUM,             //调零 吸收峰
    CMD_R_IR_GALIBGAS_SPECTRUM,         //标定气体吸收峰
    CMD_R_IR_DCWAVE,
    CMD_R_IR_ACWAVE = 0x40,
    CMD_R_IR_RAW_SPECTRUM,              //原始吸收峰
    CMD_R_IR_BKG_SPECTRUM,              //背景 吸收峰
    CMD_R_IR_DIFF_SPECTRUM,             //差分 吸收峰

    CMD_R_IR_STATE = 0x80,              //读取红外状态

    //综合测试相关
    CMD_RW_MEAS_DEAD_TIME   = 0xA0,     //设置死区时间
    CMD_RW_MEAS_MEAS_TIME,              //设置测量时间
    CMD_RW_MEAS_INVALID_DOTS,           //设置无效点数
    CMD_RW_MEAS_ACTIVE_DOTS,            //设置有效点数
    CMD_R_MEAS_SAMPLEDOT_CO2,           //读取CO2采样点
    CMD_R_MEAS_SAMPLEDOT_CO,            //读取CO采样点

    //紫外相关命令
    CMD_R_SPE_STATE = 0xD0,
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

    //新添加的命令
    CMD_RW_IR_RISEDOTS = 0xF0,
    CMD_RW_IR_HIGHDOTS,
    CMD_RW_IR_FALLDOTS,
    CMD_RW_IR_LOWDOTS,


}eLasterBoardCmd;


typedef union {

    INT16U auin_Buff[10000];
    FP32   af_Buff[5000];
}ComTemp_t;


#ifdef __cplusplus
#pragma DATA_SECTION("Exsram")
#else
#pragma DATA_SECTION(un_Temp,"Exsram");
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
            if(pst_Fram->uin_PayLoadLenth ==0)
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

#if FALSE //紫外相关
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
//                                  读取当前状态
//==================================================================================
    case CMD_R_IR_STATE:
        //读取第一页返回数组长度
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->puc_PayLoad[0] = st_GasMeasForIr.e_State;
            Bsp_CnvINT32UToArr(&pst_Fram->puc_PayLoad[1],rand(),FALSE);                                //测试序号 暂时用一个随机数
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[5],st_Trans.f_Transmission,FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[9],st_GasMeasForIr.pst_Gas1->f_R,FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[13],st_GasMeasForIr.pst_Gas1->f_Con,FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[17],st_GasMeasForIr.pst_Gas2->f_R,FALSE);
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[21],st_GasMeasForIr.pst_Gas2->f_Con,FALSE);

            pst_Fram->uin_PayLoadLenth = 25;
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                  设置工作模式
//==================================================================================
    case CMD_RW_IR_SET_MODE:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            switch(pst_Fram->puc_PayLoad[0])
            {
            case eGasAdjZero:
                if(pst_Fram->uin_PayLoadLenth == 3)
                {
                    Mod_GasMeasDoAdjZero(&st_GasMeasForIr,Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[1],FALSE));
                    res = TRUE;    //应答
                }
                break;
            case eGasCalibGas1:
            case eGasCalibGas2:
            case eGasCalibGasAll:
                if(pst_Fram->uin_PayLoadLenth == 11)
                {
                    Mod_GasMeasDoCalib(&st_GasMeasForIr,
                                       pst_Fram->puc_PayLoad[0],
                                       Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[1],FALSE),
                                       Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[3],FALSE),
                                       Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[7],FALSE));
                    res = TRUE;    //应答
                }
                break;
            case eGasAbsMeasure:
                if(pst_Fram->uin_PayLoadLenth == 1)
                {
                    if(st_GasMeasForIr.b_DiffMeasrue != FALSE)
                    {
                        st_GasMeasForIr.b_DiffMeasrue = FALSE;
                        res = (BOOL)SaveToEeprom((INT32U)&st_GasMeasForIr.b_DiffMeasrue);
                    }
                    else
                        res = TRUE;

                    Mod_GasMeasDoAbsMeasure(&st_GasMeasForIr);
                }
                break;
            case eGasDiffBackground:
                if(pst_Fram->uin_PayLoadLenth == 1)
                {
                    if(st_GasMeasForIr.b_DiffMeasrue != TRUE)
                    {
                        st_GasMeasForIr.b_DiffMeasrue = TRUE;
                        res = (BOOL)SaveToEeprom((INT32U)&st_GasMeasForIr.b_DiffMeasrue);
                    }
                    else
                        res = TRUE;

                    Mod_GasMeasDoDiffBackground(&st_GasMeasForIr);
                }
                break;
            case eGasDiffMeasure:
                if(pst_Fram->uin_PayLoadLenth == 1)
                {
                    if(st_GasMeasForIr.b_DiffMeasrue != TRUE)
                    {
                        st_GasMeasForIr.b_DiffMeasrue = TRUE;
                        res = (BOOL)SaveToEeprom((INT32U)&st_GasMeasForIr.b_DiffMeasrue);
                    }
                    else
                        res = TRUE;

                    Mod_GasMeasDoDiffMeasure(&st_GasMeasForIr);
                }
                break;
            default:
                break;

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
//                                  设置测量工作模式
//==================================================================================
    case CMD_RW_IR_SET_MEASURE_MODE:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                if(pst_Fram->puc_PayLoad[0] != FALSE)
                    Mod_MeasureDoStaticMeasure(&st_Measure);
                else
                    Mod_MeasureDoDynamicMeasure(&st_Measure);
                res = TRUE;
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_Measure.b_IsStaticMeasure;
            res = TRUE;    //应答
        }
        break;
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

                if (Mod_DLiaSetPhase(&st_DLia,f_Temp,TRUE) == TRUE)
                {
                    res = TRUE;    //应答
                }
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],st_DLia.f_PsdPhase,FALSE);
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
                if(Mod_TecSetVolt(&st_LaserTEC, f_Temp,TRUE) == TRUE)
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

//==================================================================================
//                                  设置扫描平均次数
//==================================================================================
    case CMD_RW_IR_SCANAVG:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 2)
            {
                st_IrSpectrum.uch_ScanAvg = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0],FALSE);

                res = SaveToEeprom((INT32U)&st_IrSpectrum.uch_ScanAvg);
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 2;
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_IrSpectrum.uch_ScanAvg,FALSE);
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                              设置透过率计算系数
//==================================================================================
    case CMD_RW_IR_TRANSCOEFF:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 4)
            {
                st_Trans.f_TransK = Bsp_CnvArrToFP32(&pst_Fram->puc_PayLoad[0],FALSE);

                res = SaveToEeprom((INT32U)&st_Trans.f_TransK);
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 4;
            Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[0],st_Trans.f_TransK ,FALSE);
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                              设置调制波上升沿点数
//==================================================================================
    case CMD_RW_IR_RISEDOTS:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 2)
            {
                INT16U uin_Temp = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0],FALSE);
                if(Mod_SetRiseDots(&st_ModWave, uin_Temp,TRUE) == TRUE)
                {
                    res = TRUE;    //应答
                }
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 2;
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_ModWave.uin_RiseDot,FALSE);
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                              设置调制波高电平点数
//==================================================================================
    case CMD_RW_IR_HIGHDOTS:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 2)
            {
                INT16U uin_Temp = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0],FALSE);
                if(Mod_SetHighDots(&st_ModWave, uin_Temp,TRUE) == TRUE)
                {
                    res = TRUE;    //应答
                }
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 2;
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_ModWave.uin_HigtDot,FALSE);
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                              设置调制波下降沿点数
//==================================================================================
    case CMD_RW_IR_FALLDOTS:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 2)
            {
                INT16U uin_Temp = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0],FALSE);
                if(Mod_SetFallDots(&st_ModWave, uin_Temp,TRUE) == TRUE)
                {
                    res = TRUE;    //应答
                }
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 2;
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_ModWave.uin_FallDot,FALSE);
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                              设置调制波下降沿点数
//==================================================================================
    case CMD_RW_IR_LOWDOTS:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 2)
            {
                INT16U uin_Temp = Bsp_CnvArrToINT16U(&pst_Fram->puc_PayLoad[0],FALSE);
                if(Mod_SetLowDots(&st_ModWave, uin_Temp,TRUE) == TRUE)
                {
                    res = TRUE;    //应答
                }
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 2;
            Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_ModWave.uin_LowDot,FALSE);
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                              设置交流放大倍数
//==================================================================================
    case CMD_RW_IR_ACGAIN:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                res = Mod_GainSetAcGain(&st_Gain,pst_Fram->puc_PayLoad[0],TRUE);
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_Gain.in_AcGain & 0x00ff;
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                              设置直流放大倍数
//==================================================================================
    case CMD_RW_IR_DCGAIN:
        if(pst_Fram->uch_SubCmd == e_StdbusWriteCmd)
        {
            if(pst_Fram->uin_PayLoadLenth == 1)
            {
                res = Mod_GainSetDcGain(&st_Gain,pst_Fram->puc_PayLoad[0],TRUE);
            }
        }
        else if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
            pst_Fram->uin_PayLoadLenth = 1;
            pst_Fram->puc_PayLoad[0] = st_Gain.in_DcGain & 0x00ff;
            res = TRUE;    //应答
        }
        break;
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
                    un_Temp.auin_Buff[i] = st_ModWave.puin_RecvBuff[i];//aui_TestSenseRecvBuff[i];//
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
//==================================================================================
//                                   获取原始吸收峰波形
//==================================================================================
    case CMD_R_IR_RAW_SPECTRUM:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
             //第一个字节是PageIndex  第二三个字节是ReadAddress 第四五个字节是ReadLenth
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                //读取第一页返回数组长度
                pst_Fram->uin_PayLoadLenth = 2;
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_IrSpectrum.uin_SpectrumLen,FALSE);
                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱*/
                Bsp_IntDis();
                for(i = 0; i < st_IrSpectrum.uin_SpectrumLen; i++)
                {
                    un_Temp.af_Buff[i] = st_IrSpectrum.af_ProceSpectrum[i];//st_ModWave.puin_RecvBuff[i];
                }
                Bsp_IntEn();

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 加载原始吸收峰波形到缓冲区 %d个点\r\n",i);
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                uint16_t i = 0;
                uint16_t uin_Offset = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad,FALSE);
                uint16_t uin_Lenth = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad + 2,FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i * 4 + 4],un_Temp.af_Buff[uin_Offset + i],FALSE);
                }

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 发送缓冲区数据 Offset = %d 长度 = %d \r\n",uin_Offset,i);
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   获取处理后吸收峰波形
//==================================================================================
    case CMD_R_IR_PROC_SPECTRUM:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
             //第一个字节是PageIndex  第二三个字节是ReadAddress 第四五个字节是ReadLenth
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                //读取第一页返回数组长度
                pst_Fram->uin_PayLoadLenth = 2;
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_IrSpectrum.uin_SpectrumLen,FALSE);
                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱*/
                Bsp_IntDis();
                for(i = 0; i < st_IrSpectrum.uin_SpectrumLen; i++)
                {
                    un_Temp.af_Buff[i] = st_IrSpectrum.af_OriginalSpectrum[i];//st_ModWave.puin_RecvBuff[i];
                }
                Bsp_IntEn();

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 加载处理后吸收峰波形到缓冲区 %d个点\r\n",i);
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                uint16_t i = 0;
                uint16_t uin_Offset = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad,FALSE);
                uint16_t uin_Lenth = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad + 2,FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i * 4 + 4],un_Temp.af_Buff[uin_Offset + i],FALSE);
                }

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 发送缓冲区数据 Offset = %d 长度 = %d \r\n",uin_Offset,i);
            }
            res = TRUE;    //应答
        }
        break;

//==================================================================================
//                                   获取调零吸收峰波形
//==================================================================================
    case CMD_R_IR_ZERO_SPECTRUM:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
             //第一个字节是PageIndex  第二三个字节是ReadAddress 第四五个字节是ReadLenth
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                //读取第一页返回数组长度
                pst_Fram->uin_PayLoadLenth = 2;
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_GasMeasForIr.uin_SpectrumLen,FALSE);
                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱*/
                Bsp_IntDis();
                for(i = 0; i < st_GasMeasForIr.uin_SpectrumLen; i++)
                {
                    un_Temp.af_Buff[i] = st_GasMeasForIr.af_ZeroSpectrum[i];//st_ModWave.puin_RecvBuff[i];
                }
                Bsp_IntEn();

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 加载调零吸收峰波形到缓冲区 %d个点\r\n",i);
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                uint16_t i = 0;
                uint16_t uin_Offset = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad,FALSE);
                uint16_t uin_Lenth = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad + 2,FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i * 4 + 4],un_Temp.af_Buff[uin_Offset + i],FALSE);
                }

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 发送缓冲区数据 Offset = %d 长度 = %d \r\n",uin_Offset,i);
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   获取标定吸收峰波形
//==================================================================================
    case CMD_R_IR_GALIBGAS_SPECTRUM:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
             //第一个字节是PageIndex  第二三个字节是ReadAddress 第四五个字节是ReadLenth
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                //读取第一页返回数组长度
                pst_Fram->uin_PayLoadLenth = 2;
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_GasMeasForIr.uin_SpectrumLen,FALSE);
                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱*/
                Bsp_IntDis();
                for(i = 0; i < st_GasMeasForIr.uin_SpectrumLen; i++)
                {
                    un_Temp.af_Buff[i] = st_GasMeasForIr.af_CalibSpectrum[i];//st_ModWave.puin_RecvBuff[i];
                }
                Bsp_IntEn();

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 加载标定吸收峰波形到缓冲区 %d个点\r\n",i);
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                uint16_t i = 0;
                uint16_t uin_Offset = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad,FALSE);
                uint16_t uin_Lenth = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad + 2,FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i * 4 + 4],un_Temp.af_Buff[uin_Offset + i],FALSE);
                }

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 发送缓冲区数据 Offset = %d 长度 = %d \r\n",uin_Offset,i);
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   获取背景吸收峰波形
//==================================================================================
    case CMD_R_IR_BKG_SPECTRUM:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
             //第一个字节是PageIndex  第二三个字节是ReadAddress 第四五个字节是ReadLenth
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                //读取第一页返回数组长度
                pst_Fram->uin_PayLoadLenth = 2;
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_GasMeasForIr.uin_SpectrumLen,FALSE);
                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱*/
                Bsp_IntDis();
                for(i = 0; i < st_GasMeasForIr.uin_SpectrumLen; i++)
                {
                    un_Temp.af_Buff[i] = st_GasMeasForIr.af_BkgSpectrum[i];//st_ModWave.puin_RecvBuff[i];
                }
                Bsp_IntEn();

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 加载背景吸收峰波形到缓冲区 %d个点\r\n",i);
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                uint16_t i = 0;
                uint16_t uin_Offset = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad,FALSE);
                uint16_t uin_Lenth = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad + 2,FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i * 4 + 4],un_Temp.af_Buff[uin_Offset + i],FALSE);
                }

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 发送缓冲区数据 Offset = %d 长度 = %d \r\n",uin_Offset,i);
            }
            res = TRUE;    //应答
        }
        break;
//==================================================================================
//                                   获取差分吸收峰波形
//==================================================================================
    case CMD_R_IR_DIFF_SPECTRUM:
        if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
        {
             //第一个字节是PageIndex  第二三个字节是ReadAddress 第四五个字节是ReadLenth
            if(pst_Fram->uin_PayLoadLenth == 0)
            {
                //读取第一页返回数组长度
                pst_Fram->uin_PayLoadLenth = 2;
                Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_GasMeasForIr.uin_SpectrumLen,FALSE);
                /* 加载光谱到 缓冲区 确保不会再传输一半中 更新光谱*/
                Bsp_IntDis();
                for(i = 0; i < st_GasMeasForIr.uin_SpectrumLen; i++)
                {
                    un_Temp.af_Buff[i] = st_GasMeasForIr.af_DiffSpectrum[i];//st_ModWave.puin_RecvBuff[i];
                }
                Bsp_IntEn();

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 加载差分吸收峰波形到缓冲区 %d个点\r\n",i);
            }
            else if(pst_Fram->uin_PayLoadLenth == 4)
            {
                uint16_t i = 0;
                uint16_t uin_Offset = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad,FALSE);
                uint16_t uin_Lenth = Bsp_CnvArrToINT16U(pst_Fram->puc_PayLoad + 2,FALSE);

                pst_Fram->uin_PayLoadLenth = 4 + uin_Lenth * 4;
                for(i = 0; i<uin_Lenth;i++)
                {
                    Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[i * 4 + 4],un_Temp.af_Buff[uin_Offset + i],FALSE);
                }

                MASTERDEAL_DBG(">>MASTERDEAL_DBG: 发送缓冲区数据 Offset = %d 长度 = %d \r\n",uin_Offset,i);
            }
            res = TRUE;    //应答
        }
        break;
#endif
//==================================================================================
//                                   设置死区时间
//==================================================================================
    case CMD_RW_MEAS_DEAD_TIME:
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
    case CMD_RW_MEAS_MEAS_TIME:
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
//                                   设置无效点数N1
//==================================================================================
	case CMD_RW_MEAS_INVALID_DOTS:
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
	case CMD_RW_MEAS_ACTIVE_DOTS:
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
//                                  读取CO2采样点
//==================================================================================
	case CMD_R_MEAS_SAMPLEDOT_CO2:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 0)
			{
				//读取第一页返回数组长度
				int i = 0;

				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.st_SampleCO2.ul_Len,FALSE);

				for(i = 0; i < st_Measure.st_SampleCO2.ul_Len; i++)
				{
					Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[2+i*4],st_Measure.st_SampleCO2.af_Buff[i],FALSE);
				}

				pst_Fram->uin_PayLoadLenth = 2 + i * 4;

				res = 1;    //应答
			}
		}
		break;
//==================================================================================
//                                  读取CO采样点
//==================================================================================
	case CMD_R_MEAS_SAMPLEDOT_CO:
		if(pst_Fram->uch_SubCmd == e_StdbusReadCmd)
		{
			if(pst_Fram->uin_PayLoadLenth == 0)
			{
				//读取第一页返回数组长度
				int i = 0;

				Bsp_CnvINT16UToArr(&pst_Fram->puc_PayLoad[0],st_Measure.st_SampleCO.ul_Len,FALSE);

				for(i = 0; i < st_Measure.st_SampleCO.ul_Len; i++)
				{
					Bsp_CnvFP32ToArr(&pst_Fram->puc_PayLoad[2+i*4],st_Measure.st_SampleCO.af_Buff[i],FALSE);
				}

				pst_Fram->uin_PayLoadLenth = 2 + i * 4;

				res = 1;    //应答
			}
		}
		break;


    default:
        break;
    }
	return res;
}
