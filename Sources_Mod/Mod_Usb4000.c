#include "App_Include.h"

#define DEF_USB4000_DBG_EN           FALSE

#if (DEF_USB4000_DBG_EN == TRUE)
    #define USB4000_DBG(...)           do {                            \
                                            Bsp_Printf(__VA_ARGS__);    \
                                        }while(0)
#else
    #define USB4000_DBG(...)
#endif


USB4000_HandleTypeDef	st_Usb4000;

void USBHSPESample(void* timer);

SoftTimer_t st_USBHSampleTimer = {
  FALSE,                  //单次模式
  1000,                   //第一次的定时时间
  0,                      //周期定时时间
  &USBHSPESample          //回调函数
};

static BOOL b_SyncFlag = FALSE;

//*****************************************************************************
//
// The instance data for the MSC driver.
//
//*****************************************************************************
tUSBHSPEInstance *g_psSPEInstance = 0;

//*****************************************************************************
//
// This is the callback from the MSC driver.
//
// \param ulInstance is the driver instance which is needed when communicating
// with the driver.
// \param ulEvent is one of the events defined by the driver.
// \param pvData is a pointer to data passed into the initial call to register
// the callback.
//
// This function handles callback events from the MSC driver.  The only events
// currently handled are the MSC_EVENT_OPEN and MSC_EVENT_CLOSE.  This allows
// the main routine to know when an MSC device has been detected and
// enumerated and when an MSC device has been removed from the system.
//
// \return Returns \e true on success or \e false on failure.
//
//*****************************************************************************
void
SPECallback(tUSBHSPEInstance *psSPEInstance, uint32_t ui32Event, void *pvEventData)
{
    switch(ui32Event)
    {
        case SPE_EVENT_OPEN:
			st_Usb4000.e_State = USB4000_INIT;
            break;
        case SPE_EVENT_CLOSE:
			st_Usb4000.e_State = USB4000_DISCONNECT;
            break;

        default:

            break;
    }
}




BOOL Mod_Usb4000Init(void)
{
    //
    // Open an instance of the mass storage class driver.
    //
    g_psSPEInstance = USBHSPEDriveOpen(0, (tUSBHSPECallback)SPECallback);
    st_Usb4000.e_State = USB4000_DISCONNECT;

    Bsp_SoftTimerReg(&st_USBHSampleTimer);
    return TRUE;
}

BOOL Mod_Usb4000Poll(void)
{
    uint16_t res;
	switch(st_Usb4000.e_State)
	{
	case USB4000_INIT:
	{
	    uint8_t  string[62];
	    uint16_t  strlen;
	    uint8_t  i;
	    res = USBHSPEGetState(g_psSPEInstance,
                                  200,
                                  &st_Usb4000.uin_Pixels,
                                  &st_Usb4000.ul_IntegralTime,
                                  NULL,
                                  &st_Usb4000.uc_PackNum,
                                  &st_Usb4000.b_HighSpeed);

        if ( res != TRUE )
            break;

        for( i = 0 ; i < 16; i++)
        {
            res = USBHSPEQueryInfo(g_psSPEInstance,
                                     200,
                                     i,
                                     string,
                                     &strlen);
            if ( res != TRUE )
                break;

            if ( i == 0 )
                memcpy(st_Usb4000.auc_SerialNumber,string,strlen);
            else if( i < 5 ) //1-4
            {
                st_Usb4000.af_WlcCoeff[i-1] = atof((const char*)string);
            }
            else if( i == 6 )
            {

            }
            else if( i < 15)
            {
                st_Usb4000.af_NlcCoeff[i-7] = atof((const char*)string);
            }
            else if( i == 15 )
            {
                st_Usb4000.uch_NlcOrder = atoi((const char*)string);
            }

            USB4000_DBG("QueryInfo %d = %s\r\n",i,string);
        }

        USB4000_DBG("USB4000 INIT SUCESS\r\n");

        st_Usb4000.e_State = USB4000_CONFIG;

	    break;
	}

	case USB4000_CONFIG:
        res =  USBHSPESetTrigger(g_psSPEInstance,20,0);//Data Value = 0 Normal Mode Data Value = 1 Software Trigger Mode
        if ( res != TRUE )
            break;

	    res =  USBHSPESetIntegralTime(g_psSPEInstance,20,3000);
        if ( res != TRUE )
            break;

        USB4000_DBG("USB4000_CONFIG SUCESS\r\n");

        st_Usb4000.e_State = USB4000_SAMPLE;

        st_USBHSampleTimer.ul_Period = 3;
        Bsp_SoftTimerStart(&st_USBHSampleTimer);
        break;
	case USB4000_SAMPLE:
	    if (b_SyncFlag == TRUE)
	    {
	        b_SyncFlag = FALSE;
	        res =  USBHSPEGetSpectrum(g_psSPEInstance,25,st_Usb4000.ain_Spectrum);
	        if ( res != TRUE )
	        {
	            USB4000_DBG("_F\r\n");
	            break;
	        }

	        USB4000_DBG("_S\r\n");
	    }
	    break;
	case USB4000_DISCONNECT:

        Bsp_SoftTimerStop(&st_USBHSampleTimer);

	    break;

	default:
	    break;
	}
	
	
	return TRUE;
}

void USBHSPESample(void* timer)
{
    b_SyncFlag = TRUE;
}



