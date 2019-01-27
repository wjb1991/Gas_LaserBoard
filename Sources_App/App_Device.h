#ifndef __APP_DEVICE_H__
#define __APP_DEVICE_H__

typedef enum {
    e_DevInit = 0xAA,
    e_DevSelfTest,
    e_DevStart,
    e_DevRun,
    e_DevError,
}DeviceState_t;

typedef enum {
    e_NoAlarm = 0x00,

    e_WarningFlag = 0x20,


    e_FaultFlag = 0x80,

    e_Fault_3_3V,
    e_Fault_1_2V,
    e_Fault_REF2_5V,
    e_Fault_ExramRam,


}DevAlarmCode_t;

typedef struct {
    DeviceState_t e_State;      //×´Ì¬
    DevAlarmCode_t ae_Alarm[5]; //±¨¾¯´úÂë
}Device_t;

extern Device_t st_Device;

BOOL App_DeviceInit(void);

BOOL App_DevicePoll(void);

BOOL App_DeviceSelfTest(void);

void App_AddOneAlarm(DevAlarmCode_t uch_AlarmCode);

void App_ClrOneAlarm(DevAlarmCode_t uch_AlarmCode);

#endif
