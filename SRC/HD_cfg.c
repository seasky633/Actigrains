/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include".\Head\HD_cfg.h"

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
void	System_INIT(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/
static 	void 	SetSysClock(void);
static	void 	Port_INIT(void);
//static	void	PWM_INIT(void);
static 	void 	Timer2_INIT(void);
static  void    Timer3_INIT(void);
static 	void 	IE_INIT(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	bdata	SysTime;					
sbit	bSys1S_CookTime	=	SysTime^0;      
sbit	bSys1S_BookTime	=	SysTime^1; 

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/
uint8	m_u8SysTickCnt = 0;
uint8	g_u8PwrOnlineCnt =	0;
uint16 xdata g_u16RelayDelayCnt;

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the System.                                                 *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void System_INIT(void)
{
    EA	=	0;							//关闭全中断
	SetSysClock();
	Port_INIT();
	Timer2_INIT();
    Timer3_INIT();
	ADC_INIT();
    LCD_INIT();
//	PWM_INIT();
	IE_INIT();
    EA	=	1;							//开启全中断
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Configures the System clock frequency                                  *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static void SetSysClock(void)
{
	CLKCON	&=	Bin(10011111);		//Fsys=Foscs
	CLKCON	|=	Bin(00001000);		//设置HFON=1,打开OSCXCLK
	delay(5);					    //至少等待振荡器预热时间128*Tosc
	CLKCON	|=	Bin(00000100);		//设置FS=1,选择OSCXCLK作为OSCSCLK		  
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the GPIO.                                                   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void Port_INIT(void)
{
	// initialize ports
	//	 1 : P02	  o SEG19
	//   2 : P03      o IO_HEAT_ON
	//   3 : P04	  i IO_LID_OC
	//   4 : P05      o IO_HEAT_TOP
	//   5 : P06      o IO_HEAT_ARD
	//   6 : P07      o IO_HEAT_BOT
	//   7 : GND
	//   8 : XTAL1    i 32.768KHz
	//   9 : XTAL2    i 32.768KHz
	//  10 : P52      o BUZ
	//  11 : P53      o IO_LED_BKL
	//  12 : VDD
	//  13 : P54      o SEG20
	//  14 : P55      o SEG21
	//  15 : P47      o SEG22
	//  16 : P46      o SEG23
	//  17 : P45      o SEG24
	//  18 : P44	  o SCL
	//  19 : P43      o SDA
	//  20 : AN2      o AD_POWER_IN
	//  21 : AN1      i AD_TOP_IN
	//  22 : AN0	  i AD_BOT_IN
	//  23 : P37      o COM8
	//  24 : P36      o COM7
	//  25 : P35      o COM6
	//  26 : P34      o COM5
	//  27 : P33      o COM4
	//  28 : P32      o COM3
	//  29 : P31      o COM2
	//  30 : P30      o COM1
	//  31 : P10      o SEG1
	//  32 : P11      o SEG2
	//  33 : P12      o SEG3
	//  34 : P13      o SEG4
	//  35 : P14      o SEG5
	//  36 : P15      o SEG6
	//  37 : P16      o SEG7
	//  38 : P17      o SEG8
	//  39 : P20      o SEG9
	//  40 : P21      o SEG10
	//  41 : P22      o SEG11
	//  42 : P23      o SEG12
	//  43 : P24      o SEG13
	//  44 : P25      o SEG14
	//  45 : P26      o SEG15
	//  46 : P27      o SEG16
	//  47 : P00      o SEG17
	//  48 : P01      o SEG18
	
    P0SS	=	Bin(11111111);		//P4.7-P4.5/P5.5-P5.4/P0.2-P0.0作为SEG17~SEG24
    P1SS	=	Bin(11111111);		//P1.0~P1.7作为SEG1~SEG8
	P2SS	=	Bin(11111111);		//P2.0~P2.7作为SEG9~SEG16
	P3SS    =   Bin(11111111);      //P3.0~P3.7作为COM1~COM8

	//没有用作IO的引脚设置为输出（低电平）
	P0CR	=	Bin(11101111);      
	P0PCR	=	Bin(00000000);
	P0 		=	Bin(00000000);
    P0OS	=	Bin(00000000);
	
	P1CR	=	Bin(11111111);
	P1PCR	=	Bin(00000000);
	P1		=	Bin(00000000);
	
	P2CR    =	Bin(11111111);
	P2PCR	=	Bin(00000000);
	P2		=	Bin(00000000);
	
	P3CR	=	Bin(11111111);
	P3PCR	=	Bin(00000000);
	P3		=	Bin(00000000);
	
	P4CR	=	Bin(11111000);
	P4PCR	=	Bin(00011000);      //P4.3/P4.4上拉,模拟iic接口
	P4		=	Bin(00000000);
	
	Enter_Bank1;
	P5CR	=	Bin(00111100);
	P5PCR   = 	Bin(00000000);
	P5		=	Bin(00000000);
	Enter_Bank0;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the Timer2.                                                 *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static void Timer2_INIT(void)
{
	TR2		=	0;						// stop timer2
	TF2		=	0;                  	// 清定时器2溢出标志
	T2CON	=	Bin(00000000);			// 16位带重载功能的定时器
	T2MOD	=	Bin(00000000);			// 时钟源为系统时钟,仅作为递增计数器
	RCAP2L	=	LOBYTE(CLOCK_1MS);		// 载入定时初值
	RCAP2H	=	HIBYTE(CLOCK_1MS);
	TL2		=	LOBYTE(CLOCK_1MS); 	
	TH2		=	HIBYTE(CLOCK_1MS);
	TR2		=	1;						// start timer2
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the PWM.                                                    *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
//static void PWM_INIT(void)	
//{
//	PWMLO	=	0x55;			//解锁PWM寄存器
//	PWM0PL	=	186;			//设置PWM周期Period = 186*(16/12us) = 250us,Fpwm = 4Khz		
//	PWM0PH	=	0;
//	PWM0DL	=	93;			    //占空比1/2
//	PWM0DH	=	0;
//	PWM0C   =   Bin(00000011);	//高电平驱动,PWM时钟=(Fsoc/16)MHz	
//	PWMEN	=	0;				//PWM0输出禁止
//	PWMLO	=	0xAA;			//锁定PWM寄存器
//}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the Timer3.                                                 *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static void Timer3_INIT(void)
{
	Enter_Bank1;
    TR3  	= 	0;                      // stop timer3
    TF3  	= 	0;						// 清定时器3溢出标志
	T3PS0	=	0;						// 定时器3预分频比1/1
	T3PS1	=	0;
	T3CLKS0	=	0;						// 外部32.768Khz晶振
	T3CLKS1	=	1;
	TL3		=	LOBYTE(CLOCK_1S);		// 载入定时初值
	TH3		=	HIBYTE(CLOCK_1S);
	TR3     =   1;                      // start timer3	
	Enter_Bank0;
}
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the Interrupt.                                              *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static void IE_INIT(void)
{
	//	 		 Bit7	Bit6  Bit5  Bit4  Bit3  Bit2  Bit1  Bit0 
	//IEN0		 EA 	EADC  ET2   ES0    ---  EX1   ET5   EX0 
	//IEN1	  	 LPD    ET4   EPWM  ET3   EX4   EX3   EX2   ESPI
	//使能ADC中断、Timer2中断、Timer3中断
	IEN0	=	Bin(01100000);			
	IEN1	=	Bin(00010000);
	IENC	=	0;
	IENC1	=	Bin(00010000);//允许定时器3溢出中断
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      定时器2中断处理(中断入口地址 = 0x002B =43=N * 8 + 3=>>N = 5)     	   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	Timer2_ISP(void) interrupt 5
{
    uint8   u8Bankbak = INSCON;
    
    Enter_Bank0;
	EA	=	0;					//关闭全中断
	TF2	=	0;					//清除Timer2中断溢出标志,Timer2能自动重载	
	
	m_u8SysTickCnt++;

	if(m_u8SysTickCnt>=8) 
	{
		m_u8SysTickCnt = 0;
		bSysTick = 1;			// 系统8MS时钟节拍标志
	}

    if(bHeatPwrReq)             //交流驱动加热总开关
    {
        if(bPwroffline==0)
        {
            if(IO_HEAT_ON)
            {
                IO_HEAT_ON = 0;
            }
            else
            {
                IO_HEAT_ON = 1;
            }
        }
        
        if(g_u16RelayDelayCnt<1000)
        {
            g_u16RelayDelayCnt++;
        }
    }
    else
    {
        g_u16RelayDelayCnt = 0;
        IO_HEAT_ON = 0;   
    }

	EA			=	1;			//开启全中断
    INSCON = u8Bankbak;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      定时器3中断处理(中断入口地址 = 0x005B =91=N * 8 + 3=>>N = 11)     	   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	Timer3_ISP(void) interrupt 11
{
    uint8   u8Bankbak = INSCON;
    
    Enter_Bank0;  
	EA		=	0;							//关闭全中断
	
	Enter_Bank1;
	TF3		=	0;							//清除Timer3中断溢出标志
	Enter_Bank0;

	SysTime	= 0xff;							//系统1S时钟
	EA		=	1;							//开启全中断
    INSCON = u8Bankbak;
}

