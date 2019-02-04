#ifndef     __MOD_IRSPECTRUM_H__
#define     __MOD_IRSPECTRUM_H__

typedef struct {
    INT16U  uch_ScanAvg;
    INT16U  uch_ScanCnt;
    INT16U  uin_SpectrumLen;
    FP32    af_RawSpectrum[200];
    FP32    af_SumSpectrum[200];
    FP32    af_ProceSpectrum[200];
    FP32    af_OriginalSpectrum[200];
    void    (*cb_SpectrumReady)(FP32* pf_Spectrum,INT16U uin_SpectrumLen);
}IrSpectrum_t;

extern IrSpectrum_t st_IrSpectrum;

BOOL Mod_SpectrumProcForIr(IrSpectrum_t* pst_Spe);

#endif
