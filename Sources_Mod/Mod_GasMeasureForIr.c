#include "App_Include.h"


#define DEF_GASMEASIR_DBG_EN           FALSE

#if (DEF_GASMEASIR_DBG_EN == TRUE)
    #define GASMEASIR_DBG(...)             do {                            \
                                            Bsp_Printf(__VA_ARGS__);    \
                                        }while(0)
#else
    #define GASMEASIR_DBG(...)
#endif

GasInfoForIr st_GasCO2 = {
    (INT8U*)"CO2",                          /* 气体名字 */
    eGasCO2,                        /* 气体编号 */
    0,                              /* 光谱范围 */
    77,                             /* 光谱范围 */

    0,                              /* 标定浓度 */

    0,                              /* 系数A */
    0,                              /* 系数B */
    0,                              /* 系数R */
    
    0,                              /* 浓度 */
    NULL,                           /* 浓度通知回调 */  
};

GasInfoForIr st_GasCO = {
    (INT8U*)"CO",                           /* 气体名字 */
    eGasCO,                         /* 气体编号 */
    77,                             /* 光谱范围 */
    154,                            /* 光谱范围 */

    0,                              /* 标定浓度 */

    0,                              /* 系数A */
    0,                              /* 系数B */
    0,                              /* 系数R */
    
    0,                              /* 浓度 */
    NULL,                           /* 浓度通知回调 */  
};

GasMeasForIr_t st_GasMeasForIr = {
    eGasDiffBackground,             /* 测量状态 */

    85,                             /* 两个气体的分界线*/
    0,
    NULL,                           /* 处理后光谱 */                         
    {0},                            /* 调零光谱 */
    {0},                            /* 背景光谱 */
    {0},                            /* 差分光谱 */
    {0},                            /* 校准光谱 */
    
    0,
    0,
    0.5,                            /* 背景光谱一阶滤波系数 */
    TRUE,                           /* 是否差分测量 */

    &st_GasCO2,                     /* 气体1 */
    &st_GasCO,                      /* 气体2 */
    
};

void SpectrumReady(FP32* pf_Spectrum,INT16U uin_SpectrumLen)
{
    Mod_GasMeas(&st_GasMeasForIr, pf_Spectrum, uin_SpectrumLen);
}

BOOL Mod_GasMeasInit(GasMeasForIr_t* pst_Meas)
{
    INT16U i = 0;
    if(pst_Meas->b_DiffMeasrue == TRUE)
        pst_Meas->e_State = eGasDiffBackground;
    else
        pst_Meas->e_State = eGasAbsMeasure;

    for(i = 0; i < pst_Meas->uin_SpectrumLen;i++)
    {
        pst_Meas->af_BkgSpectrum[i] = 0;
        pst_Meas->af_DiffSpectrum[i] = 0;
    }

    st_IrSpectrum.cb_SpectrumReady = SpectrumReady;

    return TRUE;
}

BOOL Mod_GasMeasDoIdle(GasMeasForIr_t* pst_Meas)
{
    if(pst_Meas == NULL || pst_Meas->uin_ScanAvg != 0)
        return FALSE;
    pst_Meas->e_State = eGasIdle;
    return TRUE;
}

BOOL Mod_GasMeasDoAdjZero(GasMeasForIr_t* pst_Meas,INT16U uin_Count)
{
    if(pst_Meas == NULL || pst_Meas->uin_ScanAvg != 0)
        return FALSE;
    pst_Meas->e_State = eGasAdjZero;
    pst_Meas->uin_ScanAvg = uin_Count;
    pst_Meas->uin_Cnt = 0;

    GASMEASIR_DBG(">>GASMEASIR_DB: 进入调零模式 %d次平均\r\n",pst_Meas->uin_ScanAvg);
    return TRUE;
}

BOOL Mod_GasMeasDoCalib(GasMeasForIr_t* pst_Meas,INT16U uin_Kind,INT16U uin_Count,FP32 f_CalibCon1,FP32 f_CalibCon2)
{
    if(pst_Meas == NULL || pst_Meas->uin_ScanAvg != 0)
        return FALSE;
    if(uin_Kind == eGasCalibGas1)
    {
        pst_Meas->e_State = eGasCalibGas1;
        pst_Meas->uin_ScanAvg = uin_Count;
        pst_Meas->uin_Cnt = 0;
        pst_Meas->pst_Gas1->f_CalibCon = f_CalibCon1;
        SaveToEeprom((INT32U)(&st_GasCO2.f_CalibCon));
        GASMEASIR_DBG(">>GASMEASIR_DB: 进入标定模式 标定气体1 %d次平均\r\n",pst_Meas->uin_ScanAvg);
        return TRUE;
    }
    else if(uin_Kind == eGasCalibGas2)
    {
        pst_Meas->e_State = eGasCalibGas2;
        pst_Meas->uin_ScanAvg = uin_Count;
        pst_Meas->uin_Cnt = 0;
        pst_Meas->pst_Gas2->f_CalibCon = f_CalibCon2;
        SaveToEeprom((INT32U)(&st_GasCO.f_CalibCon));
        GASMEASIR_DBG(">>GASMEASIR_DB: 进入标定模式 标定气体2 %d次平均\r\n",pst_Meas->uin_ScanAvg);
        return TRUE;
    }
    else if(uin_Kind == eGasCalibGasAll)
    {
        pst_Meas->e_State = eGasCalibGasAll;
        pst_Meas->uin_ScanAvg = uin_Count;
        pst_Meas->uin_Cnt = 0;
        pst_Meas->pst_Gas1->f_CalibCon = f_CalibCon1;  
        pst_Meas->pst_Gas2->f_CalibCon = f_CalibCon2;  
        SaveToEeprom((INT32U)(&st_GasCO2.f_CalibCon));
        SaveToEeprom((INT32U)(&st_GasCO.f_CalibCon));
        GASMEASIR_DBG(">>GASMEASIR_DB: 进入标定模式 标定所有气体 %d次平均\r\n",pst_Meas->uin_ScanAvg);
        return TRUE;
    }
    return FALSE;
}

BOOL Mod_GasMeasDoAbsMeasure(GasMeasForIr_t* pst_Meas)
{
    if(pst_Meas == NULL || pst_Meas->uin_ScanAvg != 0)
        return FALSE;
    if(pst_Meas->b_DiffMeasrue == FALSE)
    {
        pst_Meas->e_State = eGasAbsMeasure;
        GASMEASIR_DBG(">>GASMEASIR_DB: 进入绝对测量模式\r\n");
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL Mod_GasMeasDoDiffMeasure(GasMeasForIr_t* pst_Meas)
{
    if(pst_Meas == NULL || pst_Meas->uin_ScanAvg != 0)
        return FALSE;
    if(pst_Meas->b_DiffMeasrue == TRUE)
    {
        pst_Meas->e_State = eGasDiffMeasure;  
        GASMEASIR_DBG(">>GASMEASIR_DB: 进入差分测量模式\r\n");
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL Mod_GasMeasDoDiffBackground(GasMeasForIr_t* pst_Meas)
{
    if(pst_Meas == NULL || pst_Meas->uin_ScanAvg != 0)
        return FALSE;
    if(pst_Meas->b_DiffMeasrue == TRUE)
    {
        pst_Meas->e_State = eGasDiffBackground;  

        GASMEASIR_DBG(">>GASMEASIR_DB: 进入差分背景模式\r\n");
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

///////////////////////////////////////////////////////////////////
//                          仪表调零
///////////////////////////////////////////////////////////////////
static void Mod_GasMeasAdjZero(GasMeasForIr_t* pst_Meas)
{
    INT16U  i;
    if(pst_Meas->uin_Cnt == 0 )
    {
        for( i = 0; i < pst_Meas->uin_SpectrumLen; i++)
        {
            pst_Meas->af_BkgSpectrum[i] = 0;
        }
    }

    for( i = 0; i < pst_Meas->uin_SpectrumLen; i++)
    {
        pst_Meas->af_BkgSpectrum[i] += pst_Meas->pf_ProcSpectrum[i];
    }
    
    if(++pst_Meas->uin_Cnt >= pst_Meas->uin_ScanAvg)
    {
        for( i = 0; i < pst_Meas->uin_SpectrumLen; i++)
        {
            pst_Meas->af_BkgSpectrum[i] /= pst_Meas->uin_ScanAvg;
            pst_Meas->af_ZeroSpectrum[i] = pst_Meas->af_BkgSpectrum[i];
        }
        pst_Meas->uin_ScanAvg = 0;
        
        SaveToEepromExt((INT32U)pst_Meas->af_ZeroSpectrum, 200);

        if(pst_Meas->b_DiffMeasrue == TRUE)
            pst_Meas->e_State = eGasDiffBackground;
        else
            pst_Meas->e_State = eGasAbsMeasure;


        GASMEASIR_DBG(">>GASMEASIR_DB: 调零完成\r\n");
    }

}

///////////////////////////////////////////////////////////////////
//                          仪表标定
///////////////////////////////////////////////////////////////////

static void Mod_GasMeasCalib(GasMeasForIr_t* pst_Meas, GasInfoForIr* pst_Gas)
{
    INT16U  i;

    for( i = 0; i < pst_Meas->uin_SpectrumLen; i++)
    {
        pst_Meas->af_DiffSpectrum[i] = pst_Meas->pf_ProcSpectrum[i] - pst_Meas->af_ZeroSpectrum[i]; /* 跟新差分光谱 */
    }

    if(pst_Meas->pst_Gas1 == pst_Gas)
    {
        if(pst_Meas->uin_Cnt == 0 )
        {
            for( i = 0; i < pst_Meas->uin_SpeLimitCenter; i++)  /*清空对应区段的背景光谱*/
                pst_Meas->af_BkgSpectrum[i] = 0;
        }

        for( i = 0; i < pst_Meas->uin_SpeLimitCenter; i++)  /*累计对应区段的背景光谱*/
        {
            pst_Meas->af_BkgSpectrum[i] += pst_Meas->af_DiffSpectrum[i];
        }

        if(++pst_Meas->uin_Cnt >= pst_Meas->uin_ScanAvg )
        {
            for( i = 0; i < pst_Meas->uin_SpeLimitCenter; i++)   /*平均对应区段的背景光谱*/
            {
                pst_Meas->af_BkgSpectrum[i] /= pst_Meas->uin_ScanAvg;
                pst_Meas->af_CalibSpectrum[i] = pst_Meas->af_BkgSpectrum[i];
            }

            SaveToEepromExt((INT32U)pst_Meas->af_CalibSpectrum, 200);

            pst_Meas->uin_ScanAvg = 0;
            if(pst_Meas->b_DiffMeasrue == TRUE)
                pst_Meas->e_State = eGasDiffBackground;
            else
                pst_Meas->e_State = eGasAbsMeasure;

            GASMEASIR_DBG(">>GASMEASIR_DB: 标定气体1完成\r\n");
        }
    }
    else
    {
        if(pst_Meas->uin_Cnt == 0 )
        {
            for( i = pst_Meas->uin_SpeLimitCenter; i < pst_Meas->uin_SpectrumLen; i++)  /*清空对应区段的背景光谱*/
                pst_Meas->af_BkgSpectrum[i] = 0;
        }


        for( i = pst_Meas->uin_SpeLimitCenter; i < pst_Meas->uin_SpectrumLen; i++)   /*累计对应区段的背景光谱*/
        {
            pst_Meas->af_BkgSpectrum[i] += pst_Meas->af_DiffSpectrum[i];
        }

        if(++pst_Meas->uin_Cnt >= pst_Meas->uin_ScanAvg )
        {
            for( i = pst_Meas->uin_SpeLimitCenter; i < pst_Meas->uin_SpectrumLen; i++)  /*平均对应区段的背景光谱*/
            {
                pst_Meas->af_BkgSpectrum[i] /= pst_Meas->uin_ScanAvg;
                pst_Meas->af_CalibSpectrum[i] = pst_Meas->af_BkgSpectrum[i];
            }

            SaveToEepromExt((INT32U)pst_Meas->af_CalibSpectrum, 200);

            pst_Meas->uin_ScanAvg = 0;
            if(pst_Meas->b_DiffMeasrue == TRUE)
                pst_Meas->e_State = eGasDiffBackground;
            else
                pst_Meas->e_State = eGasAbsMeasure;

            GASMEASIR_DBG(">>GASMEASIR_DB: 标定气体2完成\r\n");
        }
    }
}

static void Mod_GasMeasCalibAll(GasMeasForIr_t* pst_Meas, GasInfoForIr* pst_Gas1,GasInfoForIr* pst_Gas2)
{
    INT16U  i;
    
    for( i = 0; i < pst_Meas->uin_SpectrumLen; i++)
    {
        pst_Meas->af_DiffSpectrum[i] = pst_Meas->pf_ProcSpectrum[i] - pst_Meas->af_ZeroSpectrum[i]; /* 跟新差分光谱 */
    }
    
    if(pst_Meas->uin_Cnt == 0 )
    {
        for( i = 0; i < pst_Meas->uin_SpectrumLen; i++)     /*清空对应区段的背景光谱*/
        {
            pst_Meas->af_BkgSpectrum[i] = 0;
        } 
    }

    for( i = 0; i < pst_Meas->uin_SpectrumLen; i++)         /*累计对应区段的背景光谱*/
    {
        pst_Meas->af_BkgSpectrum[i] += pst_Meas->af_DiffSpectrum[i];
    }
    
    if(++pst_Meas->uin_Cnt >= pst_Meas->uin_ScanAvg )
    {
        for( i = 0; i < pst_Meas->uin_SpectrumLen; i++)     /*平均对应区段的背景光谱*/
        {
            pst_Meas->af_BkgSpectrum[i] /= pst_Meas->uin_ScanAvg;
            pst_Meas->af_CalibSpectrum[i] = pst_Meas->af_BkgSpectrum[i];
        }
        
        SaveToEepromExt((INT32U)pst_Meas->af_CalibSpectrum, 200);

        pst_Meas->uin_ScanAvg = 0;
        if(pst_Meas->b_DiffMeasrue == TRUE)
            pst_Meas->e_State = eGasDiffBackground;
        else
            pst_Meas->e_State = eGasAbsMeasure;

        GASMEASIR_DBG(">>GASMEASIR_DB: 标定所有气体完成\r\n");
    }
}


void LinearRegression(FP32* f_X, FP32* f_Y, INT16U uin_SampleNum,
                      FP32* f_K, FP32* f_B, FP32* f_R)
{
    FP32 f_Xavg, f_Yavg, f_Lxx, f_Lyy, f_Lxy;
    INT16U i;
    
///////////////////////////////////////////////////////////////////
//                          计算X和Y的平均值
///////////////////////////////////////////////////////////////////
    f_Xavg = 0;
    for (i = 0; i < uin_SampleNum; i++ )
    {
        f_Xavg += f_X[i];
    }
    f_Xavg /= uin_SampleNum;
    
    f_Yavg = 0;
    for (i = 0; i < uin_SampleNum; i++ )
    {
        f_Yavg += f_Y[i];
    }
    f_Yavg /= uin_SampleNum;
///////////////////////////////////////////////////////////////////
//                          计算Lxx Lyy Lxy
///////////////////////////////////////////////////////////////////
    f_Lxx = 0;
    for (i = 0; i < uin_SampleNum; i++ )
    {
        f_Lxx += f_X[i]*f_X[i];
    }
    f_Lxx -= uin_SampleNum*f_Xavg*f_Xavg;
    
    f_Lyy = 0;
    for (i = 0; i < uin_SampleNum; i++ )
    {
        f_Lyy += f_Y[i]*f_Y[i];
    }
    f_Lyy -= uin_SampleNum*f_Yavg*f_Yavg;
      
    f_Lxy = 0;
    for (i = 0; i < uin_SampleNum; i++ )
    {
        f_Lxy += f_X[i]*f_Y[i];
    }
    f_Lxy -= uin_SampleNum*f_Xavg*f_Yavg;

///////////////////////////////////////////////////////////////////
//                          计算K B R
///////////////////////////////////////////////////////////////////
    if(f_K != NULL)
        *f_K = f_Lxy/f_Lxx;

    if(f_K != NULL && f_B != NULL)
        *f_B = f_Yavg - *f_K*f_Xavg;

    if(f_R != NULL)
        *f_R = f_Lxy/sqrt(f_Lxx*f_Lyy);
}

void Mod_GasMeasMeasure(GasMeasForIr_t* pst_Meas,GasInfoForIr* pst_Gas)
{
    //使用整个光谱区域计算
    LinearRegression(&pst_Meas->af_CalibSpectrum[pst_Gas->uin_GasLeft],
                     &pst_Meas->af_DiffSpectrum[pst_Gas->uin_GasLeft],
                     pst_Gas->uin_GasRight - pst_Gas->uin_GasLeft + 1,
                     &pst_Gas->f_K,&pst_Gas->f_B,&pst_Gas->f_R);


    pst_Gas->f_Con = pst_Gas->f_CalibCon * pst_Gas->f_K;
    
    /* 浓度限制 */
    if(pst_Gas->f_Con > 100)
        pst_Gas->f_Con = 100;
    if(pst_Gas->f_Con < 0)
        pst_Gas->f_Con = 0;

    if(pst_Gas->cb_Notification != NULL)
        pst_Gas->cb_Notification(pst_Gas->f_Con);

#if 0
    GASMEASIR_DBG(">>GASMEASIR_DBG: %s浓度 = %f, K = %f, B = %f, R = %f\r\n",
                  pst_Gas->puch_Name,
                  pst_Gas->f_Con,
                  pst_Gas->f_K,
                  pst_Gas->f_B,
                  pst_Gas->f_R);
#endif

}

///////////////////////////////////////////////////////////////////
//                        仪表绝对测量
///////////////////////////////////////////////////////////////////
static void Mod_GasMeasAbsMeasure(GasMeasForIr_t* pst_Meas)
{
    INT16U i;
    for( i = 0; i < pst_Meas->uin_SpectrumLen; i++)
    {
        pst_Meas->af_DiffSpectrum[i] = pst_Meas->pf_ProcSpectrum[i] - pst_Meas->af_ZeroSpectrum[i] ;
    }
    Mod_GasMeasMeasure(pst_Meas,pst_Meas->pst_Gas1);
    Mod_GasMeasMeasure(pst_Meas,pst_Meas->pst_Gas2);
}

///////////////////////////////////////////////////////////////////
//                        仪表差分测量
///////////////////////////////////////////////////////////////////
static void Mod_GasMeasDiffMeasure(GasMeasForIr_t* pst_Meas)
{
    INT16U i;
    for( i = 0; i < pst_Meas->uin_SpectrumLen; i++)
    {
        pst_Meas->af_DiffSpectrum[i] = pst_Meas->pf_ProcSpectrum[i] - pst_Meas->af_BkgSpectrum[i] ;
    }
    Mod_GasMeasMeasure(pst_Meas,pst_Meas->pst_Gas1);
    Mod_GasMeasMeasure(pst_Meas,pst_Meas->pst_Gas2);
}

static void Mod_GasMeasBackground(GasMeasForIr_t* pst_Meas)
{
    INT16U i;
    for( i = 0; i < pst_Meas->uin_SpectrumLen; i++)
    {
        pst_Meas->af_BkgSpectrum[i] = pst_Meas->pf_ProcSpectrum[i] * pst_Meas->f_FilterCoeff + 
                                      pst_Meas->af_BkgSpectrum[i] *(1-pst_Meas->f_FilterCoeff);
                                      
        pst_Meas->af_DiffSpectrum[i] = pst_Meas->pf_ProcSpectrum[i] - pst_Meas->af_BkgSpectrum[i] ;
    }
}
       
BOOL Mod_GasMeasPoll(GasMeasForIr_t* pst_Meas)
{
    Mod_SpectrumProc(&st_IrSpectrum,
                       pst_Meas->uin_SpeLimitCenter,
                       pst_Meas->pst_Gas1->uin_GasCenter,
                       pst_Meas->pst_Gas2->uin_GasCenter);
    return TRUE;
}

BOOL Mod_GasMeas(GasMeasForIr_t* pst_Meas,FP32* pf_Spectrum, INT16U uin_SpectrumLen)
{
    pst_Meas->pf_ProcSpectrum = pf_Spectrum;
    pst_Meas->uin_SpectrumLen = uin_SpectrumLen;
    GASMEASIR_DBG(">>GASMEASIR_DB: 红外气体浓度分析\r\n");
    switch(pst_Meas->e_State)
    {
    case eGasIdle:
    
        break;
    case eGasAdjZero:
        Mod_GasMeasAdjZero(pst_Meas);
        break;
    case eGasCalibGas1:
        Mod_GasMeasCalib(pst_Meas,pst_Meas->pst_Gas1);
        break;  
    case eGasCalibGas2:
        Mod_GasMeasCalib(pst_Meas,pst_Meas->pst_Gas2);
        break; 
    case eGasCalibGasAll:
        Mod_GasMeasCalibAll(pst_Meas,pst_Meas->pst_Gas1,pst_Meas->pst_Gas2);
        break;
    case eGasAbsMeasure:
        Mod_GasMeasAbsMeasure(pst_Meas);
        break;  
    case eGasDiffBackground:
        Mod_GasMeasBackground(pst_Meas);
        break;
    case eGasDiffMeasure:
        Mod_GasMeasDiffMeasure(pst_Meas);
        break;
    default:
        break;
    }

    return TRUE;
}


BOOL Mod_GasSetLeft(GasInfoForIr* pst_Gas,INT16U uin_Index,BOOL b_WriteEPROM)
{
    if( pst_Gas != NULL)
    {
        if(uin_Index <= 200)
        {
            pst_Gas->uin_GasLeft = uin_Index;
            if(b_WriteEPROM == TRUE)
            {
                if(SaveToEeprom((INT32U)&pst_Gas->uin_GasLeft) != TRUE)
                    return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOL Mod_GasSetCenter(GasInfoForIr* pst_Gas,INT16U uin_Index,BOOL b_WriteEPROM)
{
    if( pst_Gas != NULL)
    {
        if(uin_Index <= 200)
        {
            pst_Gas->uin_GasCenter = uin_Index;
            if(b_WriteEPROM == TRUE)
            {
                if(SaveToEeprom((INT32U)&pst_Gas->uin_GasCenter) != TRUE)
                    return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOL Mod_GasSetRight(GasInfoForIr* pst_Gas,INT16U uin_Index,BOOL b_WriteEPROM)
{
    if( pst_Gas != NULL)
    {
        if(uin_Index <= 200)
        {
            pst_Gas->uin_GasRight = uin_Index;
            if(b_WriteEPROM == TRUE)
            {
                if(SaveToEeprom((INT32U)&pst_Gas->uin_GasRight) != TRUE)
                    return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOL Mod_GasSetLimitCenter(GasMeasForIr_t* pst_Meas,INT16U uin_Index,BOOL b_WriteEPROM)
{
    if( pst_Meas != NULL)
    {
        if(uin_Index <= 200)
        {
            pst_Meas->uin_SpeLimitCenter = uin_Index;
            if(b_WriteEPROM == TRUE)
            {
                if(SaveToEeprom((INT32U)&pst_Meas->uin_SpeLimitCenter) != TRUE)
                    return FALSE;
            }
            return TRUE;
        }
    }
    return FALSE;
}
