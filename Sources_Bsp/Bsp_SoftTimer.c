#include  "Bsp.h"

typedef struct {
    INT16U  ul_Count;
    SoftTimer_t* ast_Buff[DEF_SOFT_TIMER_MAX];  
}SoftTimerList_t;

SoftTimerList_t st_SoftTimerList = {0,{0}};



BOOL Bsp_SoftTimerReg(SoftTimer_t* pst_Timer)
{
    if(st_SoftTimerList.ul_Count >= DEF_SOFT_TIMER_MAX || pst_Timer == NULL)
        return FALSE;
    Bsp_SoftTimerStop(pst_Timer);
    st_SoftTimerList.ast_Buff[st_SoftTimerList.ul_Count++] = pst_Timer;
    return TRUE;
}

BOOL Bsp_SoftTimerStart(SoftTimer_t* pst_Timer)
{
    if( pst_Timer == NULL || pst_Timer->b_Run == TRUE )
        return FALSE;
    
    if( pst_Timer->b_IsOneShot == TRUE && pst_Timer->ul_Dly == 0)
        return FALSE;
    
    if( pst_Timer->b_IsOneShot == FALSE && pst_Timer->ul_Period == 0)
        return FALSE;
    
    pst_Timer->ul_Cnt = 0;
    pst_Timer->b_First = TRUE;
    pst_Timer->b_Run = TRUE;
    return FALSE;
}

BOOL Bsp_SoftTimerStop(SoftTimer_t* pst_Timer)
{
    if( pst_Timer == NULL || pst_Timer->b_Run == FALSE )
        return FALSE;
    
    pst_Timer->ul_Cnt = 0;
    pst_Timer->b_First = TRUE;
    pst_Timer->b_Run = FALSE;
    
    return TRUE;
}

void Bsp_SoftTimerPoll(void)
{
    INT16U i;
    for (i = 0; i < st_SoftTimerList.ul_Count; i++)
    {
        SoftTimer_t* pst_Timer = st_SoftTimerList.ast_Buff[i];
        if( pst_Timer == NULL || pst_Timer->b_Run == FALSE)
            continue;
        
        if(pst_Timer->b_First == TRUE)
        {
            if ( ++pst_Timer->ul_Cnt >= pst_Timer->ul_Dly )
            {
                if(pst_Timer->b_IsOneShot == TRUE)
                {
                    Bsp_SoftTimerStop(pst_Timer);
                }
                else
                {
                    pst_Timer->ul_Cnt = 0;
                    pst_Timer->b_First = FALSE;
                }
                
                if (pst_Timer->cb_CallBack != NULL) 
                    (*pst_Timer->cb_CallBack)(pst_Timer);
            }
        }
        else
        {
            if ( ++pst_Timer->ul_Cnt >= pst_Timer->ul_Period )
            {
                pst_Timer->ul_Cnt = 0;
                pst_Timer->b_First = FALSE;
                
                if (pst_Timer->cb_CallBack != NULL) 
                    (*pst_Timer->cb_CallBack)(pst_Timer);
            }
        }
        
    }
}

void Bsp_DelayUs(INT32U us)
{
	DELAY_US(us);
} 

void Bsp_DelayMs(INT32U ms)
{
    while(ms--)
        Bsp_DelayUs(1000);
}
