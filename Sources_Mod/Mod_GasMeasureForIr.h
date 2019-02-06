#ifndef     __MOD_GASMEASFORIR_H__
#define     __MOD_GASMEASFORIR_H__


typedef enum {
    eGasN2 = 0,
    eGasCO2,
    eGasCO,
    eGasNO,
    eGasHC,
}GasType_e;

typedef struct {
    INT8U*      puch_Name;                      /* 气体名字 */
    GasType_e   e_GasType;                      /* 气体编号 */

    INT16U      uin_SpectrumRangeLeft;          /* 光谱范围 */
    INT16U      uin_SpectrumRangeRight;         /* 光谱范围 */

    FP32        f_CalibCon;                     /* 标定浓度 */
    
    FP32        f_K;                            /* 系数K */
    FP32        f_B;                            /* 系数B */
    FP32        f_R;                            /* 系数R */
    
    FP32        f_Con;                          /* 浓度 f_Concentration */
}GasInfoForIr;

typedef enum {
    eGasIdle = 0,                               /* 空闲啥都不干 */
    eGasAdjZero,                                /* 调零 */
    eGasCalibGas1,                              /* 标定 气体1 */
    eGasCalibGas2,                              /* 标定 气体2 */
    eGasCalibGasAll,                            /* 标定 所有气体 */
    eGasAbsMeasure,                             /* 绝对浓度测量 */
    eGasDiffBackground,                         /* 差分测量采集背景 */
    eGasDiffMeasure,                            /* 差分浓度测量 */
}GasMeasureState_e;

typedef struct {
    GasMeasureState_e   e_State;                /* 状态 */
    INT16U  uin_SpectrumLen;
    FP32*   pf_ProcSpectrum;                    /* 处理后光谱 */
    FP32    af_ZeroSpectrum[200];               /* 调零光谱 */
    FP32    af_BkgSpectrum[200];                /* 背景光谱 */
    FP32    af_DiffSpectrum[200];               /* 差分光谱 */
    FP32    af_CalibSpectrum[200];              /* 校准光谱 */
    
    INT16U  uin_Cnt;
    INT16U  uin_ScanAvg; 
    FP32    f_FilterCoeff;                      /* 背景光谱一阶滤波系数 */
    BOOL    b_DiffMeasrue;                      /* 是否差分测量 */
    
    GasInfoForIr* pst_Gas1;
    GasInfoForIr* pst_Gas2;
}GasMeasForIr_t;

extern GasInfoForIr st_GasCO2;
extern GasInfoForIr st_GasCO;
extern GasMeasForIr_t st_GasMeasForIr;

BOOL Mod_GasMeasInit(GasMeasForIr_t* pst_Meas);

BOOL Mod_GasMeasForIr(GasMeasForIr_t* pst_Meas,FP32* pf_Spectrum, INT16U uin_SpectrumLen);

BOOL Mod_GasMeasDoAdjZero(GasMeasForIr_t* pst_Meas,INT16U uin_Count);

BOOL Mod_GasMeasDoCalib(GasMeasForIr_t* pst_Meas,INT16U uin_Kind,INT16U uin_Count,FP32 f_CalibCon1,FP32 f_CalibCon2);

BOOL Mod_GasMeasDoAbsMeasure(GasMeasForIr_t* pst_Meas);

BOOL Mod_GasMeasDoDiffMeasure(GasMeasForIr_t* pst_Meas);

BOOL Mod_GasMeasDoDiffBackground(GasMeasForIr_t* pst_Meas);


#endif
