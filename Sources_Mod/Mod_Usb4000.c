#include "App_Include.h"

typedef enum
{
	USB4000_DISCONNECT = 0,
	USB4000_INIT,  
	USB4000_CONFIG,
	USB4000_SAMPLE,
	USB4000_ERROR,
}USB4000_StateTypeDef;

typedef struct _USB4000_Process
{
    uint32_t             rx_count;

    uint8_t              b_SetFlag;
    uint32_t             ul_SetIntegralTime;
    


    int16_t              ain_Spectrum[3840];
    int32_t              al_SumSpectrum[38];
    
    float               af_ProcessSpectrum[38];
    float               af_WaveLenth[38];

    
    uint8_t              uch_ScansToAverage;
    uint8_t              uch_ScansConut;
    uint8_t              uch_Boxcar;
    


    uint8_t              b_EdcEnable;
    uint8_t              b_NlcEnable;
    uint8_t              b_IsConnect;
    uint8_t              b_Open;

    



    uint8_t             auc_SerialNumber[20];          /* 序列号 */
    uint16_t            auin_EdcIndexs[11];        /* 5-15像素 */
    float               af_WlcCoeff[4];            /* 波长拟合系数 */
    float               af_NlcCoeff[8];            /* 非线性校正系数 */
    uint8_t              uch_NlcOrder;             /* 非线性矫正阶数 */

    uint16_t             uin_Pixels;               /* 像素 */
    uint32_t             ul_IntegralTime;          /* 积分时间 */
    uint8_t              uc_PackNum;               /* 光谱数据包总数 */
    uint8_t              b_HighSpeed;              /* 是否是高速USB接口 */

	USB4000_StateTypeDef e_State;
}USB4000_HandleTypeDef;

USB4000_HandleTypeDef	st_Usb4000;

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
#if 1
#include "utils/uartstdio.h"
        UARTprintf("QueryInfo %d = %s\r\n",i,string);
#endif
        }

#if 1
#include "utils/uartstdio.h"
        UARTprintf("USB4000 INIT SUCESS\r\n");
#endif
        st_Usb4000.e_State = USB4000_CONFIG;

	    break;
	}

	case USB4000_CONFIG:
	    res =  USBHSPESetIntegralTime(g_psSPEInstance,200,10000);
        if ( res != TRUE )
            break;

#if 1
#include "utils/uartstdio.h"
        UARTprintf("USB4000_CONFIG SUCESS\r\n");
#endif

        st_Usb4000.e_State = USB4000_SAMPLE;
        break;
	case USB4000_SAMPLE:
        res =  USBHSPEGetSpectrum(g_psSPEInstance,200,st_Usb4000.ain_Spectrum);
        if ( res != TRUE )
        {
            //UARTprintf("USB4000_SAMPLE_FAIL\r\n");
            break;
        }
#if 1
#include "utils/uartstdio.h"
        UARTprintf("USB4000_SAMPLE\r\n");
#endif
	    break;
	case USB4000_DISCONNECT:



	    break;

	default:
	    break;
	}
	
	
	return TRUE;
}





