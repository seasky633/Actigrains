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
    2015-07-02	1. 开始编写程序
    2015-09-18  1. 调整油温
	2015-12-01  1. V3版程序修改
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
uint8	g_u8SecTick	= 0;				   	//时钟节拍计数器
uint8	bdata	 g_u8SysStatus[4];		   	//系统标志
sbit bSysTick	 =	g_u8SysStatus[0]^0;	   	//系统Tick标志
sbit bSys_1000MS =  g_u8SysStatus[0]^1;    	//系统1000ms标志
sbit bKeyPress   =  g_u8SysStatus[0]^2;    	//有无按键标志
sbit bADOK       =	g_u8SysStatus[0]^3;    	//AD转换完成标志
sbit bFirstPress = 	g_u8SysStatus[0]^4;    	//第一次按键标志
sbit bNewKey     =	g_u8SysStatus[0]^5;    	//新按键按下标志
sbit bLEDBlank   =	g_u8SysStatus[0]^6;	   	//闪灯标志
sbit bTempReach  =  g_u8SysStatus[0]^7;    	//达到温度值标志

sbit bHeatArdReq  = g_u8SysStatus[1]^0;     //侧部加热请求标志
sbit bHeatTopReq  = g_u8SysStatus[1]^1;     //顶部加热请求标志
sbit bCookBySctEN = g_u8SysStatus[1]^2;     //允许按温度曲线烹饪标志
sbit bSysPwrOn    = g_u8SysStatus[1]^3;     //系统上电标志
sbit bTempAch1st  = g_u8SysStatus[1]^4;     //第一次达到设定温度点
sbit bHeatState   = g_u8SysStatus[1]^5;     //加热状态标志
sbit bCalBookTime_EN = g_u8SysStatus[1]^6;	//允许预约计算标志
sbit bLEDComeback =	g_u8SysStatus[1]^7;	    //LED显示恢复标志

sbit bTimeAdjSt     = g_u8SysStatus[2]^0;   //时间调整判断标志
sbit bPwroffline    = g_u8SysStatus[2]^1;   //掉电
sbit bHeatPwrReq    = g_u8SysStatus[2]^2;   //加热总开关
sbit bTimeCountDown = g_u8SysStatus[2]^3;
sbit bTimeDisp_EN   = g_u8SysStatus[2]^4;
sbit bBookTime_EN   = g_u8SysStatus[2]^5;   //预约时间有效
sbit bTempDisp_EN   = g_u8SysStatus[2]^6;   
sbit bWarm_Start    = g_u8SysStatus[2]^7;   //开始保温

sbit bStageChgReq   = g_u8SysStatus[3]^0;	//控制跳转请求
sbit bDebug_EN      = g_u8SysStatus[3]^1;	
sbit bWarm_EN       = g_u8SysStatus[3]^2;	//保温标志
sbit bBoilChk_EN    = g_u8SysStatus[3]^3;	//沸腾判断有效
sbit bBoilReach     = g_u8SysStatus[3]^4;   //已沸腾标志
sbit bKeyMusic_EN   = g_u8SysStatus[3]^5;	//蜂鸣器标志
sbit bSoakStage     = g_u8SysStatus[3]^6;   //吸水阶段
sbit bHeatStage     = g_u8SysStatus[3]^7;   //加热阶段

uint8	bdata   g_u8ErrorType;		//报警类型
sbit bTopSensorErr   = g_u8ErrorType^0;     //上传感器错误标志
sbit bBotSensorErr   = g_u8ErrorType^1;     //下传感器错误标志
sbit bIGBTSensorErr  = g_u8ErrorType^2;     //IGBT传感器错误标志
sbit bNoPanErr       = g_u8ErrorType^3;    	//无锅标志
sbit bPanOverTempErr = g_u8ErrorType^4;    	//锅底温度过高
sbit bVoltageErr     = g_u8ErrorType^5;    	//电压异常标志
sbit bHaltHeatErr    = g_u8ErrorType^6;    	//不加热标志
sbit bIGBTOverTempErr= g_u8ErrorType^7;     //IGBT温度过高标志

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
		RSTSTAT	=  Bin(00000000);	// 喂狗 

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

//		//自检启动
        AutotestActivate();
		
		//故障检测
	  	FaultCheckProc();
		
		//沸腾判断
//		BoilCheckProc();

		//温度控制
		KeepTempProc();
		HeatProc();

        bSysPwrOn   =   0;
		bSys_1000MS	=	0;
	}
}
