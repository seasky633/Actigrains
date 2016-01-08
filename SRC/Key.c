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
void   KeyProc(void);
uint16 GetKey(void);
uint8  IsNewKey(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint16	m_u16KeyVal;
uint8   xdata m_u8LidOCDcnt;
Bool bLidOCDEn; //0:open,1:closed
/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/


/*-----------------------------------------------------------------------------*
  *  Function Description:                                                      *
  *      从IO口上读取键值                                                       *
  *  Parameters:                                                                *
  *      None                                                                   *
  *  Return                                                                     *
  *      键值                                                                   *
  *----------------------------------------------------------------------------*/
uint16  GetKeyVal(void)
{
    uint8 apk;

    if(bLidOCDEn)
    {
        //Lid is closed
        if(IO_LID_OC==1)    //Open ?
        {
            m_u8LidOCDcnt++;
            if(m_u8LidOCDcnt>20)
            {
               m_u8LidOCDcnt = 0;
               bLidOCDEn = 0;   //open yes
            }
        }
        else
        {
            m_u8LidOCDcnt = 0;
        }
    }
    else
    {
        //Lid is open
        if(IO_LID_OC==0)    //closed ?
        {
            m_u8LidOCDcnt++;
            if(m_u8LidOCDcnt>20)
            {
                m_u8LidOCDcnt = 0;
                bLidOCDEn = 1;  //closed yes
            }
        }
        else
        {
            m_u8LidOCDcnt = 0;
        }
    }

    m_u16KeyVal	=  0x00;	
    i2c_read();
    if(bLidOCDEn)
    {
        apk=(rdat1<<3);
        if(rdat2==apk)
        {
            if(rdat1==1)  m_u16KeyVal |= (1<<0);		//"Reheat"
            if(rdat1==2)  m_u16KeyVal |= (1<<1);		//"KeepWarm"   
            if(rdat1==3)  m_u16KeyVal |= (1<<2);		//"Delay"
            if(rdat1==4)  m_u16KeyVal |= (1<<3);		//"Stop"   
            if(rdat1==5)  m_u16KeyVal |= (1<<4);		//"Plus"
            if(rdat1==6)  m_u16KeyVal |= (1<<5);		//"Start"   
            if(rdat1==7)  m_u16KeyVal |= (1<<6);		//"Minus"
            if(rdat1==8)  m_u16KeyVal |= (1<<7);		//"Cancel"   
            if(rdat1==9)  m_u16KeyVal |= (1<<8);		//"DIY"
            if(rdat1==10) m_u16KeyVal |= (1<<9);	    //"Timer"   
            if(rdat1==11) m_u16KeyVal |= (1<<10);	    //"Menu"
        }
    }
	return	m_u16KeyVal;
}


/*                     下面程序可移植                    */

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      扫键进程,连键时间=(100-95)*8+(100-0)*8+(100-85)*8*(n-2)                *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
uint16	g_u16KeyRpyCnt = 0;     
static	uint16	m_u16KeyAct = 0;	
uint16  g_u16KeyUpAct = 0;       
#define	KEY_START_TIME	85; //95
#define	KEY_RPY_TIME	85
#define	KEY_END_TIME	100

void	KeyProc(void)	
{
	static uint8	u8KeyPressCnt	=	0;
	static uint16	u16PrevKey	=	0;
	uint16	u16Key	=	GetKeyVal();	

	if(u16Key==u16PrevKey)
	{
		u8KeyPressCnt++; 

		if(u8KeyPressCnt>KEY_END_TIME)
		{
			m_u16KeyAct	=	u16PrevKey;	
			
			if(m_u16KeyAct==0)
			{
				bKeyPress	=	0;		
			}
			else
			{
				bKeyPress	=	1;	
			}
			
			g_u16KeyRpyCnt++;	
			if(bFirstPress)
			{
				u8KeyPressCnt	=	0;
				bFirstPress		=	0;
                if((u16Key)&&(bNewKey))
				{
				    g_u16KeyUpAct = m_u16KeyAct;
                    if(bKeyMusic_EN) 
                    {
                        PlaySound(MusicKeyDown);
                    }
				}
			} 
			else
			{
				u8KeyPressCnt	=	KEY_RPY_TIME;
			}
		}
	}
	else
	{ 
		if(!bKeyPress)	
		{
			bNewKey		=	1;		
		}
		else
		{
			bNewKey		=	0;		
		}
		m_u16KeyAct		=	0;      	
		g_u16KeyRpyCnt	=	0;			
		bFirstPress		=	1; 			
		u16PrevKey		=	u16Key; 		
		u8KeyPressCnt	=	KEY_START_TIME;  
	}
}    

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      读取键值                                                      		   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      经过处理后的键值                                                       *
 *----------------------------------------------------------------------------*/
uint16	GetKey(void)
{
	uint16	u16Ret	=	m_u16KeyAct;
	m_u16KeyAct	=	0;
	return	u16Ret;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      判断是否按下新键                                                       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      判断结果                                                        	   *
 *----------------------------------------------------------------------------*/
uint8	IsNewKey(void)
{
	if(bNewKey)
	{
		bNewKey	=	0;
		return	TRUE;
	}
	return	FALSE;
}
