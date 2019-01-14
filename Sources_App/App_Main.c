#include "App_Include.h"

int main(void)
{
    // Step 1. Initialize System Control: // PLL, WatchDog, enable Peripheral Clocks
    DINT;

    InitSysCtrl();

    // Step 2. Initialize GPIO:
    InitGpio();

    InitCpuTimers();   // For this example, only initialize the Cpu Timers

    DMAInitialize();    //Initialize DMA

    // Step 3. Clear all interrupts and initialize PIE vector table:Disable CPU interrupts

    // Initialize PIE control registers to their default state. The default state is all PIE interrupts disabled and flags are cleared.
    InitPieCtrl();
    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;
    // Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
    // This will populate the entire table, even if the interrupt is not used in this example.
    // This is useful for debug purposes.
    InitPieVectTable();

    // Step 4. Initialize the Device Peripherals:

    SysCtlPeripheralEnable(SYSCTL_PERIPH_SCI1);

    EALLOW;
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 2;
    GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 2;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO64 = 1;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO65 = 1;
    EDIS;

    UARTStdioConfig(0, 230400, SysCtlLowSpeedClockGet(SYSTEM_CLOCK_SPEED));
    UARTprintf("\n==================USB Host Connect Usb4000 Test==================\n");


    Mod_Usb4000Init();
    Mod_UsbHostInit();


    // Step 5. User specific code, enable interrupts:

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    // Step 6. IDLE loop. Just sit and loop forever (optional):
    for(;;)
    {
        Mod_UsbHostPoll();
        Mod_Usb4000Poll();
    }
}
