/*
 * usbhusb4000.c
 *
 *  Created on: 2019年1月10日
 *      Author: Wangjiangbo
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "driverlib/usb.h"
#include "usblib/usblib.h"
#include "usblib/usblibpriv.h"
#include "usblib/host/usbhost.h"
#include "usblib/host/usbhostpriv.h"
#include "usbhspe.h"
#include "bsp.h"

//*****************************************************************************
//
// Forward declarations for the driver open and close calls.
//
//*****************************************************************************
static void *USBHSPEOpen(tUSBHostDevice *psDevice);
static void USBHSPEClose(void *pvInstance);

static void USBHSPEPipeCallback(uint32_t ui32Pipe, uint32_t ui32Event);
//*****************************************************************************
//
// This is the structure for an instance of a USB Spectrometer host driver.
//
//*****************************************************************************
struct tUSBHSPEInstance
{
    //
    // Save the device instance.
    //
    tUSBHostDevice *psDevice;

    //
    // Used to save the callback.
    //
    tUSBHSPECallback pfnCallback;


    uint32_t ui32EP1InPipe;

    uint32_t ui32EP1OutPipe;

    uint32_t ui32EP2InPipe;

    uint32_t ui32EP6InPipe;

    uint8_t     bMux;          //互斥锁
    uint8_t     bSem;       //接受信号量
    uint16_t    uiTimeCnt;    //超时计时
};

//*****************************************************************************
//
// The array of USB MSC Spectrometer drivers.
//
//*****************************************************************************
static tUSBHSPEInstance g_sUSBHSPEDevice =
{
    0, 0
};
//*****************************************************************************
//
//! This constant global structure defines the Mass Storage Class Driver that
//! is provided with the USB library.
//
//*****************************************************************************
const tUSBHostClassDriver g_sUSBHostSPEClassDriver =
{
	USB_CLASS_VEND_SPECIFIC,
	USBHSPEOpen,
	USBHSPEClose,
    0
};

//*****************************************************************************
//
//! This function is used to open an instance of the MSC driver.
//!
//! \param psDevice is a pointer to the device information structure.
//!
//! This function will attempt to open an instance of the MSC driver based on
//! the information contained in the \e psDevice structure.  This call can fail
//! if there are not sufficient resources to open the device.  The function
//! returns a value that should be passed back into USBMSCClose() when the
//! driver is no longer needed.
//!
//! \return The function will return a pointer to a MSC driver instance.
//
//*****************************************************************************
static void *
USBHSPEOpen(tUSBHostDevice *psDevice)
{
    int32_t i32Idx;
    tEndpointDescriptor *psEndpointDescriptor;
    tInterfaceDescriptor *psInterface;

    //
    // Don't allow the device to be opened without closing first.
    //
    if(g_sUSBHSPEDevice.psDevice)
    {
        return(0);
    }

    //
    // Save the device pointer.
    //
    g_sUSBHSPEDevice.psDevice = psDevice;

    //
    // Get the interface descriptor.
    //
    psInterface = USBDescGetInterface(psDevice->psConfigDescriptor, 0, 0);

    //
    // Loop through the endpoints of the device.
    //
    for(i32Idx = 0; i32Idx < 5; i32Idx++)
    {
        //
        // Get the first endpoint descriptor.
        //
        psEndpointDescriptor =
            USBDescGetInterfaceEndpoint(psInterface, i32Idx,
                                        psDevice->ui32ConfigDescriptorSize);

        //
        // If no more endpoints then break out.
        //
        if(psEndpointDescriptor == 0)
        {
            break;
        }
#if 1
#include "utils/uartstdio.h"
        UARTprintf("EP Address = 0x%x, EP Size = %d ",psEndpointDescriptor->bEndpointAddress,
        												   readusb16_t(&(psEndpointDescriptor->wMaxPacketSize)));

        if((psEndpointDescriptor->bmAttributes & USB_EP_ATTR_TYPE_M) == USB_EP_ATTR_BULK)
        	UARTprintf("USB_EP_ATTR_BULK\r\n");
        else if((psEndpointDescriptor->bmAttributes & USB_EP_ATTR_TYPE_M) == USB_EP_ATTR_INT)
        	UARTprintf("USB_EP_ATTR_INT\r\n");
        else if((psEndpointDescriptor->bmAttributes & USB_EP_ATTR_TYPE_M) == USB_EP_ATTR_ISOC)
        	UARTprintf("USB_EP_ATTR_ISOC\r\n");
#endif
        if((psEndpointDescriptor->bmAttributes & USB_EP_ATTR_TYPE_M) ==
           USB_EP_ATTR_BULK)
        {
			switch (psEndpointDescriptor->bEndpointAddress)
			{
			case 0x01:
				//
				// Allocate the USB Pipe for this Bulk IN endpoint.
				//
				g_sUSBHSPEDevice.ui32EP1OutPipe =
					USBHCDPipeAllocSize(0, USBHCD_PIPE_BULK_OUT,
										psDevice,
										readusb16_t(&(psEndpointDescriptor->wMaxPacketSize)),
										USBHSPEPipeCallback);
				//
				// Configure the USB pipe as a Bulk IN endpoint.
				//
				USBHCDPipeConfig(g_sUSBHSPEDevice.ui32EP1OutPipe,
								 readusb16_t(&(psEndpointDescriptor->wMaxPacketSize)),
								 0,
								 (psEndpointDescriptor->bEndpointAddress &
								  USB_EP_DESC_NUM_M));
				break;
			case 0x82:
				//
				// Allocate the USB Pipe for this Bulk IN endpoint.
				//
				g_sUSBHSPEDevice.ui32EP2InPipe =
					USBHCDPipeAllocSize(0, USBHCD_PIPE_BULK_IN,
										psDevice,
										readusb16_t(&(psEndpointDescriptor->wMaxPacketSize)),
										USBHSPEPipeCallback);
				//
				// Configure the USB pipe as a Bulk IN endpoint.
				//
				USBHCDPipeConfig(g_sUSBHSPEDevice.ui32EP2InPipe,
								 readusb16_t(&(psEndpointDescriptor->wMaxPacketSize)),
								 0,
								 (psEndpointDescriptor->bEndpointAddress &
								  USB_EP_DESC_NUM_M));
				break;
			case 0x86:
				//
				// Allocate the USB Pipe for this Bulk IN endpoint.
				//
				g_sUSBHSPEDevice.ui32EP6InPipe =
					USBHCDPipeAllocSize(0, USBHCD_PIPE_BULK_IN,
										psDevice,
										readusb16_t(&(psEndpointDescriptor->wMaxPacketSize)),
										USBHSPEPipeCallback);
				//
				// Configure the USB pipe as a Bulk IN endpoint.
				//
				USBHCDPipeConfig(g_sUSBHSPEDevice.ui32EP6InPipe,
								 readusb16_t(&(psEndpointDescriptor->wMaxPacketSize)),
								 0,
								 (psEndpointDescriptor->bEndpointAddress &
								  USB_EP_DESC_NUM_M));
				break;
			case 0x81:
				//
				// Allocate the USB Pipe for this Bulk IN endpoint.
				//
				g_sUSBHSPEDevice.ui32EP1InPipe =
					USBHCDPipeAllocSize(0, USBHCD_PIPE_BULK_IN,
										psDevice,
										readusb16_t(&(psEndpointDescriptor->wMaxPacketSize)),
										USBHSPEPipeCallback);
				//
				// Configure the USB pipe as a Bulk IN endpoint.
				//
				USBHCDPipeConfig(g_sUSBHSPEDevice.ui32EP1InPipe,
								 readusb16_t(&(psEndpointDescriptor->wMaxPacketSize)),
								 0,
								 (psEndpointDescriptor->bEndpointAddress &
								  USB_EP_DESC_NUM_M));

				break;
			default:
				break;
			}
        }
    }

    //
    // If the callback exists, call it with an Open event.
    //
    if(g_sUSBHSPEDevice.pfnCallback != 0)
    {
    	g_sUSBHSPEDevice.pfnCallback(&g_sUSBHSPEDevice, SPE_EVENT_OPEN, 0);
    }

    //
    // Return the only instance of this device.
    //
    return(&g_sUSBHSPEDevice);
}

//*****************************************************************************
//
//! This function is used to release an instance of the MSC driver.
//!
//! \param pvInstance is an instance pointer that needs to be released.
//!
//! This function will free up any resources in use by the MSC driver instance
//! that is passed in.  The \e pvInstance pointer should be a valid value that
//! was returned from a call to USBMSCOpen().
//!
//! \return None.
//
//*****************************************************************************
static void
USBHSPEClose(void *pvInstance)
{
    //
    // Do nothing if there is not a driver open.
    //
    if(g_sUSBHSPEDevice.psDevice == 0)
    {
        return;
    }

    //
    // Reset the device pointer.
    //
    g_sUSBHSPEDevice.psDevice = 0;

    //
    // Free the Bulk IN pipe.
    //
    if(g_sUSBHSPEDevice.ui32EP1InPipe != 0)
    {
        USBHCDPipeFree(g_sUSBHSPEDevice.ui32EP1InPipe);
    }

    if(g_sUSBHSPEDevice.ui32EP2InPipe != 0)
    {
        USBHCDPipeFree(g_sUSBHSPEDevice.ui32EP2InPipe);
    }

    if(g_sUSBHSPEDevice.ui32EP6InPipe != 0)
    {
        USBHCDPipeFree(g_sUSBHSPEDevice.ui32EP6InPipe);
    }
    //
    // Free the Bulk OUT pipe.
    //
    if(g_sUSBHSPEDevice.ui32EP1OutPipe != 0)
    {
        USBHCDPipeFree(g_sUSBHSPEDevice.ui32EP1OutPipe);
    }

    //
    // If the callback exists then call it.
    //
    if(g_sUSBHSPEDevice.pfnCallback != 0)
    {
        g_sUSBHSPEDevice.pfnCallback(&g_sUSBHSPEDevice, SPE_EVENT_CLOSE, 0);
    }

}

//*****************************************************************************
//
//! This function should be called before any devices are present to enable
//! the mass storage device class driver.
//!
//! \param ui32Drive is the drive number to open.
//! \param pfnCallback is the driver callback for any mass storage events.
//!
//! This function is called to open an instance of a mass storage device.  It
//! should be called before any devices are connected to allow for proper
//! notification of drive connection and disconnection.  The \e ui32Drive
//! parameter is a zero based index of the drives present in the system.
//! There are a constant number of drives, and this number should only
//! be greater than 0 if there is a USB hub present in the system.  The
//! application should also provide the \e pfnCallback to be notified of mass
//! storage related events like device enumeration and device removal.
//!
//! \return This function will return the driver instance to use for the other
//! mass storage functions.  If there is no driver available at the time of
//! this call, this function will return zero.
//
//*****************************************************************************
tUSBHSPEInstance *
USBHSPEDriveOpen(uint32_t ui32Drive, tUSBHSPECallback pfnCallback)
{
    //
    // Only the first drive is supported and only one callback is supported.
    //
    if((ui32Drive != 0) || (g_sUSBHSPEDevice.pfnCallback))
    {
        return(0);
    }

    //
    // Save the callback.
    //
    g_sUSBHSPEDevice.pfnCallback = pfnCallback;

    //
    // Return the requested device instance.
    //
    return(&g_sUSBHSPEDevice);
}

static uint16_t SemPend(tUSBHSPEInstance *psSPEInstance,uint16_t uiTimeOut)
{
    uint32_t ulTimeCnt = uiTimeOut * 4000;
    psSPEInstance->bSem = FALSE;

    while( ulTimeCnt-- )
    {
        DELAY_US(0.25);
        if(psSPEInstance->bSem == TRUE)
        {
            psSPEInstance->bSem = FALSE;
            return TRUE;
        }
    }
    return FALSE;

    /*
    psSPEInstance->bSem = FALSE;
    psSPEInstance->uiTimeCnt = 0;
    while(psSPEInstance->uiTimeCnt < uiTimeOut)
    {
        if(psSPEInstance->bSem == TRUE)
        {
            psSPEInstance->bSem = FALSE;
            return TRUE;
        }
    }
    return FALSE;*/
}

static uint16_t SemPost(tUSBHSPEInstance *psSPEInstance)
{
    psSPEInstance->bSem = TRUE;
    return TRUE;
}

void USBHSPETimeInc(tUSBHSPEInstance *psSPEInstance)
{
    psSPEInstance->uiTimeCnt++;
}

//*****************************************************************************
//
//! This function is used to release an instance of the SPE driver.
//!
//! \param pvInstance is an instance pointer that needs to be released.
//!
//! This function will free up any resources in use by the SPE driver instance
//! that is passed in.  The \e pvInstance pointer should be a valid value that
//! was returned from a call to USBSPEOpen().
//!
//! \return None.
//
//*****************************************************************************
uint16_t
USBHSPEGetState(tUSBHSPEInstance *psSPEInstance,
                  uint16_t  ui_TimeOut,
                  uint16_t* puiPixels,
                  uint32_t* pulIntegrationTime,
                  uint16_t* pbLampEnable,
                  uint16_t* puiPacketCount,
                  uint16_t* pbUsbSpeed)
{
	uint8_t  ui8Data[64] = {0};

    //
    // If there is no device present then return an error.
    //
    if(psSPEInstance->psDevice == 0)
    {
        return(FALSE);
    }

    ui8Data[0] = 0xfe;
    USBHCDPipeWrite(psSPEInstance->ui32EP1OutPipe, ui8Data, 1);
    //UARTprintf("Send ui32Size = 0x%X\r\n",ui32Size);

    /* 阻塞读取
    ui32Size = USBHCDPipeRead(psSPEInstance->ui32EP1InPipe, ui8Data, 64);//USBHCDPipeReadNonBlocking 不能用
     */

    /* 引发一次请求 但不阻塞 数据接受再回调函数中 */
    USBHCDPipeSchedule(psSPEInstance->ui32EP1InPipe,ui8Data,64);

    if (SemPend(psSPEInstance,ui_TimeOut) != TRUE)
        return(FALSE);

    USBHCDPipeReadNonBlocking(psSPEInstance->ui32EP1InPipe, ui8Data, 64);

    if(puiPixels != NULL)
    {
        *puiPixels = ( ui8Data[0] & 0x00ff ) | ( ui8Data[1] << 8);
    }


    if(pulIntegrationTime != NULL)
    {
        *pulIntegrationTime = ((uint32_t)ui8Data[2] & 0x00ff ) | ((uint32_t)ui8Data[3] << 8) |
                              ((uint32_t)ui8Data[4] << 16 ) | ((uint32_t)ui8Data[5] << 24);
    }

    if(pbLampEnable != NULL)
        *pbLampEnable = FALSE;

    if(puiPacketCount != NULL)
        *puiPacketCount = ui8Data[9] & 0x00ff ;

    if(pbUsbSpeed != NULL)
        *pbUsbSpeed = (ui8Data[14] != 0) ? TRUE : FALSE;

    //
    // Success.
    //
    return(TRUE);
}

//*****************************************************************************
//
//! This function is used to release an instance of the SPE driver.
//!
//! \param pvInstance is an instance pointer that needs to be released.
//!
//! This function will free up any resources in use by the SPE driver instance
//! that is passed in.  The \e pvInstance pointer should be a valid value that
//! was returned from a call to USBSPEOpen().
//!
//! \return None.
//
//*****************************************************************************
uint16_t
USBHSPEQueryInfo(tUSBHSPEInstance *psSPEInstance,
                  uint16_t  ui_TimeOut,
                  uint16_t  ucIndex,
                  uint16_t* pucString,
                  uint16_t* uiStrLen)
{
    uint8_t  ui8Data[64] = {0};
    int32_t  i32Size;

    //
    // If there is no device present then return an error.
    //
    if(psSPEInstance->psDevice == 0)
    {
        return(FALSE);
    }

    ui8Data[0] = 0x05;
    ui8Data[1] = ucIndex;
    i32Size = USBHCDPipeWrite(psSPEInstance->ui32EP1OutPipe, ui8Data, 2);
    //UARTprintf("Send ui32Size = 0x%X\r\n",ui32Size);

    /* 阻塞读取
    ui32Size = USBHCDPipeRead(psSPEInstance->ui32EP1InPipe, ui8Data, 64);//USBHCDPipeReadNonBlocking 不能用
     */

    /* 引发一次请求 但不阻塞 数据接受再回调函数中 */
    USBHCDPipeSchedule(psSPEInstance->ui32EP1InPipe,ui8Data,64);

    if (SemPend(psSPEInstance,ui_TimeOut) != TRUE)
        return(FALSE);

    i32Size = USBHCDPipeReadNonBlocking(psSPEInstance->ui32EP1InPipe, ui8Data, 64);

    if(ui8Data[0] != 0x05 || ui8Data[1] != ucIndex)
        return(FALSE);

    i32Size = strlen((const char *)&ui8Data[2]) + 1;
    if (i32Size <= 0 || i32Size > 62 )
        return(FALSE);

    if (pucString != NULL)
        memcpy(pucString,&ui8Data[2],i32Size);

    if (uiStrLen != NULL)
        *uiStrLen = i32Size;
    //
    // Success.
    //
    return(TRUE);
}


//*****************************************************************************
//
//! This function is used to release an instance of the SPE driver.
//!
//! \param pvInstance is an instance pointer that needs to be released.
//!
//! This function will free up any resources in use by the SPE driver instance
//! that is passed in.  The \e pvInstance pointer should be a valid value that
//! was returned from a call to USBSPEOpen().
//!
//! \return None.
//
//*****************************************************************************
uint16_t
USBHSPESetIntegralTime(tUSBHSPEInstance *psSPEInstance,
                  uint16_t  ui_TimeOut,
                  uint32_t  ul_IntegralTime)
{
    uint8_t  ui8Data[64] = {0};
    uint16_t res;
    uint32_t ui32Read;

    //
    // If there is no device present then return an error.
    //
    if(psSPEInstance->psDevice == 0)
    {
        return(FALSE);
    }

    ui8Data[0] = 0x02;
    ui8Data[1] = (ul_IntegralTime >> 0) & 0x000000ff;
    ui8Data[2] = (ul_IntegralTime >> 8) & 0x000000ff;
    ui8Data[3] = (ul_IntegralTime >> 16) & 0x000000ff;
    ui8Data[4] = (ul_IntegralTime >> 24) & 0x000000ff;
    USBHCDPipeWrite(psSPEInstance->ui32EP1OutPipe, ui8Data, 5);

    res = USBHSPEGetState(psSPEInstance,
                          ui_TimeOut,
                          NULL,
                          &ui32Read,
                          NULL,
                          NULL,
                          NULL);

    if(res == TRUE && ui32Read == ul_IntegralTime)
        return(TRUE);
    else
        return(FALSE);
}

uint16_t
USBHSPEGetSpectrum(tUSBHSPEInstance *psSPEInstance,
                   uint16_t  ui_TimeOut,
                   int16_t*  pi_Spectrum)
 {
     uint8_t  ui8Data[64] = {0};
     uint16_t i,j;

     //
     // If there is no device present then return an error.
     //
     if(psSPEInstance->psDevice == 0 || pi_Spectrum == NULL)
     {
         return(FALSE);
     }

     ui8Data[0] = 0x09;
     USBHCDPipeWrite(psSPEInstance->ui32EP1OutPipe, ui8Data, 1);

     for(i = 0; i <= 120; i++)
     {
         /* 引发一次请求 但不阻塞 数据接受再回调函数中 */
         USBHCDPipeSchedule(psSPEInstance->ui32EP2InPipe,ui8Data,64);

         if (SemPend(psSPEInstance,ui_TimeOut) != TRUE)
             return(FALSE);

         if( i == 120 && ui8Data[0] == 0x69)
             return(TRUE);

         for(j = 0; j < 32; j++)
             pi_Spectrum[j + i*32] = (((uint16_t)ui8Data[j*2]) & 0xff) + (((uint16_t)ui8Data[j * 2 + 1])<<8);

     }
     return(FALSE);
 }
//*****************************************************************************
//
//! This function is used to release an instance of the SPE driver.
//!
//! \param pvInstance is an instance pointer that needs to be released.
//!
//! This function will free up any resources in use by the SPE driver instance
//! that is passed in.  The \e pvInstance pointer should be a valid value that
//! was returned from a call to USBSPEOpen().
//!
//! \return None.
//
//*****************************************************************************
uint16_t
USBHSPEInit(tUSBHSPEInstance *psSPEInstance,
                  uint16_t  ui_TimeOut)
{
    uint8_t  ui8Data[64] = {0};
    //
    // If there is no device present then return an error.
    //
    if(psSPEInstance->psDevice == 0)
    {
        return(FALSE);
    }

    ui8Data[0] = 0x01;
    USBHCDPipeWrite(psSPEInstance->ui32EP1OutPipe, ui8Data, 1);
    return(TRUE);
}

//*****************************************************************************
//
// This is the type definition a callback for events on USB Pipes allocated
// by USBHCDPipeAlloc().
//
// \param ui32Pipe is well the pipe
// \param ui32Event is well the event
//
// This prototype is used by any Pipe callbacks that are used in the host
// class drivers.  These functions typically handle data events like
// USB_EVENT_RX_AVAILABLE or USB_EVENT_TX_COMPLETE but can be sent other events
// depending on the USB host class in use.  See the documentation for the
// individual classes for the valid events for that class.
//
// \return None.
//
//*****************************************************************************
void
USBHSPEPipeCallback(uint32_t ui32Pipe, uint32_t ui32Event)
{
    if(ui32Pipe == g_sUSBHSPEDevice.ui32EP1InPipe && ui32Event == USB_EVENT_RX_AVAILABLE)
        SemPost(&g_sUSBHSPEDevice);

    if(ui32Pipe == g_sUSBHSPEDevice.ui32EP2InPipe && ui32Event == USB_EVENT_RX_AVAILABLE)
         SemPost(&g_sUSBHSPEDevice);
/*
	if (ui32Pipe == g_sUSBHSPEDevice.ui32EP1InPipe)
	{
		UARTprintf("ui32EP1InPipe");
	}
	else if (ui32Pipe == g_sUSBHSPEDevice.ui32EP1OutPipe)
	{
		UARTprintf("ui32EP1OutPipe");
	}
	else if (ui32Pipe == g_sUSBHSPEDevice.ui32EP2InPipe)
	{
		UARTprintf("ui32EP2InPipe");
	}
	else if (ui32Pipe == g_sUSBHSPEDevice.ui32EP6InPipe)
	{
		UARTprintf("ui32EP6InPipe");
	}

	if(ui32Event == USB_EVENT_RX_AVAILABLE)
		UARTprintf("USB_EVENT_RX_AVAILABLE\r\n");
	else if(ui32Event == USB_EVENT_TX_COMPLETE)
		UARTprintf("USB_EVENT_TX_COMPLETE\r\n");*/
}


