#ifndef _SYSTEM_H
#define	_SYSTEM_H

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
#define     BIT_KEY_REHEAT      0
#define     BIT_KEY_KEEPWARM    1
#define     BIT_KEY_DELAY       2
#define     BIT_KEY_STOP        3
#define		BIT_KEY_PLUS		4
#define     BIT_KEY_START		5
#define		BIT_KEY_MINUS		6
#define     BIT_KEY_CANCEL      7
#define		BIT_KEY_DIY  		8	
#define		BIT_KEY_TIMER		9	
#define     BIT_KEY_MENU	 	10

#define     VAL_KEY_REHEAT 		0x0001	
#define     VAL_KEY_KEEPWARM    0x0002
#define     VAL_KEY_DELAY       0x0004
#define     VAL_KEY_STOP  	 	0x0008
#define     VAL_KEY_PLUS        0x0010
#define     VAL_KEY_START       0x0020
#define     VAL_KEY_MINUS       0x0040
#define		VAL_KEY_CANCEL      0x0080	
#define		VAL_KEY_DIY  		0x0100
#define		VAL_KEY_TIMER  		0x0200
#define		VAL_KEY_MENU 		0x0400

//系统状态
#define		SYS_ID_RESET	0  		//系统复位的ID
#define		SYS_ID_READY	1		//系统待机的ID
#define     SYS_ID_SETUP    2       //系统功能设置的ID
#define	    SYS_ID_RUN		3		//系统开机运行的ID
#define	    SYS_ID_WARM		4		//系统保温的ID
#define     SYS_ID_POWRDOWN 5       //系统掉电的ID
#define     SYS_ID_ERROR    6       //系统报警的ID
#define		SYS_ID_CHECK	7		//系统自检的ID

//功能ID
#define     FUNC_ID_WHITERICE   	1
#define     FUNC_ID_BROWNRICE   	2
#define     FUNC_ID_BUCKWHEAT   	3
#define     FUNC_ID_QUINOA          4
#define     FUNC_ID_LENTILS   	    5
#define     FUNC_ID_BEANS       	6
#define     FUNC_ID_BABYPORRIDGE   	7
#define     FUNC_ID_BROWNRICE_LENTILS 8
#define     FUNC_ID_CHICKPEAS       9

#define     FUNC_ID_RISOTTO         10
#define     FUNC_ID_STEAM           11
#define     FUNC_ID_SLOWCOOK        12
#define     FUNC_ID_BROWNING        13
#define     FUNC_ID_PASTA           14
#define     FUNC_ID_BAKING          15
#define     FUNC_ID_SOUP            16
#define     FUNC_ID_BREADRISING     17
#define     FUNC_ID_DESSERT         18
#define     FUNC_ID_CREAMCHEESE     19
#define     FUNC_ID_YOGURT          20
#define     FUNC_ID_JAM  			21
#define     FUNC_ID_DIY             22
#define     FUNC_ID_REHEAT          23

//口感ID
#define     TEXTURE_ID_NUTRITIOUS   1
#define		TEXTURE_ID_HARD			2
#define		TEXTURE_ID_NORMAL		3
#define		TEXTURE_ID_SOFT			4


//设置阶段定义
#define     SET_PHASE_OVER           0
#define     SET_PHASE_MENU           1
#define     SET_PHASE_TEXTURE        2
#define     SET_PHASE_PEOPLE         3
#define     SET_PHASE_PUTBASKET      4
#define     SET_PHASE_FOODCUPS       5
#define     SET_PHASE_WATERCUPS      6
#define     SET_PHASE_DEFAULT        7
#define     SET_PHASE_TIME           8
#define     SET_PHASE_TEMPERATURE    9
#define     SET_PHASE_DIY            10
#define     SET_PHASE_AGE            11
#define     SET_PHASE_DELAY          12
#define     SET_PHASE_DIYDEFAULT     13


//运行阶段定义
#define     RUN_PHASE_START          1
#define     RUN_PHASE_DRAINED        2
#define     RUN_PHASE_DRAINEDFOOD    3
#define     RUN_PHASE_FOODCUPS       4
#define     RUN_PHASE_WATERCUPS      5
#define     RUN_PHASE_TIME           6   
#define     RUN_PHASE_TEMPERATURE    7
#define     RUN_PHASE_DELAY          8
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
extern	void	UIProc(void);
extern	void	FaultCheckProc(void);
extern  void    AutotestActivate(void);
extern	void	EnterReset(void);
extern	void	EnterSetup(void);
extern  void	EnterWarm(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern	uint8	m_u8SysID;
extern  uint8   m_u8ChkCnt;
extern	uint8	g_u8SecTick;
extern  uint8   m_u8PreTmp;
extern  uint8   u8CookTemp;

extern	uint16  code SoakTimeRiseTab[][6];   //吸水升温时间   
extern  uint16  code SoakTimeKeepTab[][6];   //吸水维持时间 
extern  uint8   code SoakTempKeepTab[][6];   //吸水维持温度  
extern  uint16  code MashTimeTab[][6];       //糊化时间  
extern  uint8   code CountDownTimeTab[][6];  //倒计时时间

#endif
