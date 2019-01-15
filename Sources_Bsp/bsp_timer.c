
#include "Bsp.h"

static void (*s_time0hook)(void) = NULL;
static void (*s_time1hook)(void) = NULL;

__interrupt void cpu_timer0_isr(void)
{
    if(s_time0hook != NULL)
    {
        (*s_time0hook)();
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

__interrupt void cpu_timer1_isr(void)
{
    if(s_time1hook != NULL)
    {
        (*s_time1hook)();
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//初始化定时器0
void Bsp_Time0Init(float timeUs)
{
	ConfigCpuTimer(&CpuTimer0, 200, timeUs);	//200MHZf	
    
    EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TIMER0_INT = &cpu_timer0_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers
    
	//PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    IER |= M_INT1;
}

//开启定时器0
void Bsp_Time0Start(void)
{
	StartCpuTimer0();     // 1 = Stop timer, 0 = Start/Restart
}
//关闭定时器0
void Bsp_Time0Stop(void)
{
	StopCpuTimer0();     // 1 = Stop timer, 0 = Start/Restart
}
//开启定时器0中断
void Bsp_Time0IntEnable(void)
{
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}
//关闭定时器0中断
void Bsp_Time0IntDisable(void)
{
    PieCtrlRegs.PIEIER1.bit.INTx7 = 0;
}
//定时器0中断配置
void Bsp_Time0HookRegister(void (*hook)(void))
{    
    s_time0hook = hook;
}

//初始化定时器1
void Bsp_Time1Init(float timeUs)
{
	ConfigCpuTimer(&CpuTimer1, 200, timeUs);	//200MHZf

    EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TIMER1_INT = &cpu_timer1_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

}

//开启定时器1
void Bsp_Time1Start(void)
{
	StartCpuTimer1();     // 1 = Stop timer, 0 = Start/Restart
}
//关闭定时器1
void Bsp_Time1Stop(void)
{
	StopCpuTimer1();     // 1 = Stop timer, 0 = Start/Restart
}
//开启定时器1中断
void Bsp_Time1IntEnable(void)
{
	//PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    IER |= M_INT13;
}
//关闭定时器0中断
void Bsp_Time1IntDisable(void)
{
	//PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    IER &= (~M_INT13);
}
//定时器1中断配置
void Bsp_Time1HookRegister(void (*hook)(void))
{
    s_time1hook = hook;
}
