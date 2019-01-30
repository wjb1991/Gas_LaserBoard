
#ifndef  __APP_TRANSMISSION_H__
#define  __APP_TRANSMISSION_H__  

typedef struct {
    INT16U  uin_SampleCount;
    INT32U  ul_TransSumAD1;
    INT32U  ul_TransSumAD2;

    FP32    f_OffestCompare;  //透过率修正偏差值
    FP32    f_LightTrans;     //通光量         采样电压差值/发送电压差值，这是反应透过率大小的原始参数
    FP32    f_TransK;         //透过率修正系数
    FP32    f_Transmission;   //透过率
    FP32    f_VoltMax;        //实际电压最大值
    FP32    f_VoltMin;        //实际电压最小值
}Transmission_t;

extern Transmission_t st_Trans;

extern void Mod_TransInit(void);
extern void Mod_TransSmapleHigh(void);
extern void Mod_TransSmapleLow(void);
extern FP32 Mod_TransmissionPoll(void);


#endif







