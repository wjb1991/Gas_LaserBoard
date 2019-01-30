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
}IrSpectrum_t;

extern IrSpectrum_t st_IrSpectrum;

BOOL Mod_IRSpectrumPoll(IrSpectrum_t* pst_Spe);

#endif
