#include "App_Include.h"

#define DEF_USBHOST_DBG_EN           TRUE

#if (DEF_USBHOST_DBG_EN == TRUE)
    #define USBHOST_DBG(...)           do {                            \
                                            Bsp_Printf(__VA_ARGS__);    \
                                        }while(0)
#else
    #define USBHOST_DBG(...)
#endif

void USBHConnectTimeOut(void* timer);

SoftTimer_t st_USBHTimeOut = {
    FALSE,                  //单次模式
    5000,                   //第一次的定时时间
    5000,                   //周期定时时间
    &USBHConnectTimeOut     //回调函数
};
//*****************************************************************************
//
// The size of the host controller's memory pool in bytes.
//
//*****************************************************************************
#define HCD_MEMORY_SIZE         128

//*****************************************************************************
//
// The memory pool to provide to the Host controller driver.
//
//*****************************************************************************
uint8_t g_pHCDPool[HCD_MEMORY_SIZE];

//*****************************************************************************
//
// Declare the USB Events driver interface.
//
//*****************************************************************************
DECLARE_EVENT_DRIVER(g_sUSBEventDriver, 0, 0, USBHCDEvents);

//*****************************************************************************
//
// The global that holds all of the host drivers in use in the application.
// In this case, only the MSC class is loaded.
//
//*****************************************************************************
extern const tUSBHostClassDriver g_sUSBHostSPEClassDriver;
static tUSBHostClassDriver const * const g_ppHostClassDrivers[] =
{
    &g_sUSBHostSPEClassDriver,
    &g_sUSBEventDriver
};

//*****************************************************************************
//
// This global holds the number of class drivers in the g_ppHostClassDrivers
// list.
//
//*****************************************************************************
#define NUM_CLASS_DRIVERS       (sizeof(g_ppHostClassDrivers) /               \
                                 sizeof(g_ppHostClassDrivers[0]))

//*****************************************************************************
//
// Hold the current state for the application.
//
//*****************************************************************************
typedef enum
{
    //
    // No device is present.
    //
    STATE_NO_DEVICE,

    //
    // No device is present.
    //
    STATE_NO_CONNECTED,

    //
    // Mass storage device is being enumerated.
    //
    STATE_DEVICE_ENUM,

    //
    // Mass storage device is ready.
    //
    STATE_DEVICE_READY,

    //
    // An unsupported device has been attached.
    //
    STATE_UNKNOWN_DEVICE,

    //
    // A power fault has occurred.
    //
    STATE_POWER_FAULT
}
tState;
volatile tState g_eState;

//*****************************************************************************
//
// The current USB operating mode - Host, Device or unknown.
//
//*****************************************************************************
tUSBMode g_eCurrentUSBMode;

//*****************************************************************************
//
// USB Mode callback
//
// \param ulIndex is the zero-based index of the USB controller making the
//        callback.
// \param eMode indicates the new operating mode.
//
// This function is called by the USB library whenever an OTG mode change
// occurs and, if a connection has been made, informs us of whether we are to
// operate as a host or device.
//
// \return None.
//
//*****************************************************************************
void
ModeCallback(unsigned long ulIndex, tUSBMode eMode)
{
    g_eCurrentUSBMode = eMode;
}

//*****************************************************************************
//
// This is the generic callback from host stack.
//
// \param pvData is actually a pointer to a tEventInfo structure.
//
// This function will be called to inform the application when a USB event has
// occurred that is outside those related to the mass storage device.  At this
// point this is used to detect unsupported devices being inserted and removed.
// It is also used to inform the application when a power fault has occurred.
// This function is required when the g_USBGenericEventDriver is included in
// the host controller driver array that is passed in to the
// USBHCDRegisterDrivers() function.
//
// \return None.
//
//*****************************************************************************
void
USBHCDEvents(void *pvData)
{
    tEventInfo *pEventInfo;

    //
    // Cast this pointer to its actual type.
    //
    pEventInfo = (tEventInfo *)pvData;

    switch(pEventInfo->ui32Event)
    {
        case USB_EVENT_CONNECTED:
            USBHOST_DBG("USBHOST_DBG:   USB_EVENT_CONNECTED\r\n");
            Bsp_SoftTimerStop(&st_USBHTimeOut);
            break;

        case USB_EVENT_DISCONNECTED:
            USBHOST_DBG("USBHOST_DBG:   USB_EVENT_DISCONNECTED\r\n");
            Bsp_SoftTimerStart(&st_USBHTimeOut);
            break;
        case USB_EVENT_UNKNOWN_CONNECTED:
            USBHOST_DBG("USBHOST_DBG:   USB_EVENT_UNKNOWN_CONNECTED\r\n");
            break;

        case USB_EVENT_POWER_FAULT:
            USBHOST_DBG("USBHOST_DBG:   USB_EVENT_POWER_FAULT\r\n");
            break;

        default:

            break;
    }
}

BOOL Mod_UsbHostInit(void)
{
    Bsp_UsbPow(eUsbPowOn);

    Bsp_DelayMs(3000);

    //
    // Initialize USB GPIO
    //
    USBGPIOEnable();
    USBIntRegister(USB0_BASE, f28x_USB0HostIntHandler);

    //
    // Enable Interrupts
    //
    IntMasterEnable();

    //
    // Initialize the USB stack mode and pass in a mode callback.
    //
    USBStackModeSet(0, eUSBModeForceHost, ModeCallback);

    //
    // Register the host class drivers.
    //
    USBHCDRegisterDrivers(0, g_ppHostClassDrivers, NUM_CLASS_DRIVERS);


    //
    // Initialize the power configuration. This sets the power enable signal
    // to be active high and does not enable the power fault.
    //
    USBHCDPowerConfigInit(0, USBHCD_VBUS_AUTO_HIGH | USBHCD_VBUS_FILTER);

    //
    // Initialize the USB controller for OTG operation with a 2ms polling
    // rate.
    //
    USBHCDInit(0,g_pHCDPool, HCD_MEMORY_SIZE);


    Bsp_SoftTimerReg(&st_USBHTimeOut);

    Bsp_SoftTimerStart(&st_USBHTimeOut);

    return TRUE;
}

BOOL Mod_UsbHostPoll(void)
{
    USBHCDMain();
    return TRUE;
}

void USBHConnectTimeOut(void* timer)
{
/**/
    USBHOST_DBG("USBHOST_DBG:   没有检测到USB设备 重启USB\r\n");

    Bsp_UsbPow(eUsbPowOff);

    Bsp_DelayMs(200);

    //USBHCDReset(0);
    Bsp_UsbPow(eUsbPowOn);
    Bsp_DelayMs(2000);
    USBHOST_DBG("USBHOST_DBG:   重启USB完成\r\n");
}

