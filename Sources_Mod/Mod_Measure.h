
#ifndef __MOD_MEASURE_H__
#define __MOD_MEASURE_H__

#define DEF_SAMPLE_DOT_MAX     200

typedef struct {
    FP32    af_Buff[DEF_SAMPLE_DOT_MAX];
    INT32U  ul_Len;
    INT32U  ul_Size;
}SampleDots_t;

typedef enum {
    e_MeasureIdle = 0x10,
    e_MeasureWait,
    e_MeasureDead,
    e_MeasureSample,
    e_MeasureCal,
    e_MeasureTimeOut,
}MeasureState_e;

typedef struct {
    MeasureState_e  e_State;        /* 测试状态 */
    BOOL	b_IsStaticMeasure;		/* 是否静态测量 */
    INT32U  ul_DeadTime;            /* 死区时间 */
    INT32U  ul_MesureTime;          /* 测试时间 */
    INT16U	uin_InvalidDots;		/* 无效点 */
    INT16U	uin_ActiveDots;			/* 有效点 */
    SampleDots_t st_SampleCO;       /* CO采样点 */
    SampleDots_t st_SampleCO2;      /* CO2采样点 */
    FP32	f_CO;					/* 测试时间窗口内的平均浓度 */
    FP32	f_CO2;					/* 测试时间窗口内的平均浓度 */
}Measure_t ;

extern Measure_t st_Measure;

void Mod_MeasureInit(Measure_t* pst_Meas);
void Mod_MeasurePoll(Measure_t* pst_Meas);
void Mod_MeasureGasCO2Notification(FP32 f_Concentration);
void Mod_MeasureGasCONotification(FP32 f_Concentration);

void Mod_MeasureDoStaticMeasure(Measure_t* pst_Meas);
void Mod_MeasureDoDynamicMeasure(Measure_t* pst_Meas);


#endif /* __APP_MEASURE_H__ */
