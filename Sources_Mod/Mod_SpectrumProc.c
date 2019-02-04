#include "App_Include.h"

IrSpectrum_t st_IrSpectrum = {
    5,          //uch_ScanAvg
    0,          //uch_ScanCnt
    150,        //uin_SpectrumLen
    {0},        //af_RawSpectrum
    {0},        //af_SumSpectrum
    {0},        //af_ProceSpectrum
    {0},        //af_OriginalSpectrum
    NULL,       //cb_SpectrumReady
};


BOOL Mod_SpectrumProcForIr(IrSpectrum_t* pst_Spe)
{
    INT16U  i;

    for(i = 0; i < st_Laser.pst_Wave->uin_SampleDot;i++)
    {
        st_Laser.pst_Wave->puin_RecvBuff[i] -= 32768UL;
        //st_Laser.pst_Wave->puin_RecvBuff[i] = aui_TestSenseRecvBuff[i] - 32768UL;       //使用调试数组计算
    }

    /* 调用锁相放大器 计算出吸收峰 吸收峰计算完成后 接受缓冲区就已经被释放了 */
    Mod_DLiaCal(&st_DLia,
                 (INT16S*)st_Laser.pst_Wave->puin_RecvBuff,
                 st_Laser.pst_Wave->uin_SampleDot,
                 pst_Spe->af_RawSpectrum,
                 &pst_Spe->uin_SpectrumLen);


    for(i = 0; i < pst_Spe->uin_SpectrumLen; i++)
        pst_Spe->af_SumSpectrum[i] += pst_Spe->af_RawSpectrum[i];        //累计求和光谱

    if(++pst_Spe->uch_ScanCnt >= pst_Spe->uch_ScanAvg)
    {
        pst_Spe->uch_ScanCnt = 0;

        Mod_TransmissionPoll();                                                                         //计算透过率

        for(i = 0; i < pst_Spe->uin_SpectrumLen; i++) 
        {
            pst_Spe->af_ProceSpectrum[i] = pst_Spe->af_SumSpectrum[i]/pst_Spe->uch_ScanAvg;             //计算平均光谱
            /*还需要转换成电压单位 并除去 交流的放大倍数 才是实际的吸收峰 */
            
            pst_Spe->af_SumSpectrum[i] = 0;                                                             //清零求和光谱
        }

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

            pst_Spe->af_OriginalSpectrum[i] = pst_Spe->af_ProceSpectrum[i] / f_AverDc;                  //ac/dc 修正光谱透过率
        }
    }

    return TRUE;
}
