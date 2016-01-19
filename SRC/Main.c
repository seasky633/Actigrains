//==============================================================================
//	Project     : Actigrains Multicooker(fz23)
//	Date	    : 2015.7.2
//	Author      : wuyong

//	Device      : SH79F3283U(TQFP48)
//	Voltage     : 5V
//	Frequency	: Internal 12 MHz OSC,external 32.768KHz OSC for RTC & LCD
//  ADC range   : 5V (Vdd)
//==============================================================================
//	History :
/*
    2015-07-02	1. ��ʼ��д����
    2015-09-18  1. ��������
	2015-12-01  1. V3������޸�
    2016-01-19  1. Modification on the quantity of grains and water
//==============================================================================

/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"
#include ".\Head\System.h"
#include ".\Head\TempCtr.h"
#include ".\Head\Script.h"

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	g_u8SecTick	= 0;				   	//ʱ�ӽ��ļ�����
uint8	bdata	 g_u8SysStatus[4];		   	//ϵͳ��־
sbit bSysTick	 =	g_u8SysStatus[0]^0;	   	//ϵͳTick��־
sbit bSys_1000MS =  g_u8SysStatus[0]^1;    	//ϵͳ1000ms��־
sbit bKeyPress   =  g_u8SysStatus[0]^2;    	//���ް�����־
sbit bADOK       =	g_u8SysStatus[0]^3;    	//ADת����ɱ�־
sbit bFirstPress = 	g_u8SysStatus[0]^4;    	//��һ�ΰ�����־
sbit bNewKey     =	g_u8SysStatus[0]^5;    	//�°������±�־
sbit bLEDBlank   =	g_u8SysStatus[0]^6;	   	//���Ʊ�־
sbit bTempReach  =  g_u8SysStatus[0]^7;    	//�ﵽ�¶�ֵ��־

sbit bHeatArdReq  = g_u8SysStatus[1]^0;     //�ಿ���������־
sbit bHeatTopReq  = g_u8SysStatus[1]^1;     //�������������־
sbit bCookBySctEN = g_u8SysStatus[1]^2;     //�����¶�������⿱�־
sbit bSysPwrOn    = g_u8SysStatus[1]^3;     //ϵͳ�ϵ��־
sbit bTempAch1st  = g_u8SysStatus[1]^4;     //��һ�δﵽ�趨�¶ȵ�
sbit bHeatState   = g_u8SysStatus[1]^5;     //����״̬��־
sbit bCalBookTime_EN = g_u8SysStatus[1]^6;	//����ԤԼ�����־
sbit bLEDComeback =	g_u8SysStatus[1]^7;	    //LED��ʾ�ָ���־

sbit bTimeAdjSt     = g_u8SysStatus[2]^0;   //ʱ������жϱ�־
sbit bPwroffline    = g_u8SysStatus[2]^1;   //����
sbit bHeatPwrReq    = g_u8SysStatus[2]^2;   //�����ܿ���
sbit bTimeCountDown = g_u8SysStatus[2]^3;
sbit bTimeDisp_EN   = g_u8SysStatus[2]^4;
sbit bBookTime_EN   = g_u8SysStatus[2]^5;   //ԤԼʱ����Ч
sbit bTempDisp_EN   = g_u8SysStatus[2]^6;   
sbit bWarm_Start    = g_u8SysStatus[2]^7;   //��ʼ����

sbit bStageChgReq   = g_u8SysStatus[3]^0;	//������ת����
sbit bDebug_EN      = g_u8SysStatus[3]^1;	
sbit bWarm_EN       = g_u8SysStatus[3]^2;	//���±�־
sbit bBoilChk_EN    = g_u8SysStatus[3]^3;	//�����ж���Ч
sbit bBoilReach     = g_u8SysStatus[3]^4;   //�ѷ��ڱ�־
sbit bKeyMusic_EN   = g_u8SysStatus[3]^5;	//��������־
sbit bSoakStage     = g_u8SysStatus[3]^6;   //��ˮ�׶�
sbit bHeatStage     = g_u8SysStatus[3]^7;   //���Ƚ׶�

uint8	bdata   g_u8ErrorType;		//��������
sbit bTopSensorErr   = g_u8ErrorType^0;     //�ϴ����������־
sbit bBotSensorErr   = g_u8ErrorType^1;     //�´����������־
sbit bIGBTSensorErr  = g_u8ErrorType^2;     //IGBT�����������־
sbit bNoPanErr       = g_u8ErrorType^3;    	//�޹���־
sbit bPanOverTempErr = g_u8ErrorType^4;    	//�����¶ȹ���
sbit bVoltageErr     = g_u8ErrorType^5;    	//��ѹ�쳣��־
sbit bHaltHeatErr    = g_u8ErrorType^6;    	//�����ȱ�־
sbit bIGBTOverTempErr= g_u8ErrorType^7;     //IGBT�¶ȹ��߱�־

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Main Function.                                                         *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void main(void)
{   	
	System_INIT();
	EnterReset();

	while(1)
	{
		RSTSTAT	=  Bin(00000000);	// ι�� 

		if(!bSysTick)continue;
		bSysTick	=	0;

		g_u8SecTick++;

		if(g_u8SecTick>=125)
		{
			g_u8SecTick	=	0;
			bSys_1000MS	=	1;
		}

		ADCProc();	
		LCDProc(m_u8SysID);	   	
		LedProc();
		KeyProc();
		
		UIProc();
        SoundProc();
		ScriptProc();

//		//�Լ�����
        AutotestActivate();
		
		//���ϼ��
	  	FaultCheckProc();
		
		//�����ж�
//		BoilCheckProc();

		//�¶ȿ���
		KeepTempProc();
		HeatProc();

        bSysPwrOn   =   0;
		bSys_1000MS	=	0;
	}
}
