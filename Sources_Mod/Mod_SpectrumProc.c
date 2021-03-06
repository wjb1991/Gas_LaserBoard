#include "App_Include.h"

#define     DEF_SPE_DBG_EN           FALSE

#if (DEF_SPE_DBG_EN == TRUE)
    #define SPE_DBG(...)            do {                            \
                                            Bsp_Printf(__VA_ARGS__);    \
                                        }while(0)
#else
    #define SPE_DBG(...)
#endif

#ifdef __cplusplus
#pragma DATA_SECTION("PublicRam")
#else
//#pragma DATA_SECTION(aui_SpeBuff,"PublicRam");
#endif
static INT16U aui_SpeBuff[DEF_SAMPLEDOT_MAX] = {0};
static INT16U uin_SpeLen = 0;

IrSpectrum_t st_IrSpectrum = {
    (INT16U*)aui_SpeBuff,
    DEF_SAMPLEDOT_MAX,
    5,          //uch_ScanAvg
    0,          //uch_ScanCnt
    200,        //uin_SpectrumLen

    {0},        //af_RawSpectrum
    {0},        //af_SumSpectrum
    {0},        //af_ProceSpectrum
    {0},        //af_OriginalSpectrum
    NULL,       //cb_SpectrumReady
};

BOOL Mod_SpectrumPost(INT16U* pui_Spectrum, INT16U uin_Len)
{
    INT16U  i;
    if(pui_Spectrum == NULL || uin_Len == 0)
        return FALSE;

    for(i = 0; i < uin_Len ; i++)
    {
        aui_SpeBuff[i] = pui_Spectrum[i];
    }
    uin_SpeLen = uin_Len;
    SPE_DBG("uin_SpeLen = %d,uin_Len = %d\r\n",uin_SpeLen,uin_Len);
    return TRUE;
}

BOOL Mod_SpectrumPend(INT16U** ppui_Spectrum, INT16U* pui_Len)
{
    if(ppui_Spectrum == NULL || pui_Len == NULL || uin_SpeLen == 0)
        return FALSE;
    SPE_DBG("uin_SpeLen = %d\r\n",uin_SpeLen);
    *ppui_Spectrum = (INT16U*)aui_SpeBuff;
    *pui_Len = uin_SpeLen;
    uin_SpeLen = 0;
    return TRUE;
}


BOOL Mod_SpectrumProc(IrSpectrum_t* pst_Spe,INT16U uin_SpeLimitCenter,INT16U uin_Gas1Center,INT16U uin_Gas2Center)
{
    INT16U  i;
    INT16U DumpPoint = 10000 % (INT16U)((st_ModWave.f_SampleFreq/st_ModWave.f_SinFreq));
    if(Mod_SpectrumPend(&pst_Spe->pui_RawData,&pst_Spe->uin_RawDataLen) == FALSE)
        return FALSE;

    SPE_DBG("Mod_SpectrumProc\r\n");


    //Bsp_RunLed(eLedOn);

    /* 调用锁相放大器 计算出吸收峰 */
    Mod_DLiaCal(&st_DLia,
                 pst_Spe->pui_RawData,
                 pst_Spe->uin_RawDataLen - DumpPoint,                   //去掉最后几个点
                 pst_Spe->af_RawSpectrum,
                 &pst_Spe->uin_SpectrumLen);

    //Bsp_RunLed(eLedOff);

    for(i = 0; i < pst_Spe->uin_SpectrumLen; i++)
        pst_Spe->af_SumSpectrum[i] += pst_Spe->af_RawSpectrum[i];        //累计求和光谱

    if(++pst_Spe->uch_ScanCnt >= pst_Spe->uch_ScanAvg)
    {
        FP32 f1 = 0;
        FP32 f2 = 0;                                //过高通 //..f2 = str_Trans.f_DcCancelV;
        FP32 f_K = st_Trans.f_LightTrans;           //采样电压的差值 / 原始电压的差值，这是反应透过率大小的原始参数
        FP32 f_AverDc = 0;

        pst_Spe->uch_ScanCnt = 0;

        Mod_TransmissionPoll();                                                                         //计算透过率

        for(i = 0; i < pst_Spe->uin_SpectrumLen; i++) 
        {
            pst_Spe->af_ProceSpectrum[i] = pst_Spe->af_SumSpectrum[i]/pst_Spe->uch_ScanAvg;             //计算平均光谱
            /* 还需要转换成电压单位 并除去 交流的放大倍数 才是实际的吸收峰 */
            
            pst_Spe->af_SumSpectrum[i] = 0;                                                             //清零求和光谱
        }

        /* ======================气体1 AC/DC ======================*/
        f1 = (st_Trans.f_VoltMax - st_Trans.f_VoltMin) * uin_Gas1Center / 250 + st_Trans.f_VoltMin;  //计算吸收峰中心点的三角板电压  250是10000个点缩放10次再缩放5次

        if(st_ModWave.f_TrgVpp >0 )                                                                 //三角波幅值大于0
        {
            //最高点直流电压(采样电压) + 采样电压和原始电压的差值 * 透过率偏差值
            f_AverDc = f1 + f2 + f_K * st_Trans.f_OffestCompare;                                    //透过率修正偏差值  假如实际接收到0.3v三角波 发射是0.9v三角波
        }
        else
        {
            f_AverDc = f2 +  f_K * st_Trans.f_OffestCompare;                                        //透过率修正偏差值
        }

        for(i = 0; i < uin_SpeLimitCenter; i++)
        {
            pst_Spe->af_OriginalSpectrum[i] = pst_Spe->af_ProceSpectrum[i] / f_AverDc / st_Gain.in_AcGain; //ac/dc 修正光谱透过率
        }

        /* ======================气体2 AC/DC ======================*/
        f1 = (st_Trans.f_VoltMax - st_Trans.f_VoltMin) * uin_Gas2Center / 250 + st_Trans.f_VoltMin;  //计算吸收峰中心点的三角板电压

        if(st_ModWave.f_TrgVpp >0 )                                                                 //三角波幅值大于0
        {
            //最高点直流电压(采样电压) + 采样电压和原始电压的差值 * 透过率偏差值
            f_AverDc = f1 + f2 + f_K * st_Trans.f_OffestCompare;                                    //透过率修正偏差值  假如实际接收到0.3v三角波 发射是0.9v三角波
        }
        else
        {
            f_AverDc = f2 +  f_K * st_Trans.f_OffestCompare;                                        //透过率修正偏差值
        }

        for(i = uin_SpeLimitCenter; i < pst_Spe->uin_SpectrumLen; i++)
        {
            pst_Spe->af_OriginalSpectrum[i] = pst_Spe->af_ProceSpectrum[i] / f_AverDc / st_Gain.in_AcGain; //ac/dc 修正光谱透过率
        }


#if 0
        for(i = 0; i < pst_Spe->uin_SpectrumLen; i++) 
        {
            //点斜率式 通过相似三角形算出f1 是三角波的直流分量
            FP32 f1 = (st_Trans.f_VoltMax - st_Trans.f_VoltMin) * i / pst_Spe->uin_SpectrumLen + st_Trans.f_VoltMin;
            FP32 f2 = 0; //过高通 //..f2 = str_Trans.f_DcCancelV;
            FP32 f_K = st_Trans.f_LightTrans;			//采样电压的差值 / 原始电压的差值，这是反应透过率大小的原始参数
            FP32 f_AverDc = 0;
/*
            f_AverDc = st_Trans.f_OffestCompare * f_K;
*/
            if(st_ModWave.f_TrgVpp >0 )													                //三角波幅值大于0
            {
                //最高点直流电压(采样电压) + 采样电压和原始电压的差值 * 透过率偏差值
                f_AverDc = f1 + f2 + f_K * st_Trans.f_OffestCompare;							        //透过率修正偏差值  假如实际接收到0.3v三角波 发射是0.9v三角波
            }
            else
            {
                f_AverDc = f2 +  f_K * st_Trans.f_OffestCompare;								        //透过率修正偏差值
            }

            pst_Spe->af_OriginalSpectrum[i] = pst_Spe->af_ProceSpectrum[i] / f_AverDc / st_Gain.in_AcGain; //ac/dc 修正光谱透过率
        }

        //for(i = 0; i < pst_Spe->uin_SpectrumLen; i++)
        //{
        //    pst_Spe->af_OriginalSpectrum[i] = pst_Spe->af_ProceSpectrum[i] / st_Gain.in_AcGain;         //不修正透过率
        //}
#endif


        if(pst_Spe->cb_SpectrumReady != NULL)
            pst_Spe->cb_SpectrumReady(pst_Spe->af_OriginalSpectrum,pst_Spe->uin_SpectrumLen);       //调用回调函数
    }

    return TRUE;
}


