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
void	SoundProc(void);
void	PlaySound(uint8	code *pMusic);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/


/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	m_u8OnTime	=	0;
uint8	m_u8OffTime	=	0;
uint8	code	*m_u8pMusic	=	NULL;
uint8   code	*m_u8pMusicBak =  NULL;		//����ָ�룬�����ظ�����

uint16	code	MetroTable[] =
{
//	0ms		80ms	200ms   500ms   1000ms  2000ms	
	0,		10,		25,		63,		125,	250
};

uint8	code	MusicPoweron[] =
{
    M_1000|S_200,0x00
};

uint8	code	MusicKeyDown[] =
{
    M_200|S_200,0x00
};

uint8	code	MusicSysError[] =
{
    M_500|S_500,M_500|S_500,M_500|S_500,M_500|S_500,M_500|S_500,
	M_500|S_500,M_500|S_500,M_500|S_500,M_500|S_500,M_500|S_500,0x00
};

uint8	code	MusicCookEnd[] =
{
    M_1000|S_500,M_1000|S_500,M_1000|S_500,0x00
};

uint8 code	MusicCountDown[]	=
{
	M_1000|S_500,M_1000|S_500,0x00
};

uint8 code  MusicInputFood[] =
{
	M_200|S_200,M_200|S_200,0x00   
};

uint8 code	MusicCycleEnd[] =
{
	M_80|S_80,M_80|S_80,0x00
};

uint8 code MusicOFF[] =
{
	0x00
};

uint8 code MusicAchievedTemp[] =
{
	M_1000|S_200,M_1000|S_200,0x00
};
/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      �������ź���                                                           *
 *  Parameters:                                                                *
 *      ���������׵�ַ                                                         *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	PlaySound(uint8	code	*pMusic)
{
	m_u8pMusicBak	=  	m_u8pMusic  =	pMusic;
	m_u8OnTime		=	0;
	m_u8OffTime	=	1;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      �����������                                                           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	SoundProc(void)
{
	uint8	u8Tmp;

	if(m_u8OnTime>0)
	{
		m_u8OnTime--;
	}
	else	
	{
		if(m_u8OffTime>0)
		{		
//			PWMLO	=	0x55;			//����PWM�Ĵ���
//			PWMEN	=	0;				//PWM0�����ֹ
//			PWMLO	=	0xAA;			//����PWM�Ĵ���
//			P0CR    &=  0xf7;
//			P0PCR   |=  0X08;
            Enter_Bank1;
            IO_BUZ  =   0;
            Enter_Bank0;
            
			m_u8OffTime--;

			if(m_u8OffTime==0)
			{    
				//ȡ����Ƶ��,�ͽ���
				u8Tmp	=	*m_u8pMusic;
				if(u8Tmp	==	0x00)
				{ 	
					//���ֽ���
			   		return;			
				}
				else	
				{
					if(u8Tmp  ==  0xff)
					{     
						//����ѭ��
			    		m_u8pMusic  =   m_u8pMusicBak;
               			m_u8OnTime	=	0;
	            		m_u8OffTime	=	1;
						return;
             		}
	
					u8Tmp		&=	0x0f;				  //ǰ4λΪ��ʱ��
					m_u8OnTime	=	MetroTable[u8Tmp];

            		u8Tmp		=	*m_u8pMusic++;
					u8Tmp		=	(u8Tmp>>4)&0x0f;      //��4λΪ�ر�ʱ��
					m_u8OffTime	=	MetroTable[u8Tmp];
					
//					PWMLO	=	0x55;			//����PWM�Ĵ���
//					PWMEN	=	0x01;			//PWM0�������
//					PWMLO	=	0xAA;			//����PWM�Ĵ���
                    Enter_Bank1;
                    IO_BUZ  =   1;
                    Enter_Bank0;
				}
			}
		}
	}
}
