/*
 * usbspe.h
 *
 *  Created on: 2019Äê1ÔÂ11ÈÕ
 *      Author: Wangjiangbo
 */

#ifndef USBHSPE_H_
#define USBHSPE_H_


//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! \addtogroup usblib_host_class
//! @{
//
//*****************************************************************************

typedef struct tUSBHSPEInstance tUSBHSPEInstance;

//*****************************************************************************
//
// These defines are the the events that will be passed in the \e ui32Event
// parameter of the callback from the driver.
//
//*****************************************************************************
#define SPE_EVENT_OPEN          1
#define SPE_EVENT_CLOSE         2

//*****************************************************************************
//
// The prototype for the USB SPE host driver callback function.
//
//*****************************************************************************
typedef void (*tUSBHSPECallback)(tUSBHSPEInstance *psSPEInstance,
                                 uint32_t ui32Event,
                                 void *pvEventData);

//*****************************************************************************
//
// Prototypes for the USB SPE host driver APIs.
//
//*****************************************************************************
extern tUSBHSPEInstance * USBHSPEDriveOpen(uint32_t ui32Drive,
                                           tUSBHSPECallback pfnCallback);
extern void USBHSPEDriveClose(tUSBHSPEInstance *psSPEInstance);

extern uint16_t USBHSPEGetState(tUSBHSPEInstance *psSPEInstance,
                                  uint16_t  ui_TimeOut,
                                  uint16_t* puiPixels,
                                  uint32_t* pulIntegrationTime,
                                  uint16_t* pbLampEnable,
                                  uint16_t* puiPacketCount,
                                  uint16_t* pbUsbSpeed);

extern uint16_t USBHSPEQueryInfo(tUSBHSPEInstance *psSPEInstance,
                                  uint16_t  ui_TimeOut,
                                  uint16_t  ucIndex,
                                  uint16_t* pucString,
                                  uint16_t* uiStrLen);

extern uint16_t USBHSPESetIntegralTime(tUSBHSPEInstance *psSPEInstance,
                                  uint16_t  ui_TimeOut,
                                  uint32_t  ul_IntegralTime);

extern uint16_t USBHSPEInit(tUSBHSPEInstance *psSPEInstance,
                                  uint16_t  ui_TimeOut);
extern uint16_t USBHSPESetTrigger(tUSBHSPEInstance *psSPEInstance,
                                      uint16_t  ui_TimeOut,
                                      uint8_t  uc_Trigger);

extern uint16_t USBHSPEGetSpectrum(tUSBHSPEInstance *psSPEInstance,
                                   uint16_t  ui_TimeOut,
                                   int16_t*  i_Spectrum);

/*
extern int32_t USBHSPEDriveReady(tUSBHSPEInstance *psSPEInstance);
extern int32_t USBHSPEBlockRead(tUSBHSPEInstance *psSPEInstance,
                                uint32_t ui32LBA, uint8_t *pui8Data,
                                uint32_t ui32NumBlocks);
extern int32_t USBHSPEBlockWrite(tUSBHSPEInstance *psSPEInstance,
                                 uint32_t ui32LBA, uint8_t *pui8Data,
                                 uint32_t ui32NumBlocks);
*/
//*****************************************************************************
//
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* USBHSPE_H_ */
