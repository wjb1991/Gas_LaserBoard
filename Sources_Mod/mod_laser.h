//==================================================================================
//| 文件名称 | 激光发射接收处理
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#ifndef __MOD_LASER_H__
#define __MOD_LASER_H__


typedef enum {
    eLaserOff,
    eLaserRise,
    eLaserHigh,
    eLaserFall,
    eLaserLow,
    eLaserStop,//停止状态   同步信号接受到来车信号后 调制波下降到0的过程
    eLaserIdle,//空闲状态   调制波下降到0后 等待 接受到车辆离去信号后 开始测量
}eLaserState;


typedef struct __Laser {
    /* public */
    volatile eLaserState e_State;                    /* 状态 */
    
    Wave_t* pst_Wave;                       /* 发送波形结构体 句柄 */

    Tec_t * pst_Tec;					    /* TEC句柄 */

    FP32    f_Curr;                         /* 激光器电流 */

}Laser_t;

extern Laser_t    st_Laser;

extern void Mod_LaserEnable(void* pv_Laser);
extern void Mod_LaserDisable(void* pv_Laser);
extern void Mod_LaserPoll(void* pv_Laser);
extern void Mod_LaserDoStop(void* pv_Laser);
extern void Mod_LaserExitIdle(void* pv_Laser);
#endif
