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
}eLaserState;


typedef struct __Laser {
    /* public */
    eLaserState e_State;                    /* 状态 */
    
    Wave_t* pst_Wave;                /* 发送波形结构体 句柄 */

    
    Tec_t * pst_Tec;					    /* TEC句柄 */
}Laser_t;

extern Laser_t    st_Laser;

extern void Mod_LaserEnable(void* pv_Laser);
extern void Mod_LaserDisable(void* pv_Laser);

#endif
