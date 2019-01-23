#ifndef __APP_DEVICE_H__
#define __APP_DEVICE_H__

typedef enum {
    e_DevInit = 0xAA,
    e_DevSelfTest,
    e_DevStart,
    e_DevWork,
    e_DevError,
}DeviceState_t;

typedef struct {
    DeviceState_t e_State;      //״̬

}Device_t;

extern Device_t st_Device;

BOOL App_DeviceInit(void);

BOOL App_DevicePoll(void);

#endif
