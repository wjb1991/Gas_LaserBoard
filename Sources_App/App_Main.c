#include "App_Include.h"

int main(void)
{
    // Step 1. Initialize System Control: // PLL, WatchDog, enable Peripheral Clocks
    DINT;

    InitSysCtrl();

    // Step 2. Initialize GPIO:
    InitGpio();

    InitCpuTimers();    // For this example, only initialize the Cpu Timers

    DMAInitialize();    //Initialize DMA

    Emif1Initialize();  //Initialize EMIF1
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

#ifdef _STANDALONE
#ifdef _FLASH
    //  Send boot command to allow the CPU02 application to begin execution
    IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
#else
    //  Send boot command to allow the CPU02 application to begin execution
    IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_RAM);
#endif
#endif

    // Step 4. Initialize the Device Peripherals:
    App_DeviceInit();

    // Step 5. User specific code, enable interrupts:

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    // Step 6. IDLE loop. Just sit and loop forever (optional):
    for(;;)
    {
        App_DevicePoll();
    }
}
