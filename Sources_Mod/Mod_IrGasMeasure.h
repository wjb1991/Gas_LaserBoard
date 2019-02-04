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
    INT16U  uin
    INT16U  uin_UseLeft;
    INT16U  uin_UseRigth;
    INT16U  uin_Max;
    INT16U  uin_LeftMin;
    INT16U  uin_RightMin;

}GasInfoForIr;

typedef struct {
    INT8U*      puch_Name;                      /* 气体名字 */
    GasType_e   e_GasType;                      /* 气体编号 */

    INT16U      uin_SpectrumRangeLeft;          /* 光谱范围 */
    INT16U      uin_SpectrumRangeRigth;         /* 光谱范围 */

    FP32        f_A;                            /* 系数A */
    FP32        f_B;                            /* 系数B */
    FP32        f_R;                            /* 系数R */
    
    FP32        f_PeakHight;                    /* 吸收峰高度 */
    FP32        f_Concentration;                /* 浓度 */
}GasInfo_t;



typedef struct {
    FP32    af_ZeroSpectrum[200];
    FP32    af_BkgSpectrum[200];
    FP32    af_DiffSpectrum[200];
    FP32    af_CalibSpectrum[200];
    
    GasInfoForIr* pst_Gas1;
    GasInfoForIr* pst_Gas2;
}GasMeasForIr_t;

extern GasMeasForIr_t st_GasMeasForIr;

BOOL Mod_GasMeasForIr(GasMeasForIr_t* pst_Spe);

#endif
