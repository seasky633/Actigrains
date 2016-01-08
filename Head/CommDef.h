#ifndef _COMMDEF_H
#define	_COMMDEF_H

	extern	uint8	bdata   g_u8ErrorType;
	
	//1S时钟标志(用于定时&预约)
	extern	bit	bSys1S_CookTime;
	extern	bit	bSys1S_BookTime;
	
	//系统标志
	extern	bit bSysTick;
	extern	bit bSys_1000MS;
	extern	bit bKeyPress;    	
	extern	bit bADOK;    	
	extern	bit bFirstPress;    	
	extern	bit bNewKey;    	
	extern	bit bLEDBlank;	   	
	extern	bit bTempReach;    	
	extern	bit bHeatArdReq;     
	extern	bit bHeatTopReq;     
	extern	bit bCookBySctEN;     
	extern	bit bSysPwrOn;     
	extern	bit bTempAch1st;   
	extern	bit bHeatState;     
	extern	bit bCalBookTime_EN;	
	extern	bit bLEDComeback;	    

	extern	bit bTimeAdjSt;  
	extern	bit bPwroffline;   
	extern	bit bHeatPwrReq;   
	extern	bit bTimeCountDown;
	extern	bit bTimeDisp_EN;
	extern	bit bBookTime_EN;  
	extern	bit bTempDisp_EN;   
	extern	bit bWarm_Start;   

	extern	bit bStageChgReq;	
	extern	bit bDebug_EN;	
	extern	bit bWarm_EN;	
	extern	bit bBoilChk_EN;	
	extern	bit bBoilReach;   
	extern	bit bKeyMusic_EN;	
    extern  bit bSoakStage;
    extern  bit bHeatStage;

	//报警类型
    extern  bit bTopSensorErr;  
    extern  bit bBotSensorErr;   
    extern  bit bIGBTSensorErr; 
    extern  bit bNoPanErr;      
    extern  bit bPanOverTempErr; 
    extern  bit bVoltageErr;     
    extern  bit bHaltHeatErr;    
    extern  bit bIGBTOverTempErr;
	
	//报警阶段
	#define	SYS_ERROR_CHECK_START	0
	#define	SYS_ERROR_CHECK_END		0xA5
	
/* ------------------- Bank Switch-------------------- */	
    #define	 Enter_Bank0	INSCON&=0x3f; 
    #define  Enter_Bank1    {INSCON&=0x3f;INSCON|=0x40;}
	
/* ------------------- BIT Register-------------------- */

 
	#define     IO_HEAT_TOP     P0_5  
    #define     IO_HEAT_MID     P0_6
    #define     IO_HEAT_BOT     P0_7
    #define     IO_HEAT_ON      P0_3
    #define     IO_LID_OC       P0_4
    #define     IO_BUZ          P5_2
    #define     IO_LED_BKL      P5_3

    #define	NULL	0
	#define	FALSE	0
	#define	TRUE	1
	
	#define	DEBUG0

#endif
