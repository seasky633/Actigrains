/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/

/*-----------------------------*
 *  Macros for others          *
 *----------------------------*/


/*-----------------------------*
 *  type definitions           *
 *----------------------------*/


/*-----------------------------*
 *  structure,union and enum   *
 *----------------------------*/


/*-----------------------------*
 *  public functions prototypes*
 *----------------------------*/
void	SetLedBlank(uint8);
void	ClrLedBlank(uint8);
void	LedOn(uint8);
void	LedProc(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/


/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
		

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/
static	uint8	g_u8LedVal;   //控制灯的开关
static  uint8	g_u8LedBlank; //控制灯的闪烁
static  uint8	g_u8LedBuf;   //有效值=开关&闪烁
uint8 xdata g_u8LedTxDAT1;
uint8 xdata g_u8LedTxDAT2;

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      设置LED闪烁位.                                                         *
 *  Parameters:                                                                *
 *      u8Led                                                                  *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	SetLedBlank(uint8 u8Led)
{
	g_u8LedBlank |=	u8Led;
	g_u8LedVal |= u8Led;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      清除LED闪烁位.                                                         *
 *  Parameters:                                                                *
 *      u8Led                                                                  *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	ClrLedBlank(uint8 u8Led)
{
	g_u8LedBlank &= ~u8Led;
	g_u8LedVal &= ~u8Led;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      设置LED开关位.                                                         *
 *  Parameters:                                                                *
 *      u8Led                                                                  *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LedOn(uint8 u8Led)
{
	g_u8LedVal |= u8Led;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      LED处理进程.                                                           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LedProc(void)
{
    g_u8LedTxDAT1 = 0;
    g_u8LedTxDAT2 = 0;
    
	if(bSys_1000MS)
	{
		if(bLEDBlank)
			bLEDBlank =	0;
		else
			bLEDBlank =	1;
	}
	
	if(bLEDBlank)
	{
		g_u8LedBuf = g_u8LedVal;
	}
	else
	{
		g_u8LedBuf = g_u8LedVal & (~g_u8LedBlank);
	}

    if((bPwroffline==1)||(bLidOCDEn==0)) //掉电或开盖时关闭指示灯
    {
        Enter_Bank1;
    	IO_LED_BKL = 0;		
        Enter_Bank0;	
    }
    else
    {    
    	if(TstBit(g_u8LedBuf,NUM_LED_START))
    	{
    	    SetBit(g_u8LedTxDAT1, NUM_LED_START);			      
    	}

    	if(TstBit(g_u8LedBuf,NUM_LED_STOP))
    	{
            SetBit(g_u8LedTxDAT1, NUM_LED_STOP);
    	}	

    	if(TstBit(g_u8LedBuf,NUM_LED_REHEAT))
    	{
            SetBit(g_u8LedTxDAT1, NUM_LED_REHEAT);
    	}

        if(TstBit(g_u8LedBuf,NUM_LED_WARM))
    	{
    		SetBit(g_u8LedTxDAT1, NUM_LED_WARM);		
    	}

        if(TstBit(g_u8LedBuf,NUM_LED_DELAY))
    	{
    	    SetBit(g_u8LedTxDAT1, NUM_LED_DELAY);		
    	}

        if(TstBit(g_u8LedBuf,NUM_LED_BKL))
    	{
    	    Enter_Bank1;
    		IO_LED_BKL = 1;		
            Enter_Bank0;
    	}
    	else
    	{
    	    Enter_Bank1;
    		IO_LED_BKL = 0;		
            Enter_Bank0;				
     	}

        g_u8LedTxDAT2 = g_u8LedTxDAT1<<3;
    }
}
