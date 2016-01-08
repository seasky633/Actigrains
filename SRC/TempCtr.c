/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"
#include ".\Head\TempCtr.h"

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
void	KeepTempProc(void);
void	HeatProc(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/


/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	g_u8HiTemp;				//�¶�����
uint8	g_u8LoTemp;				//�¶�����
uint8	g_u8OnTime_Set;
uint8	g_u8OffTime_Set;
uint8	g_u8PwrCnt;
uint16	g_u16RelayTime;	        //�̵�������ʱ��

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/
static	uint8	u8OnTime;
static	uint8	u8OffTime;

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      �¶�ά�ֳ���                                   					       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	KeepTempProc(void)
{
	uint8	u8BotDegree	=	ADCGetData(AD_CH_BOT);

	if(g_u8HiTemp==0)
	{
		u8OnTime	=	0;
		return;
  	}

	if(bSys_1000MS)
	{ 
		if(bTempReach)
		{	
			//�¶��Ѿ�����,����Low�¶�
			if(u8BotDegree>=g_u8LoTemp)
			{
				//�¶Ȼ���Ŀ�귶Χ��
				u8OnTime	=	0;
				return;
	  		}
			else
			{
				//�Ѿ�����LowTemp
				bTempReach	=	0;
			}	
		}

		//�¶Ȼ�û�ﵽ
		if(u8BotDegree>=g_u8HiTemp)
		{
			//�¶ȴ��������¶�
			bTempReach	=	1;			//���¶��Ѿ��ﵽ����״̬
			u8OnTime	=	0;
 		}
		else
		{
			//�¶���Ȼ���������¶�
			//���ü�����Ȳ���
			u8OnTime	=	g_u8OnTime_Set;
			u8OffTime	=	g_u8OffTime_Set;
   		}
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ���ȿ��Ƴ���                                  				           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                               	   *
 *-----------------------------------------------------------------------------*/
void	HeatProc(void)
{
    if((bPwroffline==1)||(g_u16RelayDelayCnt<1000))                 //���磬�ؼ���,�ӳ�����
    {
        IO_HEAT_BOT	=	0;
		IO_HEAT_TOP	=	0;
		IO_HEAT_MID	=	0;
    }
    else
    {
    	if(u8OnTime==0) 			//�޹������
    	{
    		IO_HEAT_BOT	=	0;
    		bHeatState	=	0;
    		g_u8PwrCnt	=	0;			
    	}
    	else
    	{
    		if(u8OnTime==0xf0) 		//��������
    		{
    			IO_HEAT_BOT	=	1;
    			bHeatState	=	1;
    			g_u8PwrCnt	=	0;	
    			if(bSys_1000MS)	g_u16RelayTime++;	
    		}
    	   	else
    		{
    			if(bSys_1000MS)
    			{
    				g_u8PwrCnt++;

    				if(bHeatState)
    				{
    					//HeatProc_OnState
    					if(g_u8PwrCnt>=u8OnTime)
    					{
    						bHeatState	=	0;		//���ü������״̬Ϊ�Ͽ�״̬
    						g_u8PwrCnt	=	0;		//������ȼ�ʱ������
    						IO_HEAT_BOT	=	0;		
    					}
    					else
    					{
    						IO_HEAT_BOT	=	1;	
    						g_u16RelayTime++;		
    					}
    				}
    	    		else
    				{
    					//HeatProc_OffState
    					if(g_u8PwrCnt>=u8OffTime)
    					{
    						bHeatState	=	1;		//���ü������״̬Ϊ��״̬
    						g_u8PwrCnt	=	0;		//������ȼ�ʱ������
    						IO_HEAT_BOT	=	1;
    						g_u16RelayTime++;
    					}
    					else
    					{
    			 			IO_HEAT_BOT	=	0;
    					}
    				}	
    			}
    		}
    	}

		if(bHeatTopReq)
		{
			IO_HEAT_TOP	= 1;	
		}
		else
		{
			IO_HEAT_TOP = 0;
		}

		if(bHeatArdReq)
		{
			IO_HEAT_MID	=	1;
		}
		else
		{
			IO_HEAT_MID	=	0;
		}		
    }
}