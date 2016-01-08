/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"
#include ".\Head\Time.h"

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
void	IncTime(uint8	*pTime);
void	DecTime(uint8	*pTime);
void	IncTime_MIN(uint8	*pTime);
//void	IncTime_Hour(uint8	*pTime);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/


/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	tmCookTime[3];	//��⿵���ʱʱ��
uint8   tmBookTime[3];  //ԤԼʱ��

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ʱ������1��        													   *
 *  Parameters:                                                                *
 *      ��Ҫ���ӵ�ʱ���ַָ��                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	IncTime(uint8	*pTime)
{
	pTime[0]++;
	if(pTime[0]<60)	
	{
		return;
	}
    else
	{
		pTime[0]	=	0;
		IncTime_MIN(pTime);
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ʱ�����1��        													   *
 *  Parameters:                                                                *
 *      ��Ҫ���ٵ�ʱ���ַָ��                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	DecTime(uint8	*pTime)
{	
	pTime[0]--;
	if(pTime[0]==0xff)
	{
		pTime[0]	=	59;
		pTime[1]--;
		if(pTime[1]==0xff)
		{
			pTime[1]	=	59;
			pTime[2]--;
            if(pTime[2]==0xff)
			{
				pTime[2]	=	23;
			}
	  	}
  	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ʱ������1����       												   *
 *  Parameters:                                                                *
 *      ��Ҫ���ӵ�ʱ���ַָ��                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	IncTime_MIN(uint8	*pTime)
{
	pTime[1]++;
	if(pTime[1]<60)
	{
		return;
	}
	else
	{
		pTime[1]	=	0;
		pTime[2]++;
		if(pTime[2]==24)
		{
			pTime[2]	=	0;
		}
  	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ʱ������1Сʱ      												   	   *
 *  Parameters:                                                                *
 *      ��Ҫ���ӵ�ʱ���ַָ��                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
//void	IncTime_Hour(uint8 	*pTime)
//{
//	pTime[2]++;
//	if(pTime[2]>=24)  pTime[2]	=	0;
//}
