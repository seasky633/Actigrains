/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"
#include ".\Head\System.h"
#include ".\Head\Time.h"
#include ".\Head\TempCtr.h"
#include ".\Head\Script.h"
#include ".\head\SinoLib.h"

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
void	UIProc(void);
void	FaultCheckProc(void);
void    AutotestActivate(void);
void	EnterReset(void);
void	EnterSetup(void);
void	EnterWarm(void);
void	EnterDefault(void);
void    EnterRun(void);
void    EnterPowerDown(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/
static	void	ResetProc(void);
static	void	SetupProc(void);
static	void	RunProc(void);
static	void	JudgeTimeValid(void);
static  void    FuncSetDisp(uint8 Step);
static  void    FuncRunDisp(uint8 Step);
static	void	SetDefCookTime(uint8 CookMode);
static  void    SetDefCookTemp(uint8 CookMode); 
static  void    SetDIYModeByDIY(uint8 diymode);
static  void    SetCupsWater(uint8 cookmode,uint8 texture,uint8 people);
static	void	LoadVarParam(uint8 cookMode, uint8 texture,uint8 people);
static  void	WarmProc(void);
static  void    PowerDownProc(void);
static	void	ErrorProc(void);
static	void	SelfCheckProc(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8	m_u8SysID;	  	            //系统状态变量
uint8   m_u8PrevSysID;  	        //掉电之前系统状态变量
uint8   m_u8CookMode;     	        //烹饪模式，0~22
uint8   m_u8DIYMode;	            //DIY模式:DIY1,DIY2,DIY3,DIY4,DIY5	
uint8   m_u8Age;                    //Babyporridge 年龄选择
uint8	m_u8Feel;		            //口感模式，0 无效，1 Nutritious，  2 Hard， 3 Normal，  4 Soft
uint8   m_u8People;                 //人数
uint16  m_u16Cups;	                //食物量杯数
uint16  m_u16Water;                 //水量
uint8   m_u8Foodtype;               //区分需要放入两种食物的功能
uint8   m_u8Watertype;              //区分第一次和第二次放水
uint8	m_u8PreTmp;					//温度初始值
uint8	m_u8TimeDispCnt;			//时间闪烁显示计数器
uint8   m_u8ChkCnt;				
uint8   u8CookTemp;				    //烹饪温度
uint8   MaxCookTemp;				//最高的烹饪温度
uint8   DefCookTemp;                //默认的烹饪温度
uint8   MinCookTemp;                //最低的烹饪温度
uint8	idata  tmMaxCookTime[3];    //最长的烹饪时间
uint8	idata  tmDefCookTime[3];    //默认的烹饪时间
uint8	idata  tmMinCookTime[3];	//最短的烹饪时间
uint8	idata  tmCalCookTime[3];	//预约结束，烹饪开始时间
uint8	idata  tmIntCookTime[3];	//间隔时间==预约设定时间-当前时间
uint8   idata  tmMinBookTime[3];	//最短的预约时间
uint8   idata  m_u8DispDelayCnt;    //显示延时计数器
uint8	idata  m_u8CurTimeDispCnt;
uint8   idata  m_u8AutoRunCnt;		//自动运行计数器
uint8   idata  u8ErrorCnt;          //报警计数器
uint8   idata  m_u8SelfChkStep;		//自检步骤
uint16  idata  u16PoweronCnt;       //上电自检计数器

uint8	idata	m_u8SetStep;		//设置阶段步骤
uint8   idata   m_u8RunStep;        //运行阶段步骤
uint8   idata   tmDebugTime[3];     //调试用
uint8   xdata   m_u8DIYCookmode; 
uint8   xdata   m_u8DIYFeel;
uint8   xdata   m_u8DIYPeople;
uint8   xdata   m_u8DIYAge;
uint8   xdata   m_u8LastAge;
uint8   xdata   m_u8ResetCnt;
//烹饪时间表						   Steam   Slowck  Browning Pasta   Bake    Soup    
uint16  code	MinCookTime_Table[]	= {0x0005, 0x0005, 0x0005, 0x0003, 0x0005, 0x0005, 
//                                     Bread   Dessert Cream   Yogurt  Jam     DIY     Reheat
                                       0x0005, 0x0005, 0x0030, 0x0100, 0x0005, 0x0001, 0x0005}; 
uint16	code	DefCookTime_Table[]	= {0x0030, 0x0130, 0x0020, 0x0008, 0x0040, 0x0040, 
                                       0x0030, 0x0045, 0x0200, 0x0800, 0x0030, 0x0030, 0x0020};
uint16	code	MaxCookTime_Table[]	= {0x0300, 0x0900, 0x0130, 0x0100, 0x0400, 0x0300, 
                                       0x0900, 0x0200, 0x0400, 0x2400, 0x0200, 0x0900, 0x0100};

//烹饪温度表  					       Slowcook Dessert  Crust    Bake   Bread   Reheat   DIY
uint8	code 	MinCookTemp_Table[]	= {0x50,    0x64,    0x64,    0x64,  0x1E,   0x50,    0x28};
uint8   code    DefCookTemp_Table[] = {0x64,    0x8C,    0xA0,    0x91,  0x28,   0x64,    0x64};
uint8   code    MaxCookTemp_Table[] = {0x64,    0xA0,    0xA0,    0xA0,  0x28,   0x64,    0xA0};


//Food cups                         2P     4P     6P     8P     10P     12P
uint16  code    FoodCups[6][6] = { {0x0100,0x0200,0x0300,0x0400,0x0500,0x0600},   //01 & 02 & 05
                                   {0x0100,0x0200,0x0300,0x0400,0x0400,0x0400},   //03
                                   {0x0100,0x0200,0x0300,0x0400,0x0450,0x0450},   //04
                                   {0x0100,0x0200,0x0300,0x0300,0x0300,0x0300},   //06 & 09 
                                   {0x0125,0x0250,0x0325,0x0500,0x0500,0x0500},   //08_P1
                                   {0x0050,0x0100,0x0150,0x0200,0x0200,0x0200} }; //08_P2
                                     
uint16  code   WaterCups[17][6]= { {0x0200,0x0400,0x0600,0x0800,0x1000,0x1200},   //01_T1 & 01_T3 & 01_T4
                                   {0x0180,0x0350,0x0530,0x0700,0x0880,0x1050},   //01_T2
                                   {0x0350,0x0400,0x0600,0x0800,0x1000,0x1200},   //02_T1 & 02_T3
                                   {0x0330,0x0350,0x0525,0x0700,0x0880,0x1050},   //02_T2
                                   {0x0380,0x0450,0x0680,0x0900,0x1130,0x1350},   //02_T4
                                   {0x0500,0x1000,0x1500,0x2000,0x2000,0x2000},   //03_T1
                                   {0x0280,0x0560,0x0840,0x1120,0x1120,0x1120},   //03_T2
                                   {0x0560,0x1120,0x1680,0x2240,0x2240,0x2240},   //03_T4
                                   {0x0400,0x0600,0x0800,0x1000,0x1050,0x1050},   //04_T1 & 04_T3
                                   {0x0300,0x0400,0x0600,0x0800,0x0900,0x0900},   //04_T2
                                   {0x0300,0x0450,0x0600,0x0750,0x1000,0x1000},   //05_T1 & 05_T2
                                   {0x0430,0x0580,0x0750,0x0900,0x1200,0x1200},   //05_T4
                                   {0x0700,0x1200,0x1800,0x1800,0x1800,0x1800},   //06_P1
                                   {0x0700,0x0900,0x1300,0x1300,0x1300,0x1300},   //06_P2
                                   {0x0500,0x1000,0x1500,0x1800,0x1800,0x1800},   //08
                                   {0x0800,0x1200,0x1800,0x1800,0x1800,0x1800},   //09_P1
                                   {0x0600,0x1000,0x1400,0x1400,0x1400,0x1400} }; //09_P2
/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/



/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      人机界面处理                                    					   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	UIProc(void)
{
	switch(m_u8SysID)
	{
		case	SYS_ID_RESET:
			ResetProc();
			break;
				
        case    SYS_ID_SETUP:
		    SetupProc();
            break;

		case 	SYS_ID_RUN:
			RunProc();
			break;

        case    SYS_ID_WARM:
		    WarmProc();
			break;

        case    SYS_ID_POWRDOWN:
            PowerDownProc();
            break;
            
        case    SYS_ID_ERROR:
		    ErrorProc();
			break;
			
        case	SYS_ID_CHECK:
			SelfCheckProc();
			break;
			
		default:
			break;	
	}
}


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      	复位状态处理                                     				   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	ResetProc(void)
{
 	if(bSys_1000MS)
	{
	    m_u8ResetCnt++;
        if(m_u8ResetCnt>1)
        {
            if(Flash.ROM[SSP_MUSIC_EN]==0xA5)
	        {
		        bKeyMusic_EN = 0;       //关闭按键声音
	        }
	        else
	        {
		        bKeyMusic_EN = 1;		
	        }
            EnterDefault();   
        }
	}									
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      设置状态处理                                     					   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	SetupProc(void)
{
	uint16 u16Key;
	u16Key = GetKey();

    if(u16Key>0) 
    {   
        if(m_u8DispDelayCnt==0)
        {
            u16Key = 0;             //从黑屏唤醒时按键不响应，只刷新显示
            FuncSetDisp(m_u8SetStep);
        }
        LedOn(BIT_LED_BKL);         //按下任意按键打开背光
        m_u8DispDelayCnt = 60;
    }

    if(TstBit(u16Key,BIT_KEY_CANCEL))
    {
        //返回上一个设置
        if(IsNewKey())
        {
            switch(m_u8SetStep)
            {
                case    SET_PHASE_MENU:
                    if(m_u8DIYMode>0)
                    {
                        m_u8SetStep = SET_PHASE_DIY;
                        FuncSetDisp(m_u8SetStep); 
                    }
                    else
                    {
                        EnterSetup();
                    }
                    break;
                      
                case    SET_PHASE_DIY:
                    EnterSetup();
                    break;
                    
                case    SET_PHASE_TEXTURE:
  //                  m_u8Feel = 1;   //口感默认值
                    m_u8SetStep = SET_PHASE_MENU;
                    FuncSetDisp(m_u8SetStep); 
                    break;

                case    SET_PHASE_PEOPLE:
 //                   m_u8People = 2; //人数默认值
                    if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_BROWNRICE_LENTILS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                    {
                        m_u8SetStep = SET_PHASE_MENU;
                    }
                    else
                    {
                        m_u8SetStep = SET_PHASE_TEXTURE;
                    }
                    FuncSetDisp(m_u8SetStep);
                    break;
                    
                case    SET_PHASE_PUTBASKET:
                case    SET_PHASE_FOODCUPS:
                    m_u8SetStep = SET_PHASE_PEOPLE;
                    FuncSetDisp(m_u8SetStep);
                    break;

                case    SET_PHASE_WATERCUPS:   
                    if(m_u8CookMode==FUNC_ID_BROWNRICE_LENTILS)
                    {
                        m_u8Foodtype = 1;    //放入第一种食物
                    }
                    else
                    {
                        m_u8Foodtype = 0;    //不显示食物类型
                    }
                    m_u8SetStep = SET_PHASE_FOODCUPS;
                    FuncSetDisp(m_u8SetStep);
                    break;

                case    SET_PHASE_DELAY:
                    if(m_u8CookMode>9)
                    {
                        if(m_u8CookMode==FUNC_ID_RISOTTO)
                        {
                            m_u8SetStep = SET_PHASE_MENU;
                        }
                        else
                        {
                            if(bTimeAdjSt)              //判断是否修改了默认时间
                            {
                                bTempDisp_EN = 1;		//允许调整温度
                                bTimeDisp_EN = 1;		//允许调整烹饪时间
                                bBookTime_EN = 0;	    //不允许调整预约时间
                                bCalBookTime_EN = 0;
                                m_u8SetStep = SET_PHASE_TIME;
                            }
                            else
                            {
                                if(m_u8CookMode==FUNC_ID_DIY)                         
                                {
                                    SetDIYModeByDIY(m_u8DIYMode);
                                    m_u8SetStep = SET_PHASE_DIYDEFAULT;
                                }
                                else
                                {
                                    m_u8SetStep = SET_PHASE_DEFAULT;
                                }
                            }
                        }
                    }
                    else
                    {
                        m_u8SetStep = SET_PHASE_WATERCUPS;
                    }
                    FuncSetDisp(m_u8SetStep);
                    break;

                case    SET_PHASE_TIME:
                    if(bBookTime_EN)
                    {
                        bBookTime_EN = 0;           //清预约
                        bCalBookTime_EN = 0;
                        tmBookTime[1] = 0;
                        tmBookTime[2] = 0;
                        m_u8SetStep = SET_PHASE_DELAY;
                    }
                    else
                    {
                        if(u8CookTemp>0)
                        {
                           m_u8SetStep = SET_PHASE_TEMPERATURE;
                        }
                        else
                        {
                            SetDefCookTime(m_u8CookMode);
                            tmCookTime[0] = 59;
                            tmCookTime[1] = tmDefCookTime[1];	//默认烹饪时间
                            tmCookTime[2] = tmDefCookTime[2];
                            bTimeAdjSt = 0;
                            bTempDisp_EN = 0;
                            bTimeDisp_EN = 0;    
                            m_u8SetStep = SET_PHASE_DEFAULT;
                        }
                    }
                    FuncSetDisp(m_u8SetStep);
                    break;
                    
                case    SET_PHASE_TEMPERATURE:
                    if(m_u8CookMode==FUNC_ID_DIY)                         
                    {
                        SetDIYModeByDIY(m_u8DIYMode);
                        m_u8SetStep = SET_PHASE_DIYDEFAULT;
                    }
                    else
                    {
                        SetDefCookTime(m_u8CookMode);
                        SetDefCookTemp(m_u8CookMode);
                        tmCookTime[0] = 59;
                        tmCookTime[1] = tmDefCookTime[1];	//默认烹饪时间
                        tmCookTime[2] = tmDefCookTime[2];
                        u8CookTemp = DefCookTemp;			//默认烹饪温度
                        m_u8SetStep = SET_PHASE_DEFAULT;
                    }
                    bTimeAdjSt = 0;
                    bTempDisp_EN = 0;
                    bTimeDisp_EN = 0;  
                    FuncSetDisp(m_u8SetStep);
                    break;

                case    SET_PHASE_DEFAULT:
                    if(m_u8CookMode==FUNC_ID_REHEAT)
					{
						EnterSetup();
					}
					else
					{
						m_u8SetStep = SET_PHASE_MENU;
					}               
                    FuncSetDisp(m_u8SetStep);
                    break;
                    
                case    SET_PHASE_AGE:
                    m_u8SetStep = SET_PHASE_MENU;
                    FuncSetDisp(m_u8SetStep);
                    break;

                case    SET_PHASE_DIYDEFAULT:
                    if(m_u8CookMode==FUNC_ID_DIY)
                    {
                        m_u8SetStep = SET_PHASE_DIY;
                    }
                    FuncSetDisp(m_u8SetStep); 
                    break;
                default:
                    break;
            }
        }

        if((u16Key==VAL_KEY_CANCEL)&&(g_u16KeyRpyCnt==15))	//长按返回默认状态
        {
            EnterDefault();
            PlaySound(MusicPoweron);
        }
    }

    if(TstBit(u16Key,BIT_KEY_STOP))
    {
        if(IsNewKey()){}
 
        if((u16Key==VAL_KEY_STOP)&&(g_u16KeyRpyCnt==20))	//长按3S进入OFF模式
        {
            EnterPowerDown();
            PlaySound(MusicPoweron);
            return;
        }
    }

    if(TstBit(u16Key,BIT_KEY_KEEPWARM))
    {
        if(IsNewKey())
        {
            EnterWarm();
            return;
        }
    }

    if(TstBit(u16Key,BIT_KEY_REHEAT))
    {
        if(IsNewKey())
        {    
            m_u8SetStep = SET_PHASE_DEFAULT;
            m_u8CookMode = FUNC_ID_REHEAT;
            SetDefCookTime(m_u8CookMode);
            SetDefCookTemp(m_u8CookMode);
            tmCookTime[0] = 59;
            tmCookTime[1] = tmDefCookTime[1];	//默认烹饪时间
            tmCookTime[2] = tmDefCookTime[2];
            u8CookTemp = DefCookTemp;			//默认烹饪温度
            m_u8Feel = 0;
            m_u8People = 0;
            FuncSetDisp(m_u8SetStep);
        }
    }

    if(TstBit(u16Key,BIT_KEY_DIY))
    {
        if(IsNewKey())
        {
            if(m_u8SetStep==SET_PHASE_MENU)
            {
                m_u8DIYMode = Flash.ROM[SSP_LASTDIY];
                if((m_u8DIYMode==0)||(m_u8DIYMode>5))
                {
                    m_u8DIYMode = 1;
                }
                m_u8SetStep = SET_PHASE_DIY;
                SetDIYModeByDIY(m_u8DIYMode);
                FuncSetDisp(m_u8SetStep);
            }
        }
    }

	if(TstBit(u16Key,BIT_KEY_MENU))
	{
		if(IsNewKey())
		{
		    if(m_u8SetStep==SET_PHASE_DIYDEFAULT)
            {
                m_u8SetStep = SET_PHASE_MENU;
                m_u8CookMode = m_u8DIYCookmode;
                tmCookTime[0] = 59;
                tmCookTime[1] = 0;
                tmCookTime[2] = 0;
                u8CookTemp = 0;
                if(m_u8CookMode>10)
                {
                    SetDefCookTime(m_u8CookMode);
                    SetDefCookTemp(m_u8CookMode);
                    tmCookTime[0] = 59;
                    tmCookTime[1] = tmDefCookTime[1];	//默认烹饪时间
                    tmCookTime[2] = tmDefCookTime[2];
                    u8CookTemp = DefCookTemp;			//默认烹饪温度
                }
                else
                {
                    m_u8Feel = m_u8DIYFeel;
                    m_u8People = m_u8DIYPeople;
                    m_u8Age = m_u8DIYAge;
                }
                FuncSetDisp(m_u8SetStep);
            }
            else
            {
                if(m_u8SetStep==SET_PHASE_MENU)
                {
                    if(m_u8CookMode<=9)
                    {
                        m_u8CookMode = FUNC_ID_RISOTTO;     //切换到经典菜单
                    }
                    else
                    {
                        m_u8CookMode = FUNC_ID_WHITERICE;   //切换到营养菜单
                    }
                    tmCookTime[0] = 59;
                    tmCookTime[1] = 0;	
                    tmCookTime[2] = 0;
                    u8CookTemp = 0;		
                    m_u8Feel = 0;
                    m_u8People = 0;
                    if(m_u8CookMode<=9)
                    {
                        m_u8Feel = 1;
                        m_u8People = 2;
                    }
	                else
                    {
                        SetDefCookTime(m_u8CookMode);
                        SetDefCookTemp(m_u8CookMode);
                        tmCookTime[0] = 59;
                        tmCookTime[1] = tmDefCookTime[1];	//默认烹饪时间
                        tmCookTime[2] = tmDefCookTime[2];
                        u8CookTemp = DefCookTemp;			//默认烹饪温度
                    }
                    FuncSetDisp(m_u8SetStep);
                }
            }
		}

        if(g_u16KeyRpyCnt==36)				//长按5S						
		{
			bKeyMusic_EN ^= 1;
			if(bKeyMusic_EN)
			{
				Flash.ROM[SSP_MUSIC_EN] = 0;	
			}
			else
			{
				Flash.ROM[SSP_MUSIC_EN] = 0xA5;
			}  
			SSP_Flag = SSP_ERASE_FLAG;
            EraseFlash(0x0000);                 //擦除类EEPROM
    	    SSP_Flag = SSP_WRITE_FLAG;
    	    WriteFlash(0x0000,(uint8*)&Flash);
			PlaySound(MusicPoweron);
		}
	}			

	if(TstBit(u16Key,BIT_KEY_PLUS))
	{
        if((IsNewKey())||(g_u16KeyRpyCnt>15))
        {
            if(m_u8SetStep==SET_PHASE_MENU)             
            {       
                if(m_u8CookMode>=21) m_u8CookMode = FUNC_ID_WHITERICE;
                else m_u8CookMode++;
                tmCookTime[0] = 59;
                tmCookTime[1] = 0;	
                tmCookTime[2] = 0;
                u8CookTemp = 0;		
                m_u8Feel = 0;
                m_u8People = 0;
                if(m_u8CookMode<=9)
                {
                    m_u8Feel = 1;
                    m_u8People = 2;
                }
                else
                {
                    SetDefCookTime(m_u8CookMode);
                    SetDefCookTemp(m_u8CookMode);
                    tmCookTime[0] = 59;
                    tmCookTime[1] = tmDefCookTime[1];	//默认烹饪时间
                    tmCookTime[2] = tmDefCookTime[2];
                    u8CookTemp = DefCookTemp;			//默认烹饪温度
                }
                FuncSetDisp(m_u8SetStep); 
            }

            if(m_u8SetStep==SET_PHASE_TEXTURE)
            {   
                if((m_u8CookMode==FUNC_ID_WHITERICE)||(m_u8CookMode==FUNC_ID_BROWNRICE))
                {
                    if(m_u8Feel<=1) m_u8Feel = 4;
                    else m_u8Feel--;
                }
                if(m_u8CookMode==FUNC_ID_QUINOA)
                {
                    if(m_u8Feel<=1) m_u8Feel = 3;
                    else m_u8Feel--;
                }
                if((m_u8CookMode==FUNC_ID_LENTILS)||(m_u8CookMode==FUNC_ID_BUCKWHEAT))
                {
                    switch(m_u8Feel)
                    {
                        case    1:
                            m_u8Feel = 4;
                            break;
                        case    2:
                            m_u8Feel = 1;
                            break;
                        case    4:
                            m_u8Feel = 2;
                            break;
                        default:
                            break;
                    }
                }
				if(m_u8CookMode==FUNC_ID_BEANS)
				{
					if(m_u8Feel==1) 
					{
						m_u8Feel = 3;
					}
					else
					{
						m_u8Feel = 1;
					}
				}
                FuncSetDisp(m_u8SetStep);   
            }

            if(m_u8SetStep==SET_PHASE_PEOPLE)
            {
                if((m_u8CookMode==FUNC_ID_WHITERICE)||(m_u8CookMode==FUNC_ID_BROWNRICE))
                {
                    if(m_u8People>=12)  m_u8People = 2;
                    else m_u8People += 2;
                }
                if((m_u8CookMode==FUNC_ID_BUCKWHEAT)||(m_u8CookMode==FUNC_ID_BROWNRICE_LENTILS))
                {
                    if(m_u8People>=8)  m_u8People = 2;
                    else m_u8People += 2;
                } 
                if((m_u8CookMode==FUNC_ID_QUINOA)||(m_u8CookMode==FUNC_ID_LENTILS))
                {
                    if(m_u8People>=10)  m_u8People = 2;
                    else m_u8People += 2;
                }
                if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                {
                    if(m_u8People>=6)  m_u8People = 2;
                    else m_u8People += 2;
                }
                if(m_u8CookMode==FUNC_ID_BROWNRICE_LENTILS)
                {
                    m_u8Foodtype = 1;
                }
                SetCupsWater(m_u8CookMode,m_u8Feel,m_u8People);
                FuncSetDisp(m_u8SetStep);  
            }

            if(m_u8SetStep==SET_PHASE_TIME)
            {
                if(bTimeDisp_EN==1)	
                {
                    //判断烹饪时间是否溢出 
    				if((tmCookTime[2]==tmMaxCookTime[2])&&(tmCookTime[1]==tmMaxCookTime[1]))
    				{
    					tmCookTime[2] =	tmMinCookTime[2];
    					tmCookTime[1] =	tmMinCookTime[1];
    			    }
    			    else
    				{
    				    if((m_u8CookMode==FUNC_ID_BROWNING)||(m_u8CookMode==FUNC_ID_PASTA)||(m_u8CookMode==FUNC_ID_DIY))			
    					{
    					    tmCookTime[1]++;
    					}
    					else 
    					{
    					    if(m_u8CookMode==FUNC_ID_YOGURT)
    						{
    						    tmCookTime[1] += 30;
    						}
    						else
    						{
    					        tmCookTime[1] += 5; 
    						}
    					}	
    				}
    				JudgeTimeValid();		//判断时间是否在合理范围内	 
			    }

                if(bBookTime_EN)			//预约时间是否允许调整
			    {			
    				//判断预约时间是否溢出
    				if((tmBookTime[2]==24)&&(tmBookTime[1]==0))
    				{
    					tmBookTime[2] =	tmMinBookTime[2];
    					tmBookTime[1] =	tmMinBookTime[1];
    				}
    				else
    				{   
    					tmBookTime[1] = (tmBookTime[1]/5)*5+5;
    				}
				    JudgeTimeValid();		//判断时间是否在合理范围内
			    }
                FuncSetDisp(m_u8SetStep);  
            }

            if(m_u8SetStep==SET_PHASE_TEMPERATURE)
            {
                if(bTempDisp_EN)			//温度是否允许调整
			    {
					//判断烹饪温度是否溢出
					if(u8CookTemp==MaxCookTemp)
					{
						u8CookTemp = MinCookTemp;
					}
					else
					{
					    if((m_u8CookMode==FUNC_ID_SLOWCOOK)||(m_u8CookMode==FUNC_ID_BREADRISING))
                        {
                            u8CookTemp += 10;
                        }
                        else
                        {
                            u8CookTemp += 5;
                        }
					}
					JudgeTimeValid();	//判断温度是否在合理范围内
				}			
                FuncSetDisp(m_u8SetStep);  
			}

            if(m_u8SetStep==SET_PHASE_DIY)
            {
                if(m_u8DIYMode>=5)  m_u8DIYMode = 1;
                else m_u8DIYMode++;
                SetDIYModeByDIY(m_u8DIYMode);
                FuncSetDisp(m_u8SetStep);  
            }

            if(m_u8SetStep==SET_PHASE_AGE)
            {
                if(m_u8Age>=4) m_u8Age = 1;
                else m_u8Age++;
                m_u8Feel = 0;
                m_u8People = 0;
                FuncSetDisp(m_u8SetStep);  
            }
        }
    }

	if(TstBit(u16Key,BIT_KEY_MINUS))
	{		
        if((IsNewKey())||(g_u16KeyRpyCnt>15))
        {
            if(m_u8SetStep==SET_PHASE_MENU)             
            {       
                if(m_u8CookMode<=1) m_u8CookMode = FUNC_ID_JAM;
                else m_u8CookMode--;
                tmCookTime[0] = 59;
                tmCookTime[1] = 0;	
                tmCookTime[2] = 0;
                u8CookTemp = 0;		
                m_u8Feel = 0;
                m_u8People = 0;
                if(m_u8CookMode<=9)
                {
                    m_u8Feel = 1;
                    m_u8People = 2;
                }
                else
                {
                    SetDefCookTime(m_u8CookMode);
                    SetDefCookTemp(m_u8CookMode);
                    tmCookTime[0] = 59;
                    tmCookTime[1] = tmDefCookTime[1];	//默认烹饪时间
                    tmCookTime[2] = tmDefCookTime[2];
                    u8CookTemp = DefCookTemp;			//默认烹饪温度
                }
                FuncSetDisp(m_u8SetStep); 
            }

            if(m_u8SetStep==SET_PHASE_TEXTURE)
            {          
                if((m_u8CookMode==FUNC_ID_WHITERICE)||(m_u8CookMode==FUNC_ID_BROWNRICE))
                {
                    if(m_u8Feel>=4) m_u8Feel = 1;
                    else m_u8Feel++;
                }
                if(m_u8CookMode==FUNC_ID_QUINOA)
                {
                    if(m_u8Feel>=3) m_u8Feel = 1;
                    else m_u8Feel++;
                }
                if((m_u8CookMode==FUNC_ID_LENTILS)||(m_u8CookMode==FUNC_ID_BUCKWHEAT))
                {
                    switch(m_u8Feel)
                    {
                        case    1:
                            m_u8Feel = 2;
                            break;
                        case    2:
                            m_u8Feel = 4;
                            break;
                        case    4:
                            m_u8Feel = 1;
                            break;
                        default:
                         break;
                    }
                }
				if(m_u8CookMode==FUNC_ID_BEANS)
				{
					if(m_u8Feel==1) 
					{
						m_u8Feel = 3;
					}
					else
					{
						m_u8Feel = 1;
					}
				}
                FuncSetDisp(m_u8SetStep); 
            }

            if(m_u8SetStep==SET_PHASE_PEOPLE)
            {
                if((m_u8CookMode==FUNC_ID_WHITERICE)||(m_u8CookMode==FUNC_ID_BROWNRICE))
                {
                    if(m_u8People<=2)  m_u8People = 12;
                    else m_u8People -= 2;
                }
                if((m_u8CookMode==FUNC_ID_BUCKWHEAT)||(m_u8CookMode==FUNC_ID_BROWNRICE_LENTILS))
                {
                    if(m_u8People<=2)  m_u8People = 8;
                    else m_u8People -= 2;
                }
                if((m_u8CookMode==FUNC_ID_QUINOA)||(m_u8CookMode==FUNC_ID_LENTILS))
                {
                    if(m_u8People<=2)  m_u8People = 10;
                    else m_u8People -= 2;
                }
                if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                {
                    if(m_u8People<=2)  m_u8People = 6;
                    else m_u8People -= 2;
                }
                if(m_u8CookMode==FUNC_ID_BROWNRICE_LENTILS)
                {
                    m_u8Foodtype = 1;
                }
                SetCupsWater(m_u8CookMode,m_u8Feel,m_u8People);
                FuncSetDisp(m_u8SetStep);  
            }
            
            if(m_u8SetStep==SET_PHASE_TIME)
            {
                if(bTimeDisp_EN==1)	
                {
  				    //判断烹饪时间是否溢出 
					if((tmCookTime[2]==tmMinCookTime[2])&&(tmCookTime[1]==tmMinCookTime[1]))
					{
						tmCookTime[2] =	tmMaxCookTime[2];
						tmCookTime[1] =	tmMaxCookTime[1];
					}
					else
					{
						if((m_u8CookMode==FUNC_ID_BROWNING)||(m_u8CookMode==FUNC_ID_PASTA)||(m_u8CookMode==FUNC_ID_DIY))				
						{
							if(tmCookTime[1]==0)
							{
								tmCookTime[1] = 59;
								tmCookTime[2]--;
							}
							else
							{
								tmCookTime[1]--;
							}
						}
						else 
						{
							if(m_u8CookMode==FUNC_ID_YOGURT)
							{
								if(tmCookTime[1]==0)
								{
									tmCookTime[1] = 30;
									tmCookTime[2]--;
								}
								else
								{
									tmCookTime[1] -= 30;
								}
							}
							else
							{
    						    if(tmCookTime[1]==0)
    						    {
    							    tmCookTime[1] = 55;
    							    tmCookTime[2]--;
    						    }
    						    else
    						    {
    							    tmCookTime[1] -= 5;
    						    }  
							}
						}	
					}
					JudgeTimeValid();		//判断时间是否在合理范围内
				}

                if(bBookTime_EN)			//预约时间是否允许调整
			    {			
    				//判断预约时间是否溢出
					if((tmBookTime[2]==tmMinBookTime[2])&&(tmBookTime[1]==tmMinBookTime[1]))
					{
						tmBookTime[2] =	24;
						tmBookTime[1] =	0;
					}
					else
					{					
						if(tmBookTime[1]==0)
						{
							tmBookTime[1] = 55;
							tmBookTime[2]--;
						}
						else
						{
							tmBookTime[1] -= 5;
						}	
					}
                    JudgeTimeValid();		//判断时间是否在合理范围内
				}
                FuncSetDisp(m_u8SetStep);  
            }

            if(m_u8SetStep==SET_PHASE_TEMPERATURE)
            {
                if(bTempDisp_EN)			//温度是否允许调整
			    {
					//判断烹饪温度是否溢出
					if(u8CookTemp==MinCookTemp)
					{
						u8CookTemp = MaxCookTemp;
					}
					else
					{
					    if((m_u8CookMode==FUNC_ID_SLOWCOOK)||(m_u8CookMode==FUNC_ID_BREADRISING))
                        {
                            u8CookTemp -= 10;    
                        }
                        else
                        {
                            u8CookTemp -= 5;
                        }
					}
					JudgeTimeValid();	//判断温度是否在合理范围内
				}			
                FuncSetDisp(m_u8SetStep);  
			}

            if(m_u8SetStep==SET_PHASE_DIY)
            {
                if(m_u8DIYMode<=1)  m_u8DIYMode = 5;
                else m_u8DIYMode--;
                SetDIYModeByDIY(m_u8DIYMode);
                FuncSetDisp(m_u8SetStep);  
            }

            if(m_u8SetStep==SET_PHASE_AGE)
            {
                if(m_u8Age<=1) m_u8Age = 4;
                else m_u8Age--;
                FuncSetDisp(m_u8SetStep);  
            }
        }
	}				

	if(TstBit(u16Key,BIT_KEY_TIMER))
	{
		if(IsNewKey())
		{
		    if((m_u8SetStep==SET_PHASE_DEFAULT)||(m_u8SetStep==SET_PHASE_DIYDEFAULT))
            {
                if((bTempDisp_EN==0)&&(bTimeDisp_EN==0))
                {
                    bTempDisp_EN = 1;		//允许调整温度
                    bTimeDisp_EN = 1;		//允许调整烹饪时间
                    bTimeAdjSt = 1;
                    bBookTime_EN = 0;	    //不允许调整预约时间
                    bCalBookTime_EN = 0;
                }
                if(u8CookTemp>0) m_u8SetStep = SET_PHASE_TEMPERATURE; //如果温度可调，先调温度
                else m_u8SetStep = SET_PHASE_TIME;
                FuncSetDisp(m_u8SetStep);
            }
            else
            {
                if(m_u8SetStep==SET_PHASE_TEMPERATURE)
                {
                    m_u8SetStep = SET_PHASE_TIME;
                    FuncSetDisp(m_u8SetStep);
                }
            }
        }
    }       

    if(TstBit(u16Key,BIT_KEY_DELAY))
	{
		if(IsNewKey())
		{
		    if(m_u8SetStep==SET_PHASE_DELAY)
		    {
        	    if(bBookTime_EN==0)
        		{
    				if(m_u8CookMode<=10)
    				{
    					tmBookTime[1] =	50;				            //默认预约时间
    					tmBookTime[2] =	0;
    					tmMinBookTime[1] = 50;
    					tmMinBookTime[2] = 0;			
    				}
    				else
    				{
    					tmBookTime[1] = tmCookTime[1];
    					tmBookTime[2] = tmCookTime[2];
    					tmMinBookTime[1] = tmCookTime[1];
    					tmMinBookTime[2] = tmCookTime[2];
    				}
    				bBookTime_EN = 1;
    				bCalBookTime_EN	= 1;
    				bTimeDisp_EN = 0;
    				bTempDisp_EN = 0;
                    m_u8SetStep = SET_PHASE_TIME;
                    FuncSetDisp(m_u8SetStep);
    			}	
            }
		}	
	}

	if(TstBit(u16Key,BIT_KEY_START))
	{
		if(IsNewKey())
		{
            switch(m_u8SetStep)
            {
                case    SET_PHASE_MENU:
                    if(m_u8CookMode<=9)//营养菜单
                    {
                        if(m_u8CookMode==FUNC_ID_BABYPORRIDGE)
                        {
                            if(m_u8DIYMode>0)       
                            {
                                if(m_u8DIYAge>0)
                                {
                                    m_u8Age = m_u8DIYAge;   //DIY模式下
                                }
                                else
                                {
                                    m_u8Age = 1;
                                }
                            }
                            else
                            {
                                if(m_u8LastAge>0)
                                {
                                    m_u8Age = m_u8LastAge;  //非DIY模式下
                                }
                                else
                                {
                                    m_u8Age = 1;
                                }
                            }
                            m_u8Feel = 0;
                            m_u8People = 0;
                            m_u8SetStep = SET_PHASE_AGE;
                        }
                        else
                        {
                            m_u8SetStep = SET_PHASE_TEXTURE;
                        }
                        if((m_u8CookMode==FUNC_ID_BROWNRICE_LENTILS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                        {
                            m_u8SetStep = SET_PHASE_PEOPLE;
                        }
                    }
                    else                        //经典菜单
                    {
                        if(m_u8CookMode==FUNC_ID_RISOTTO)
                        {
                            m_u8SetStep = SET_PHASE_DELAY;
                        }
                        else
                        {
                            m_u8SetStep = SET_PHASE_DEFAULT;
                        }
                    }
                    FuncSetDisp(m_u8SetStep);
                    break;
                    
                case    SET_PHASE_TEXTURE:
                    m_u8SetStep = SET_PHASE_PEOPLE;
                    FuncSetDisp(m_u8SetStep); 
                    break;
                    
                case    SET_PHASE_PEOPLE:
                    if((m_u8CookMode==FUNC_ID_BUCKWHEAT)||(m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                    {
                        m_u8SetStep = SET_PHASE_PUTBASKET;
                    }
                    else
                    {
                        m_u8SetStep = SET_PHASE_FOODCUPS;
                    }
                    if(m_u8CookMode==FUNC_ID_BROWNRICE_LENTILS)
                    {
                        m_u8Foodtype = 1;    //放入第一种食物
                    }
                    else
                    {
                        m_u8Foodtype = 0;    //不显示食物类型
                    }
                    SetCupsWater(m_u8CookMode,m_u8Feel,m_u8People);
                    FuncSetDisp(m_u8SetStep); 
                    break;

                case    SET_PHASE_PUTBASKET:
                    m_u8SetStep = SET_PHASE_FOODCUPS;
                    FuncSetDisp(m_u8SetStep); 
                    break;

                case    SET_PHASE_FOODCUPS:
                    if(m_u8CookMode==FUNC_ID_BROWNRICE_LENTILS)
                    {
                        if(m_u8Foodtype==1)
                        {
                            m_u8Foodtype = 2;    //放入第二种食物      
                            SetCupsWater(m_u8CookMode,m_u8Feel,m_u8People);
                        }
                        else
                        {   
                            m_u8Foodtype = 0;    //不显示食物类型
                            m_u8SetStep = SET_PHASE_WATERCUPS;
                        }
                    }
                    else
                    {
                        m_u8SetStep = SET_PHASE_WATERCUPS;
                    }
                    FuncSetDisp(m_u8SetStep);
                    break;

                case    SET_PHASE_WATERCUPS:          
                    if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                    {
                        m_u8SetStep = SET_PHASE_OVER;       //设置结束
                        EnterRun();
                    }
                    else
                    {
                        m_u8SetStep = SET_PHASE_DELAY;
                        FuncSetDisp(m_u8SetStep);
                    }
                    break;

                case    SET_PHASE_DELAY:
                    m_u8SetStep = SET_PHASE_OVER;       //设置结束
                    EnterRun();
                    break;

                case    SET_PHASE_TIME:
                    if(bBookTime_EN)                    //如果是调整的预约时间
                    {
                        m_u8SetStep = SET_PHASE_OVER;   //设置结束
                        EnterRun();                     //进入工作状态
                    }
                    else                                //如果是调整烹饪时间
                    {
                        if((m_u8CookMode==FUNC_ID_BROWNING)||(m_u8CookMode==FUNC_ID_BREADRISING)||(m_u8CookMode==FUNC_ID_YOGURT)
                        ||(m_u8CookMode==FUNC_ID_CREAMCHEESE)||(m_u8CookMode==FUNC_ID_PASTA))
                        {
                            m_u8SetStep = SET_PHASE_OVER;   //设置结束
                            EnterRun();                     //进入工作状态
                        }
                        else
                        {
                            m_u8SetStep = SET_PHASE_DELAY;  //进入预约判断
                            FuncSetDisp(m_u8SetStep);
                        }
                    }
                    break;

                case    SET_PHASE_TEMPERATURE:
                    m_u8SetStep = SET_PHASE_TIME;
                    FuncSetDisp(m_u8SetStep);
                    break;

                case    SET_PHASE_DIY:
                    SetDIYModeByDIY(m_u8DIYMode);
                    m_u8SetStep = SET_PHASE_DIYDEFAULT;
                    FuncSetDisp(m_u8SetStep);
                    break;
                    
                case    SET_PHASE_AGE:
                    m_u8SetStep = SET_PHASE_OVER;
                    EnterRun();
                    break;

                case    SET_PHASE_DEFAULT:
                    if((m_u8CookMode==FUNC_ID_BROWNING)||(m_u8CookMode==FUNC_ID_BREADRISING)||(m_u8CookMode==FUNC_ID_YOGURT)
                    ||(m_u8CookMode==FUNC_ID_CREAMCHEESE)||(m_u8CookMode==FUNC_ID_PASTA))
                    {
                        m_u8SetStep = SET_PHASE_OVER;   //设置结束
                        EnterRun();                     //进入工作状态
                    }
                    else
                    {
                        m_u8SetStep = SET_PHASE_DELAY;  //进入预约判断
                        FuncSetDisp(m_u8SetStep);
                    }
                    break;
                    
                case    SET_PHASE_DIYDEFAULT:
                    m_u8SetStep = SET_PHASE_DELAY;  //进入预约判断
                    FuncSetDisp(m_u8SetStep);
                    break;
                default:
                    break;
            }  
		}
	}
 
	//恢复工作状态显示
	if(bLEDComeback)
	{
		bLEDComeback = 0;
		bSysPwrOn =	1;
	}
	
	//恢复显示
	if(bSysPwrOn)
	{
        FuncSetDisp(m_u8SetStep);
	}

    //60S无操作,关闭背光显示
	if(bSys_1000MS)
	{
		if(m_u8DispDelayCnt>0)
		{
			m_u8DispDelayCnt--;
			if(m_u8DispDelayCnt==0)
			{
                ClrLedBlank(BIT_LED_ALL|BIT_LED_BKL);   //关闭指示灯
			}
		}	
	}
}

//判断时间是否合法
//输入参数,bTimeMode 0，对烹饪时间的判断；1对预约时间的判断
//说明：对于烹饪时间，如果时间大于最长的烹饪时间，那么设置为最小的烹饪时间
//  	对于预约时间，如果时间大于24:00，那么设置为00：00
static void JudgeTimeValid(void)
{
	if(bBookTime_EN)
	{
		if(tmBookTime[1]>=60)
		{
			tmBookTime[1] =	0;
			tmBookTime[2] += 1;
	  	}
		
		if(tmBookTime[2]>24)
		{
			tmBookTime[2] =	24;
			tmBookTime[1] =	0;
	 	}
		
		if((tmBookTime[2]==24)&&(tmBookTime[1]>0))
		{
			tmBookTime[2] =	24;
			tmBookTime[1] =	0;
		}
		
		if(tmBookTime[2]<tmMinBookTime[2])
		{
			tmBookTime[2] = tmMinBookTime[2];
            tmBookTime[1] =	tmMinBookTime[1];		
		}
		
		if(tmBookTime[2]==tmMinBookTime[2])
		{
			if(tmBookTime[1]<tmMinBookTime[1])
			{
				tmBookTime[2] = tmMinBookTime[2];
				tmBookTime[1] =	tmMinBookTime[1];	
			}
		}	
  	}

	if(bTimeDisp_EN)
	{
        if(m_u8CookMode==FUNC_ID_DIY)
        {
            if(u8CookTemp>100)
            {
                tmMaxCookTime[2] = 2;       //T>100,Maxcooktime==2h
                tmMaxCookTime[1] = 0;
            }
            else
            {
                tmMaxCookTime[2] = 9;       //T<=100,Maxcooktime==9h       
                tmMaxCookTime[1] = 0;
            }
        }
            
    	if(tmCookTime[1]>=60)
    	{
    		tmCookTime[1] =	0;
    		tmCookTime[2] += 1;
    	}

    	if(tmCookTime[2]>tmMaxCookTime[2])
    	{
    		tmCookTime[2] =	tmMaxCookTime[2];
    		tmCookTime[1] =	tmMaxCookTime[1];
    	}

    	if(tmCookTime[2]==tmMaxCookTime[2])
    	{
    		if(tmCookTime[1]>tmMaxCookTime[1])
    		{
    			tmCookTime[2] =	tmMaxCookTime[2];
    			tmCookTime[1] =	tmMaxCookTime[1];
    		}
    	}

    	if(tmCookTime[2]<tmMinCookTime[2])
    	{
    		tmCookTime[2] =	tmMinCookTime[2];
    		tmCookTime[1] =	tmMinCookTime[1];
    	}

    	if(tmCookTime[2]==tmMinCookTime[2])
    	{
    		if(tmCookTime[1]<tmMinCookTime[1])
    		{
    			tmCookTime[2] =	tmMinCookTime[2];
    			tmCookTime[1] =	tmMinCookTime[1];
    		}
    	}
    }
		
	if(bTempDisp_EN)
	{
        if(m_u8CookMode==FUNC_ID_DIY)
        {
            if((tmCookTime[2]>2)||((tmCookTime[2]==2)&&(tmCookTime[1]>0)))
            {
                MaxCookTemp = 100;          //cooktime>2h,Maxcooktemp==100          
            }
            else
            {
                MaxCookTemp = 160;          //cooktime<=2h,Maxcooktemp==160  
            }     
        }
        
		if(u8CookTemp>MaxCookTemp)
		{
			u8CookTemp = MaxCookTemp;
		}

		if(u8CookTemp<MinCookTemp)
		{
			u8CookTemp = MinCookTemp;
		}
	}
}		

//根据烹饪模式选择并显示烹饪时间
static	void	SetDefCookTime(uint8	CookMode)
{
	uint8 index = 0;

    switch(CookMode)
    {
        case    FUNC_ID_STEAM:
            index =	0;
            break;
        case    FUNC_ID_SLOWCOOK:
            index = 1;
            break;
        case    FUNC_ID_BROWNING:
            index = 2;
            break;
        case    FUNC_ID_PASTA:
            index = 3;
            break;
        case    FUNC_ID_BAKING:
            index = 4;
            break;
        case    FUNC_ID_SOUP:
            index = 5;
            break;
        case    FUNC_ID_BREADRISING:
            index = 6;
            break;
        case    FUNC_ID_DESSERT:
            index = 7;
            break;
        case    FUNC_ID_CREAMCHEESE:
            index = 8;
            break;
        case    FUNC_ID_YOGURT:
            index = 9;
            break;
        case    FUNC_ID_JAM:
            index = 10;
            break;
        case     FUNC_ID_DIY:
            index = 11;
            break;
        case    FUNC_ID_REHEAT:
            index = 12;
            break;
        default:
            break;
    }
    
	tmDefCookTime[2] =	BCD8toHEX(HIBYTE(DefCookTime_Table[index]));
	tmDefCookTime[1] =	BCD8toHEX(LOBYTE(DefCookTime_Table[index]));
	tmMaxCookTime[2] =	BCD8toHEX(HIBYTE(MaxCookTime_Table[index]));
	tmMaxCookTime[1] =	BCD8toHEX(LOBYTE(MaxCookTime_Table[index]));
	tmMinCookTime[2] =	BCD8toHEX(HIBYTE(MinCookTime_Table[index]));
	tmMinCookTime[1] =	BCD8toHEX(LOBYTE(MinCookTime_Table[index]));
}

//根据烹饪模式和食物种类选择烹饪温度
static  void    SetDefCookTemp(uint8 CookMode)
{
    uint8 index;

    switch(CookMode)
    {
        case    FUNC_ID_SLOWCOOK:
            index = 0;
            break;
        case    FUNC_ID_DESSERT:
            index = 1;
            break;
        case    FUNC_ID_BROWNING:
            index = 2;
            break;
        case    FUNC_ID_BAKING:
            index = 3;
            break;
        case    FUNC_ID_BREADRISING:
            index = 4;
            break;
        case    FUNC_ID_REHEAT:
            index = 5;
            break;
        case    FUNC_ID_DIY:
            index = 6;
            break;        
        default:
            index = 0xff;
            break;
    }

    if(index!=0xff)
    {
        DefCookTemp = DefCookTemp_Table[index];
        MaxCookTemp = MaxCookTemp_Table[index];
    	MinCookTemp = MinCookTemp_Table[index];	
    }
    else
    {
        DefCookTemp = 0;
        MaxCookTemp = 0;
    	MinCookTemp = 0;
    }
}

//DIY键设置DIYMode
static  void    SetDIYModeByDIY(uint8 diymode)
{
    uint8 i;

    if((diymode>0)&&(diymode<6))
    {
        if(diymode==1) i = 0;
        if(diymode==2) i = 7;
        if(diymode==3) i = 14;
        if(diymode==4) i = 21;
        if(diymode==5) i = 28;    
        m_u8Feel = 0;
        m_u8People = 0;
        m_u8Foodtype = 0;
        m_u8Watertype = 0;
        m_u8Age = 0;
    	bTimeDisp_EN = 0;           //不允许调整时间  
    	bTempDisp_EN = 0;			//不允许调整温度
    	bBookTime_EN = 0;			//取消预约
    	bCalBookTime_EN	= 0;		//取消预约计算
    	tmBookTime[0] =	59;	 		//清预约时间
    	tmBookTime[1] =	0;
    	tmBookTime[2] =	0;
    	tmCookTime[0] =	59;			//清烹饪时间
    	tmCookTime[1] =	0;
    	tmCookTime[2] =	0;	
        u8CookTemp = 0;				//清烹饪温度
        m_u8CookMode = FUNC_ID_DIY;
        SetDefCookTime(m_u8CookMode);
        SetDefCookTemp(m_u8CookMode);
        tmCookTime[1] =  Flash.ROM[i+SSP_DIY1_TM1];
        tmCookTime[2] =  Flash.ROM[i+SSP_DIY1_TM2];
        u8CookTemp = Flash.ROM[i+SSP_DIY1_TMP];
        m_u8DIYCookmode = Flash.ROM[i+SSP_DIY1_COOKMODE];
        m_u8DIYFeel = Flash.ROM[i+SSP_DIY1_TEXTURE];
        m_u8DIYPeople = Flash.ROM[i+SSP_DIY1_PEOPLE];
        m_u8DIYAge = Flash.ROM[i+SSP_DIY1_AGE];
    }
}

//设置米量和水量
static  void    SetCupsWater(uint8 cookmode,uint8 texture,uint8 people)
{
    uint8 i=0,j=0;

    m_u16Cups = 0;
    m_u16Water = 0;
    
    //Set the grains quantity
    if((cookmode==1)||(cookmode==2)||(cookmode==5)) i = 0;
    if(cookmode==3) i = 1;
    if(cookmode==4) i = 2;
    if((cookmode==6)||(cookmode==9)) i = 3;
    if((cookmode==8)&&(m_u8Foodtype==1)) i = 4;
    if((cookmode==8)&&(m_u8Foodtype==2)) i = 5;
    if((people>=2)&&(people<=12))
    {
        j = (people>>1)-1;
    }
    m_u16Cups = FoodCups[i][j];

    //set water quantity
    if(((cookmode==1)&&(texture==1))||((cookmode==1)&&(texture==3))||((cookmode==1)&&(texture==4))) i = 0;
    if((cookmode==1)&&(texture==2)) i = 1;
    if(((cookmode==2)&&(texture==1))||((cookmode==2)&&(texture==3))) i = 2;
    if((cookmode==2)&&(texture==2)) i = 3;
    if((cookmode==2)&&(texture==4)) i = 4;
    if((cookmode==3)&&(texture==1)) i = 5;
    if((cookmode==3)&&(texture==2)) i = 6;
    if((cookmode==3)&&(texture==4)) i = 7;
    if(((cookmode==4)&&(texture==1))||((cookmode==4)&&(texture==3))) i = 8;
    if((cookmode==4)&&(texture==2)) i = 9;
    if(((cookmode==5)&&(texture==1))||((cookmode==5)&&(texture==2))) i = 10;
    if((cookmode==5)&&(texture==4)) i = 11;
    if((cookmode==6)&&(m_u8Watertype==0)) i = 12;
    if((cookmode==6)&&(m_u8Watertype==1)) i = 13;
    if(cookmode==8) i = 14;
    if((cookmode==9)&&(m_u8Watertype==0)) i = 15;
    if((cookmode==9)&&(m_u8Watertype==1)) i = 16;
    m_u16Water = WaterCups[i][j];
}

//吸水、糊化、倒计时参数设置
static	void	LoadVarParam(uint8 cookMode, uint8 texture,uint8 people)
{
	uint8	i=0,j=0;
    
	g_u16VarParam[SOAK_TIME_RISE_S1] = 0;		//吸水升温时间1
	g_u16VarParam[SOAK_TIME_KEEP_S1] = 0;		//吸水维持时间1
	g_u16VarParam[SOAK_TEMP_KEEP_S1] = 0;		//吸水维持温度1  
	g_u16VarParam[SOAK_TIME_RISE_S2] = 0;		//吸水升温时间2(BrownRice_Nutritious)
	g_u16VarParam[SOAK_TIME_KEEP_S2] = 0;       //吸水维持时间2
    g_u16VarParam[SOAK_TEMP_KEEP_S2] = 0;       //吸水维持温度2
    g_u16VarParam[MASH_TIME] = 0;               //糊化时间  
    g_u16VarParam[COUNTDOWN_TIME] = 0;          //倒计时时间  

    if((people>=2)&&(people<=12))
    {
        j = (people>>1)-1;
    }
    
    //吸水升温时间 / 吸水维持时间 / 吸水维持温度  
    if((cookMode==1)&&(texture==1)) i = 0;
    if((cookMode==1)&&(texture==4)) i = 1;
    if(((cookMode==2)&&(texture==1))||((cookMode==2)&&(texture==3))||((cookMode==2)&&(texture==4))) i = 2;
    if(((cookMode==3)&&(texture==1))||((cookMode==3)&&(texture==4))) i = 4;
	if((cookMode==5)&&(texture==1)) i = 5;
	if((cookMode==5)&&(texture==2)) i = 6;
    if((cookMode==5)&&(texture==4)) i = 7;
    if(cookMode==8) i = 8;
    g_u16VarParam[SOAK_TIME_RISE_S1] = SoakTimeRiseTab[i][j];
    g_u16VarParam[SOAK_TIME_KEEP_S1] = SoakTimeKeepTab[i][j];
    g_u16VarParam[SOAK_TEMP_KEEP_S1] = SoakTempKeepTab[i][j];
    if((cookMode==2)&&(texture==1))     //Brownrice_Nutritious_S2
    {
        i = 3;
        g_u16VarParam[SOAK_TIME_RISE_S2] = SoakTimeRiseTab[i][j];
        g_u16VarParam[SOAK_TIME_KEEP_S2] = SoakTimeKeepTab[i][j];
        g_u16VarParam[SOAK_TEMP_KEEP_S2] = SoakTempKeepTab[i][j];
    }
    
    //糊化时间
    if(cookMode==1) i = 0;
    if(cookMode==2) i = 1;
    if(cookMode==3) i = 2;
    g_u16VarParam[MASH_TIME] = MashTimeTab[i][j];

    //倒计时时间 
    if((cookMode==1)&&(texture==1)) i = 0;
    if((cookMode==1)&&(texture==2)) i = 1;    
    if((cookMode==1)&&(texture==3)) i = 2;
    if((cookMode==1)&&(texture==4)) i = 3;
    if((cookMode==2)&&(texture==1)) i = 4;
    if((cookMode==2)&&(texture==2)) i = 5;
    if((cookMode==2)&&(texture==3)) i = 6;
    if((cookMode==2)&&(texture==4)) i = 7;
    if((cookMode==3)&&(texture==1)) i = 8;
    if((cookMode==3)&&(texture==2)) i = 9;
    if((cookMode==3)&&(texture==4)) i = 10;
    if((cookMode==4)&&(texture==1)) i = 11;
    if((cookMode==4)&&(texture==2)) i = 12;
    if((cookMode==4)&&(texture==3)) i = 13;
    if((cookMode==5)&&(texture==1)) i = 14;
    if((cookMode==5)&&(texture==2)) i = 15;
    if((cookMode==5)&&(texture==4)) i = 16;
    if((cookMode==6)&&(bStageChgReq==0)) i = 17;    //Beans_stage1
    if((cookMode==6)&&(bStageChgReq==1)) 			//Beans_stage2
	{
		if(texture==1)								//Beans_stage2_Nutritious
		{
			i = 14;
		}
		else 										//Beans_stage2_Normal
		{
			i = 18;
		}
	}
    if(cookMode==8) i = 20;
    if((cookMode==9)&&(bStageChgReq==0)) i = 21;
    if((cookMode==9)&&(bStageChgReq==1)) i = 22;
    g_u16VarParam[COUNTDOWN_TIME] = CountDownTimeTab[i][j];
    if(cookMode==7)
    {
        i = 19;
        if(m_u8Age>0)
        {
            j = m_u8Age - 1;
        }
        g_u16VarParam[COUNTDOWN_TIME] = CountDownTimeTab[i][j];
    }
}

/*-----------------------------------------------------------------------------*
*  Function Description:                                                       *
*      功能选择状态下的显示                                    			       * 
*  Parameters:                                                                 *
*      Step                                                                    *
*  Return                                                                      *
*      None                                                                    *
*----------------------------------------------------------------------------*/
static  void    FuncSetDisp(uint8 Step)
{
    ClrLedBlank(BIT_LED_ALL);
    SetLedBlank(BIT_LED_START);     
    SetLedBlank(BIT_LED_STOP);
    u8BlankCnt = 88;
    
    switch(Step)
    {
        case    SET_PHASE_MENU:                 //菜单设置阶段
            if(m_u8CookMode<=9)
            {   
                LCDDispZone1(m_u8CookMode,0,ZONE1_STAT_NUTRIMENU_SELECT);   //显示营养菜单图标
            }
            else
            {
                LCDDispZone1(m_u8CookMode,0,ZONE1_STAT_CLASSICMENU_SELECT); //显示经典菜单图标
            }
            LCDDispZone2(0, ZONE2_STAT_ALLOFF);
            LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8DIYMode, 0, 0, ZONE4_STAT_ALLOFF);
            PlayGif(NULL, 0, 1);
            break;
        case    SET_PHASE_TEXTURE:              //口感设置阶段
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_SELECT);
            LCDDispZone2(0, ZONE2_STAT_ALLOFF);
            LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8DIYMode, 0, 0, ZONE4_STAT_ALLOFF);
            PlayGif(NULL, 0, 1);
            break;
        case    SET_PHASE_PEOPLE:               //人数设置阶段
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_SELECT);
            LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8DIYMode, 0, 0, ZONE4_STAT_ALLOFF);
            PlayGif(NULL, 0, 1);
            break;
        case    SET_PHASE_PUTBASKET:            //放入吸水篮子阶段
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8DIYMode, 0, 0, ZONE4_STAT_ALLOFF);
            PlayGif(GifPutbasket, 600, 0);
            break;
        case    SET_PHASE_FOODCUPS:             //放入食物阶段
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(m_u16Cups, m_u16Water, m_u8Foodtype, ZONE3_STAT_PUTFOOD);
            LCDDispZone4(m_u8DIYMode,0,0,ZONE4_STAT_ALLOFF);
            if((m_u8CookMode==FUNC_ID_BUCKWHEAT)||(m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
            {
                PlayGif(GifFoodBSK,600, 0);
            }
            else
            {
                PlayGif(GifFoodNBSK,600, 0);
            }
            break;
        case    SET_PHASE_WATERCUPS:            //放入水阶段
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(m_u16Cups, m_u16Water, m_u8Foodtype, ZONE3_STAT_PUTWATER);
            LCDDispZone4(m_u8DIYMode, 0, 0, ZONE4_STAT_ALLOFF);
            if((m_u8CookMode==FUNC_ID_BUCKWHEAT)||(m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
            {
                PlayGif(GifWaterBSK,600,0);
            }
            else
            {
                PlayGif(GifWaterNBSK,600,0);
            }
            break;
       case     SET_PHASE_DELAY:                //预约选择阶段(预约灯闪烁)
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(0, 0, m_u8Foodtype, ZONE3_STAT_ALLOFF);
            if(m_u8CookMode<=10)
            {
                LCDDispZone4(m_u8DIYMode, 0, 0, ZONE4_STAT_ALLOFF);   //健康菜单&Rissto不显示ZONE4
            }
            else
            {
                LCDDispZone4(m_u8DIYMode,u8CookTemp,tmCookTime,ZONE4_STAT_SET_DEFAULT);
            }
            if(m_u8CookMode<=9)    //健康菜单
            {
                if((m_u8CookMode==FUNC_ID_BUCKWHEAT)||(m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                {
                    PlayGif(GifCookerBSK,600,0);
                }
                else
                {
                    PlayGif(GifCookerNBSK,600,0);
                }
            }
            else                   //经典菜单
            {
                PlayGif(GifCookerNBSKNLID, 600, 0);
            }
            SetLedBlank(BIT_LED_DELAY);
            if(m_u8CookMode==FUNC_ID_REHEAT) SetLedBlank(BIT_LED_REHEAT);
            break;
        case    SET_PHASE_DEFAULT:              //默认温度和时间显示阶段(经典菜单&DIY)
            LCDDispZone1(m_u8CookMode, 0, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(0, ZONE2_STAT_ALLOFF);
            LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8DIYMode,u8CookTemp,tmCookTime,ZONE4_STAT_SET_DEFAULT);
            PlayGif(GifCookerNBSKNLID, 600, 0);
            if(m_u8CookMode==FUNC_ID_REHEAT) SetLedBlank(BIT_LED_REHEAT);
            break;
        case    SET_PHASE_TIME:                 //时间设置阶段
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(0, 0, m_u8Foodtype, ZONE3_STAT_ALLOFF);
            if(bBookTime_EN)
            {
                LCDDispZone4(m_u8DIYMode,u8CookTemp,tmBookTime,ZONE4_STAT_TIME_SELECT);
                LedOn(BIT_LED_DELAY);
            }
            else
            {
                if(m_u8CookMode<=10)
                {
                    LCDDispZone4(m_u8DIYMode,0,0,ZONE4_STAT_ALLOFF);    //健康菜单&Rissto不显示ZONE4
                }
                else
                {
                    LCDDispZone4(m_u8DIYMode,u8CookTemp,tmCookTime,ZONE4_STAT_TIME_SELECT);
                }
            }
            if(m_u8CookMode<=9)    //健康菜单
            {
                if((m_u8CookMode==FUNC_ID_BUCKWHEAT)||(m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                {
                    PlayGif(GifCookerBSK,600,0);
                }
                else
                {
                    PlayGif(GifCookerNBSK,600,0);
                }
            }
            else                   //经典菜单
            {
                PlayGif(GifCookerNBSKNLID, 600, 0);
            }
            if(m_u8CookMode==FUNC_ID_REHEAT) SetLedBlank(BIT_LED_REHEAT);
            break;
        case    SET_PHASE_TEMPERATURE:          //温度设置阶段     
            LCDDispZone1(m_u8CookMode, 0, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(0, ZONE2_STAT_ALLOFF);
            LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8DIYMode,u8CookTemp,tmCookTime,ZONE4_STAT_TEMP_SELECT);
            PlayGif(GifCookerNBSKNLID, 600, 0);
            if(m_u8CookMode==FUNC_ID_REHEAT) SetLedBlank(BIT_LED_REHEAT);
            break;
        case    SET_PHASE_DIY:                  //DIY设置阶段(选择DIY1-5中的一个)
            LCDDispZone1(0, 0, ZONE1_STAT_ALLOFF);
            LCDDispZone2(0, ZONE2_STAT_ALLOFF);
            LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8DIYMode,u8CookTemp,tmCookTime,ZONE4_STAT_DIY_SELECT);
            PlayGif(NULL, 0, 1);
            u8BlankCnt = 30;
            break;
        case    SET_PHASE_AGE:                  //Babyporridge 年龄设置阶段
            LCDDispZone1(m_u8CookMode, 0, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(0, ZONE2_STAT_ALLOFF);
            LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8Age|0x80, 0, 0, ZONE4_STAT_DIY_SELECT);
            PlayGif(GifCookerNBSK,600, 0);
            u8BlankCnt = 30;
            break;
        case    SET_PHASE_DIYDEFAULT:
            LCDDispZone1(m_u8CookMode, 0, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(0, ZONE2_STAT_ALLOFF);
            LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8DIYMode,u8CookTemp,tmCookTime,ZONE4_STAT_SET_DEFAULT);
            PlayGif(GifCookerNBSKNLID, 600, 0);
            break;
        default:
            break;
    }
}
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      工作状态处理                                     					   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	RunProc(void)
{
	uint16	u16Key;
	u16Key	=	GetKey();

    if(TstBit(u16Key,BIT_KEY_CANCEL))
    {
        //返回上一个设置
        if(IsNewKey())
        {
            switch(m_u8RunStep)
            {                 
                case    RUN_PHASE_DELAY:
                    m_u8RunStep = RUN_PHASE_WATERCUPS;
                    FuncRunDisp(m_u8RunStep);
                    break;

                case    RUN_PHASE_TIME:
                    if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                    {
                        bBookTime_EN = 0;           //清预约
                        bCalBookTime_EN = 0;
                        tmBookTime[1] = 0;
                        tmBookTime[2] = 0;
                        m_u8RunStep = RUN_PHASE_DELAY;
                        FuncRunDisp(m_u8RunStep);
                    }
                    break;

                default:
                    PlaySound(MusicOFF);
                    break;
            }
        }
        
        if((u16Key==VAL_KEY_CANCEL)&&(g_u16KeyRpyCnt==15))	//长按2S进入设置状态
        {
            EnterDefault(); 
            PlaySound(MusicCycleEnd);
            return;
        }
    }

    if(TstBit(u16Key,BIT_KEY_STOP))
    {
        if(IsNewKey())
        {
            PlaySound(MusicOFF);
        }
        if((u16Key==VAL_KEY_STOP)&&(g_u16KeyRpyCnt==15))	//长按2S进入设置状态
        {
            g_u16KeyRpyCnt = 0;
            EnterDefault(); 
            PlaySound(MusicCycleEnd);
            return;
        }
    }

    if(TstBit(u16Key,BIT_KEY_PLUS))
    {
        if((IsNewKey())||(g_u16KeyRpyCnt>15))
        {
            if(m_u8RunStep==RUN_PHASE_TIME)
            {
                if(bTimeDisp_EN==1)	
                {
                    //判断烹饪时间是否溢出 
    				if((tmCookTime[2]==tmMaxCookTime[2])&&(tmCookTime[1]==tmMaxCookTime[1]))
    				{
    					tmCookTime[2] =	tmMinCookTime[2];
    					tmCookTime[1] =	tmMinCookTime[1];
    			    }
    			    else
    				{
    				    if((m_u8CookMode==FUNC_ID_BROWNING)||(m_u8CookMode==FUNC_ID_PASTA)||(m_u8CookMode==FUNC_ID_DIY))			
    					{
    					    tmCookTime[1]++;
    					}
    					else 
    					{
    					    if(m_u8CookMode==FUNC_ID_YOGURT)
    						{
    						    tmCookTime[1] += 30;
    						}
    						else
    						{
    					        tmCookTime[1] += 5; 
    						}
    					}	
    				}
    				JudgeTimeValid();		//判断时间是否在合理范围内	
    				tmCookTime[0] = 59;
    				m_u8AutoRunCnt = 5;
			    }

                if(bBookTime_EN)			//预约时间是否允许调整
			    {			
    				//判断预约时间是否溢出
    				if((tmBookTime[2]==24)&&(tmBookTime[1]==0))
    				{
    					tmBookTime[2] =	tmMinBookTime[2];
    					tmBookTime[1] =	tmMinBookTime[1];
    				}
    				else
    				{   
    					tmBookTime[1] = (tmBookTime[1]/5)*5+5;
    				}
				    JudgeTimeValid();		//判断时间是否在合理范围内
			    }
                FuncRunDisp(m_u8RunStep);  
            }

            if(m_u8RunStep==RUN_PHASE_TEMPERATURE)
            {
                if(bTempDisp_EN)			//温度是否允许调整
			    {
					//判断烹饪温度是否溢出
					if(u8CookTemp==MaxCookTemp)
					{
						u8CookTemp = MinCookTemp;
					}
					else
					{
                        if((m_u8CookMode==FUNC_ID_SLOWCOOK)||(m_u8CookMode==FUNC_ID_BREADRISING))
                        {
                            u8CookTemp += 10;
                        }
                        else
                        {
                            u8CookTemp += 5;
                        }
					}
					JudgeTimeValid();	//判断温度是否在合理范围内
					m_u8AutoRunCnt = 5;
				}			
                FuncRunDisp(m_u8RunStep);  
			}

        }
    }

    if(TstBit(u16Key,BIT_KEY_MINUS))
    {
        if((IsNewKey())||(g_u16KeyRpyCnt>15))
        {
            if(m_u8RunStep==RUN_PHASE_TIME)
            {
                if(bTimeDisp_EN==1)	
                {
  				    //判断烹饪时间是否溢出 
					if((tmCookTime[2]==tmMinCookTime[2])&&(tmCookTime[1]==tmMinCookTime[1]))
					{
						tmCookTime[2] =	tmMaxCookTime[2];
						tmCookTime[1] =	tmMaxCookTime[1];
					}
					else
					{
						if((m_u8CookMode==FUNC_ID_BROWNING)||(m_u8CookMode==FUNC_ID_PASTA)||(m_u8CookMode==FUNC_ID_DIY))			
						{
							if(tmCookTime[1]==0)
							{
								tmCookTime[1] = 59;
								tmCookTime[2]--;
							}
							else
							{
								tmCookTime[1]--;
							}
						}
						else 
						{
							if(m_u8CookMode==FUNC_ID_YOGURT)
							{
								if(tmCookTime[1]==0)
								{
									tmCookTime[1] = 30;
									tmCookTime[2]--;
								}
								else
								{
									tmCookTime[1] -= 30;
								}
							}
							else
							{
    						    if(tmCookTime[1]==0)
    						    {
    							    tmCookTime[1] = 55;
    							    tmCookTime[2]--;
    						    }
    						    else
    						    {
    							    tmCookTime[1] -= 5;
    						    }  
							}
						}	
					}
					JudgeTimeValid();		//判断时间是否在合理范围内
					tmCookTime[0] = 59;
				    m_u8AutoRunCnt = 5;
				}

                if(bBookTime_EN)			//预约时间是否允许调整
			    {			
    				//判断预约时间是否溢出
					if((tmBookTime[2]==tmMinBookTime[2])&&(tmBookTime[1]==tmMinBookTime[1]))
					{
						tmBookTime[2] =	24;
						tmBookTime[1] =	0;
					}
					else
					{					
						if(tmBookTime[1]==0)
						{
							tmBookTime[1] = 55;
							tmBookTime[2]--;
						}
						else
						{
							tmBookTime[1] -= 5;
						}	
					}
                    JudgeTimeValid();		//判断时间是否在合理范围内
				}
                FuncRunDisp(m_u8RunStep);  
            }

            if(m_u8RunStep==RUN_PHASE_TEMPERATURE)
            {
                if(bTempDisp_EN)			//温度是否允许调整
			    {
					//判断烹饪温度是否溢出
					if(u8CookTemp==MinCookTemp)
					{
						u8CookTemp = MaxCookTemp;
					}
					else
					{
                        if((m_u8CookMode==FUNC_ID_SLOWCOOK)||(m_u8CookMode==FUNC_ID_BREADRISING))
                        {
                            u8CookTemp -= 10;    
                        }
                        else
                        {
                            u8CookTemp -= 5;
                        }
					}
					JudgeTimeValid();	//判断温度是否在合理范围内
					m_u8AutoRunCnt = 5;
				}			
                FuncRunDisp(m_u8RunStep);  
			}

        }
    }

    if(TstBit(u16Key,BIT_KEY_TIMER))
    {
		if(IsNewKey())
		{
		    if(m_u8RunStep==RUN_PHASE_START)
            {
                if(m_u8CookMode>10)                 //经典菜单&&DIY
                {
                    if(bBookTime_EN==0)             //无预约可调整烹饪温度和时间
                    {
                        if((bTempDisp_EN==0)&&(bTimeDisp_EN==0))
                        {
                            m_u8AutoRunCnt = 5;
                            bTempDisp_EN = 1;		//允许调整温度
                            bTimeDisp_EN = 1;		//允许调整烹饪时间
                            bBookTime_EN = 0;	    //不允许调整预约时间
                            bCalBookTime_EN = 0;
                        }
                        if(u8CookTemp>0) m_u8RunStep = RUN_PHASE_TEMPERATURE; //如果温度可调，先调温度
                        else m_u8RunStep = RUN_PHASE_TIME;
                        FuncRunDisp(m_u8RunStep);
                    }
                    else                            //有预约则显示设定的烹饪温度和时间
                    {
                        if(m_u8CookMode>10)
                        {
                            m_u8CurTimeDispCnt = 5;
                        }
                    }
                }
            }
            else
            {
                if(m_u8RunStep==RUN_PHASE_TEMPERATURE)
                {
                    m_u8AutoRunCnt = 5;
                    m_u8RunStep = RUN_PHASE_TIME;
                    FuncRunDisp(m_u8RunStep);
                }
            }
        }
    }

    if(TstBit(u16Key,BIT_KEY_DELAY))
    {
        if(IsNewKey())
        {
            if(m_u8RunStep==RUN_PHASE_DELAY)
		    {
        	    if(bBookTime_EN==0)
        		{
    				if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
    				{
    					tmBookTime[1] =	50;				            //默认预约时间
    					tmBookTime[2] =	0;
    					tmMinBookTime[1] = 50;
    					tmMinBookTime[2] = 0;			
    				    bBookTime_EN = 1;
    				    bCalBookTime_EN	= 1;
    				    bTimeDisp_EN = 0;
    				    bTempDisp_EN = 0;
                        m_u8RunStep = RUN_PHASE_TIME;
                        FuncRunDisp(m_u8RunStep);
                    }
    			}	
            }
        }
    }

	if(TstBit(u16Key,BIT_KEY_START))
	{
		if(IsNewKey())
		{   
		    switch(m_u8RunStep)
            {
                case    RUN_PHASE_START:
                    if((m_u8CookMode==FUNC_ID_PASTA)&&(bTempAch1st==1)&&(bTimeCountDown==0))   //pasta
                    {
                        bTimeCountDown = 1;   
                        if(bKeyMusic_EN==0)PlaySound(MusicKeyDown); 
                    }  
                    break;
                    
                case    RUN_PHASE_DRAINED:
                    m_u8RunStep = RUN_PHASE_DRAINEDFOOD;
                    FuncRunDisp(m_u8RunStep); 
                    break;
                    
                case    RUN_PHASE_DRAINEDFOOD:
                     m_u8Foodtype = 0; 
                     m_u8RunStep  = RUN_PHASE_WATERCUPS;   
                    if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                    {
                        m_u8Watertype = 1;
                    }
                    SetCupsWater(m_u8CookMode,m_u8Feel,m_u8People);
                    FuncRunDisp(m_u8RunStep); 
                    break;

                case    RUN_PHASE_FOODCUPS:
                    m_u8RunStep = RUN_PHASE_WATERCUPS;    
                    FuncRunDisp(m_u8RunStep); 
                    break;

                case    RUN_PHASE_WATERCUPS:
                    m_u8RunStep = RUN_PHASE_DELAY;
                    FuncRunDisp(m_u8RunStep); 
                    break;

                case    RUN_PHASE_DELAY:
                    m_u8RunStep = RUN_PHASE_START;       //不预约，直接开始工作
                    bStageChgReq = 1;                    //进入工作第二阶段
                    EnterRun();
                    break;

                case    RUN_PHASE_TIME:
                    if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                    {
                        bStageChgReq = 1;               //进入工作第二阶段
                    }
                    m_u8RunStep = RUN_PHASE_START;      //设置结束
                    EnterRun();                         //进入工作状态
                    break;

                case    RUN_PHASE_TEMPERATURE:
                    m_u8RunStep = RUN_PHASE_TIME;
                    m_u8AutoRunCnt = 5;
                    FuncRunDisp(m_u8RunStep);
                    break;

                default:
                    break;           
            }
        }       
	}

	#ifdef	DEBUG0
	if(TstBit(u16Key,BIT_KEY_DIY))
	{
		if(IsNewKey())
		{
		    bDebug_EN ^= 1;
            FuncRunDisp(m_u8RunStep);
		}
  	}
	#endif
		
	//恢复工作状态显示
	if(bLEDComeback)
	{
		bLEDComeback	=	0;
		bSysPwrOn		=	1;
	}
	
	if(bSys_1000MS)
	{		
        if(m_u8CurTimeDispCnt>0)
		{
			m_u8CurTimeDispCnt--;
            if(m_u8CurTimeDispCnt==0)
            {
                bSysPwrOn = 1;
            }
	  	}
        
		if(m_u8AutoRunCnt>0)
		{
			m_u8AutoRunCnt--;
			if(m_u8AutoRunCnt==0)
			{
				PlaySound(MusicPoweron);
				EnterRun(); 			
			}
		}
 	}
		
	if(bSysPwrOn)
	{
        FuncRunDisp(m_u8RunStep);
	}

	if(bBookTime_EN==0)
	{
		bCookBySctEN =	1;				//无预约,允许功能脚本运行
        bHeatPwrReq  =  1;              //无预约,打开加热总开关
        bSoakStage   =  0;
		bHeatStage   =  1;
        if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
        {
            if(bStageChgReq==0) 
            {
                bSoakStage = 1;
                bHeatStage = 0;
            }
        }
		if((tmCookTime[0]==59)&&(tmCookTime[1]==0)&&(tmCookTime[2]==0))
		{	
		    if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
            {
               if(m_u8RunStep!=RUN_PHASE_START) //处于滤水、放食物、放水阶段则停止运行
               {
                    ResetCurScript();             
    	            g_u8HiTemp	=	0;				
    	            bHeatTopReq	=	0;			   
    	            bHeatArdReq =   0;
                    bHeatStage = 0;
                    bSoakStage = 0;
               }
            }
		}
		else
		{
		    if(m_u8RunStep==RUN_PHASE_START)    //只在运行状态下执行
            {
    			//有定时，无预约
    	        if((m_u8CookMode==FUNC_ID_PASTA)&&(bTimeCountDown==0))
    			{
    		        if(bTempAch1st==0)
                    {
                       m_u8TimeDispCnt = 0;
                    }
                    else
                    {
                        if(bSys1S_CookTime)
                        {
                            bSys1S_CookTime = 0;
                            m_u8TimeDispCnt++;
                            if(m_u8TimeDispCnt>=20)
                            {
                                m_u8TimeDispCnt = 0;
                                PlaySound(MusicInputFood);
                            }
                        }
    				}
                }
                else
                {
                    if(bSys1S_CookTime)
        		    {
        		        bSys1S_CookTime	=	0;
        			    DecTime(tmCookTime);
        				if((tmCookTime[0]==59)&&(tmCookTime[1]==0)&&(tmCookTime[2]==0))
        				{
        				    if(((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))&&(bStageChgReq==0))
                            {
                                m_u8RunStep=RUN_PHASE_DRAINED;
                                ResetCurScript();           //必须停止脚本运行，不然会再赋值倒计时时间
                                PlaySound(MusicCountDown);
                            }
                            else
                            {
                                if((m_u8CookMode==FUNC_ID_BREADRISING)||(m_u8CookMode==FUNC_ID_YOGURT)||(m_u8CookMode==FUNC_ID_PASTA)
                                ||(m_u8CookMode==FUNC_ID_JAM)||(m_u8CookMode==FUNC_ID_CREAMCHEESE)||(m_u8CookMode==FUNC_ID_DESSERT)
                                ||((m_u8CookMode==FUNC_ID_DIY)&&(u8CookTemp<=100)))
                                {
                                    EnterDefault();
                                }
                                else
                                {
            					    EnterWarm();
                                    bWarm_Start = 1;
                                    bSys1S_CookTime = 1;        //立即刷新显示
                                }
                                PlaySound(MusicCookEnd);
                                return;
                            }
        				}
                        FuncRunDisp(m_u8RunStep);
        			}
                }
			}
		}
    }
	else
	{
		//有预约
        ResetCurScript();       //预约状态下不执行脚本     
        bHeatPwrReq = 0;        //关所有加热
        g_u8HiTemp = 0;	        //预约状态下不加热			
        bHeatTopReq	= 0;			   
        bHeatArdReq = 0;
        bHeatStage = 0;         
        bSoakStage = 0;

        if(m_u8RunStep==RUN_PHASE_START)    //只在运行状态下执行
        {
    		if(bCalBookTime_EN)
    		{
    			bCalBookTime_EN	=	0;			//清预约计算

    			tmIntCookTime[2]	=	tmBookTime[2];
    			tmIntCookTime[1]	=	tmBookTime[1];

    			//计算烹饪需要的时间
    			tmCalCookTime[2]	=	tmCookTime[2];
    			tmCalCookTime[1]	=	tmCookTime[1];
    			if((tmCookTime[0]==59)&&(tmCookTime[1]==0)&&(tmCookTime[2]==0))
    			{		
    			    tmCalCookTime[2] = tmMinBookTime[2];
    			    tmCalCookTime[1] = tmMinBookTime[1];
    			}
    			
    			//间隔时间是否满足烹饪所需时间
    			if(tmIntCookTime[2]<tmCalCookTime[2])
    			{
    				bBookTime_EN	=	0;			//间隔时间小于烹饪时间，取消预约
    				bLEDComeback	=	1;			//恢复正常烹饪显示
    		  	}
    			else	
    			{
    				if(tmIntCookTime[2]==tmCalCookTime[2])
    				{
    					if(tmIntCookTime[1]<=tmCalCookTime[1])
    					{
    						bBookTime_EN	=	0;			//间隔时间小于烹饪时间，取消预约
    						bLEDComeback	=	1;			//恢复正常烹饪显示
    					}
    				}
    		  	}
    		}
    		else
    		{
    			if((tmCalCookTime[2]==tmBookTime[2])&&(tmCalCookTime[1]==tmBookTime[1]))
    			{
    				//烹饪开始时间==当前时间
    				bBookTime_EN	=	0;				//预约标志取消
    				bLEDComeback	=	1;				//恢复到烹饪状态显示
    		  	}
    		}

    		if(bSys1S_BookTime)
    		{
    			bSys1S_BookTime	=	0;
    			DecTime(tmBookTime);
                FuncRunDisp(m_u8RunStep);
    		}
        }
	}
}

/*-----------------------------------------------------------------------------*
*  Function Description:                                                       *
*      功能运行状态下的显示                                    			       * 
*  Parameters:                                                                 *
*      Step                                                                    *
*  Return                                                                      *
*      None                                                                    *
*----------------------------------------------------------------------------*/
static  void    FuncRunDisp(uint8 Step)
{
    #ifdef	DEBUG0
    uint16 i,j;
    #endif
    
    ClrLedBlank(BIT_LED_ALL);
    SetLedBlank(BIT_LED_START);
    SetLedBlank(BIT_LED_STOP);

    switch(Step)
    {
        case    RUN_PHASE_START:                //开始工作
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(0, 0, m_u8Foodtype, ZONE3_STAT_ALLOFF);
            if(bBookTime_EN)
            {
                if(m_u8CurTimeDispCnt==0)
                {    
                    LCDDispZone4(m_u8DIYMode,u8CookTemp,tmBookTime,ZONE4_STAT_SET_CONFIRM);
                }
                else
                {
                    LCDDispZone4(m_u8DIYMode,u8CookTemp,tmCookTime,ZONE4_STAT_SET_DEFAULT); //在预约状态下显示烹饪温度和时间
                }
                LedOn(BIT_LED_DELAY);
            }
            else
            {
                //无预约
                if(m_u8CookMode<=9)            //健康菜单
                {
                    if(m_u8CookMode==FUNC_ID_BABYPORRIDGE)
                    {
                        if((tmCookTime[0]==59)&&(tmCookTime[1]==0)&&(tmCookTime[2]==0))
                        {
                            LCDDispZone4(m_u8Age|0x80, 0, 0, ZONE4_STAT_AUTO_CONFIRM);
                        }
                        else
                        {
                            LCDDispZone4(m_u8Age|0x80, 0, tmCookTime, ZONE4_STAT_SET_CONFIRM);//倒计时显示
                        }
                    }
                    else
                    {
                        if((tmCookTime[0]==59)&&(tmCookTime[1]==0)&&(tmCookTime[2]==0))
                        {
                            LCDDispZone4(m_u8DIYMode,0,0,ZONE4_STAT_AUTO_CONFIRM);
                        }
                        else
                        {
                            LCDDispZone4(m_u8DIYMode,0,tmCookTime,ZONE4_STAT_SET_CONFIRM);    //倒计时显示
                        }
                    }
                }
                else                           //经典菜单
                {
                    if((m_u8CookMode==FUNC_ID_RISOTTO)||((m_u8CookMode==FUNC_ID_PASTA)&&(bTimeCountDown==0)))
                    {
                        LCDDispZone4(m_u8DIYMode, 0, 0, ZONE4_STAT_AUTO_CONFIRM);
                    }
                    else
                    {
                        LCDDispZone4(m_u8DIYMode,u8CookTemp,tmCookTime,ZONE4_STAT_SET_CONFIRM);
                    }
                }
            }
            if(m_u8CookMode<=9)               
            {
                //营养菜单
                if((m_u8CookMode==FUNC_ID_BUCKWHEAT)||(m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                {
                    PlayGif(GifCookerBSK,600, 2);   //动态旋转方框
                }
                else
                {
                    PlayGif(GifCookerNBSK,600, 2);  //动态旋转方框
                }
            }
            else                   
            {
                //经典菜单
                if(m_u8CookMode==FUNC_ID_BROWNING)
                {
                    PlayGif(GifCookerNBSKNLID, 600, 2); //动态旋转方框
                }
                else
                {
                    PlayGif(GifCookerNBSK,600, 2);  //动态旋转方框
                    if((m_u8CookMode==FUNC_ID_PASTA)&&(bTempAch1st==1)&&(bTimeCountDown==0))   //pasta
                    {
                        PlayGif(GifCookerNBSKNLID, 600, 0); 
                    }
                }
            }
            ClrLedBlank(BIT_LED_ALL);
            LedOn(BIT_LED_START);
            if(m_u8CookMode==FUNC_ID_REHEAT) LedOn(BIT_LED_REHEAT);
            if(bBookTime_EN) LedOn(BIT_LED_DELAY);
            
            #ifdef	DEBUG0
            if(bDebug_EN)
            {
                LCDDispZone1(0, 0, ZONE1_STAT_ALLOFF);
                LCDDispZone2(0, ZONE2_STAT_ALLOFF);
                i =  GetScriptStage();
                i =  HEX8toBCD(i);
                j =  ADCGetData(AD_CH_TOP);
                j =  HEX8toBCD(j);
                LCDDispZone3(i, j, 0, ZONE3_STAT_DEBUG);
                i =  ADCGetData(AD_CH_BOT);
                tmDebugTime[1] =  g_u16RelayTime/60;
                tmDebugTime[2] =  g_u16RelayTime/3600;
                LCDDispZone4(0,i,tmDebugTime,ZONE4_STAT_DEBUG);
                ClrLedBlank(BIT_LED_ALL);
            }
            #endif
            
            break;
        case    RUN_PHASE_DRAINED:          //滤水(beans & chickpeas & beans+brownrice)
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(0, 0, m_u8Foodtype, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8DIYMode,0,0,ZONE4_STAT_ALLOFF);
            PlayGif(GifOutfood,600,0);       //取出食物 & 倒掉水
            break;
        case    RUN_PHASE_DRAINEDFOOD:     //放入滤水后的食物(beans & chickpeas & beans+brownrice)
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(0, 0, m_u8Foodtype, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8DIYMode,0,0,ZONE4_STAT_ALLOFF);
            if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))PlayGif(GifDrainedfoodBSK, 600,0);
            break;
        case    RUN_PHASE_FOODCUPS:         //再放入食物(beans+brownrice)
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(m_u16Cups, m_u16Water, m_u8Foodtype, ZONE3_STAT_PUTFOOD);
            LCDDispZone4(m_u8DIYMode,0,0,ZONE4_STAT_ALLOFF);
            PlayGif(GifFoodNBSK,600,0);
            break;
        case    RUN_PHASE_WATERCUPS:        //再放入水(beans & chickpeas & beans+brownrice)
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(m_u16Cups, m_u16Water, m_u8Foodtype, ZONE3_STAT_PUTWATER);
            LCDDispZone4(m_u8DIYMode,0,0,ZONE4_STAT_ALLOFF);
            if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))PlayGif(GifWaterBSK, 600, 0);        
            break;
        case    RUN_PHASE_TIME:             //运行状态下时间调整(Cooktime & Booktime)
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(0, 0, m_u8Foodtype, ZONE3_STAT_ALLOFF);
            if(bBookTime_EN)
            {
                LCDDispZone4(m_u8DIYMode,u8CookTemp,tmBookTime,ZONE4_STAT_TIME_SELECT);
                LedOn(BIT_LED_DELAY);
            }
            else
            {
                if(m_u8CookMode<=9) 
                {
                    //营养菜单
                    LCDDispZone4(m_u8DIYMode,0,0,ZONE4_STAT_ALLOFF);
                }
                else
                {
                    //经典菜单
                    LCDDispZone4(m_u8DIYMode,u8CookTemp,tmCookTime,ZONE4_STAT_TIME_SELECT);
                }
            }
            if(m_u8CookMode<=9)    
            {
                if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
                {
                    PlayGif(GifCookerBSK,600,0);
                }
            }
            else                   
            {
                if(m_u8CookMode==FUNC_ID_BROWNING)
                {
                    PlayGif(GifCookerNBSKNLID, 600, 2); 
                }
                else
                {
                    PlayGif(GifCookerNBSK,600, 2);  
                    if((m_u8CookMode==FUNC_ID_PASTA)&&(bTempAch1st==1)&&(bTimeCountDown==0))   //pasta
                    {
                        PlayGif(GifCookerNBSKNLID, 600, 0); 
                    }
                }
            }
            if(m_u8CookMode==FUNC_ID_REHEAT) LedOn(BIT_LED_REHEAT);
            break;
        case    RUN_PHASE_TEMPERATURE:
            LCDDispZone1(m_u8CookMode, 0, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(0, ZONE2_STAT_ALLOFF);
            LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
            LCDDispZone4(m_u8DIYMode,u8CookTemp,tmCookTime,ZONE4_STAT_TEMP_SELECT);
            if(m_u8CookMode==FUNC_ID_BROWNING)
            {
                PlayGif(GifCookerNBSKNLID, 600, 2); //动态旋转方框
            }
            else
                {
                    PlayGif(GifCookerNBSK,600, 2);  //动态旋转方框
                }
            if(m_u8CookMode==FUNC_ID_REHEAT) LedOn(BIT_LED_REHEAT);
            break;
        case    RUN_PHASE_DELAY:            //再预约(beans & chickpeas)
            LCDDispZone1(m_u8CookMode, m_u8Feel, ZONE1_STAT_TEXTURE_CONFIRM);
            LCDDispZone2(m_u8People, ZONE2_STAT_PEOPLENUM_CONFIRM);
            LCDDispZone3(0, 0, m_u8Foodtype, ZONE3_STAT_ALLOFF);
            LCDDispZone4(0, 0, 0, ZONE4_STAT_ALLOFF);   
            if((m_u8CookMode==FUNC_ID_BEANS)||(m_u8CookMode==FUNC_ID_CHICKPEAS))
            {
                PlayGif(GifCookerBSK,600,0);
            }
            SetLedBlank(BIT_LED_DELAY);
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      保温状态处理                                     					   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	WarmProc(void)
{
	uint16	u16Key;
	uint8	u8TopDegree =   ADCGetData(AD_CH_TOP);
    uint8   u8BotDegree =   ADCGetData(AD_CH_BOT);

	u16Key	=	GetKey();

    if(bWarm_Start)
    {
        bHeatPwrReq =   1;      
        
    	if(m_u8ChkCnt<15)
    	{
    		if(m_u8ChkCnt>=10)
    		{
    			g_u8HiTemp	=	65;
    			g_u8LoTemp	=	60;
    			g_u8OnTime_Set	=	0xf0;
    			bHeatTopReq	=	0;
    		}
    		else
    		{
    		    if(m_u8ChkCnt>=5)
                {
                    g_u8HiTemp	=	0;
    			    bHeatTopReq	=	1;
                    bHeatArdReq =   0;
                }
                else
                {
        			g_u8HiTemp	=	0;
        			bHeatTopReq	=	0;
                    bHeatArdReq =   1;
                }
    		}
    		if(bSys_1000MS)	m_u8ChkCnt++;	
    	}
    	else
    	{
    		g_u8HiTemp	=	70;
    		g_u8LoTemp	=	65;
    		g_u8OnTime_Set	=	10;
    		g_u8OffTime_Set	=	20;
            if(u8BotDegree<68)	bHeatArdReq	=	1;
    	    if(u8BotDegree>73)	bHeatArdReq	=	0;
    		if(u8TopDegree<80)	bHeatTopReq	=	1;
    		if(u8TopDegree>85)	bHeatTopReq	=	0;
    	}

        if(bSys1S_CookTime)
	    {
    		bSys1S_CookTime	=	0;
    		IncTime(tmCookTime);
    		if((tmCookTime[2]==23)&&(tmCookTime[1]==59)&&(tmCookTime[0]==59))
    		{
    			EnterSetup();
                return;
    		}
            LCDDispZone1(0, 0, ZONE1_STAT_ALLOFF);
            LCDDispZone2(0, ZONE2_STAT_ALLOFF);
            LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
            LCDDispZone4(0,0,tmCookTime,ZONE4_STAT_SET_CONFIRM);
            ClrLedBlank(BIT_LED_ALL);
            LedOn(BIT_LED_WARM);
            LedOn(BIT_LED_BKL);
	    }
    }
    else
    {
        if(u16Key>0) 
        {   
            if(m_u8DispDelayCnt==0)
            {
                u16Key = 0;             //从黑屏唤醒时按键不响应
            }
            LedOn(BIT_LED_BKL);         //按下任意按键打开背光
            SetLedBlank(BIT_LED_START);
            SetLedBlank(BIT_LED_STOP);
            SetLedBlank(BIT_LED_WARM);
            m_u8DispDelayCnt = 60;
        }

        //30S无操作,关闭背光显示
	    if(bSys_1000MS)
	    {
		    if(m_u8DispDelayCnt>0)
		    {
			    m_u8DispDelayCnt--;
			    if(m_u8DispDelayCnt==0)
			    {
                    ClrLedBlank(BIT_LED_ALL|BIT_LED_BKL);
			    }
		    }	
	    }
    }

    if(TstBit(u16Key,BIT_KEY_START))
    {
       if(IsNewKey()) 
       {
            if(bWarm_Start==0)
            {
                bWarm_Start = 1;
            }
       }
    }
        
	if(TstBit(u16Key,BIT_KEY_CANCEL))
	{
        if(IsNewKey())
        {
            if(bWarm_Start)
            {
                PlaySound(MusicOFF);
            }
            else
            {
                EnterDefault();
            }
        }
        
        if((u16Key==VAL_KEY_CANCEL)&&(g_u16KeyRpyCnt==15))	//长按2S进入设置状态
        {
            EnterDefault(); 
            PlaySound(MusicCycleEnd);
        }
	}

    if(TstBit(u16Key,BIT_KEY_STOP))
	{
        if(IsNewKey())
        {
            if(bWarm_Start)
            {
                PlaySound(MusicOFF);
            }
            else
            {
                EnterDefault();
            }
        }
        if((u16Key==VAL_KEY_STOP)&&(g_u16KeyRpyCnt==15))	//长按2S进入设置状态
        {
            g_u16KeyRpyCnt = 0;
            EnterDefault(); 
            PlaySound(MusicCycleEnd);
        }
	}
}
			
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      系统报警状态处理                                     				   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	ErrorProc(void)
{
	uint16	u16Key;
	u16Key	=	GetKey();

	//开始键处理
	if(TstBit(u16Key,BIT_KEY_STOP))
	{
		if(IsNewKey())
		{   
            EnterSetup();
            if(bKeyMusic_EN==0)
			{
				PlaySound(MusicOFF);
			}
            return;
		}
	}
	
	if(g_u8ErrorType==0)			
	{
		//报警解除
		m_u8SysID	=	m_u8PrevSysID;
		bLEDComeback	=	1;
		PlaySound(MusicKeyDown);
		return;
  	}
}


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      系统OFF模式,关闭所有加热和显示                                         *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static void PowerDownProc(void)
{
    uint16 u16Key;	
	u16Key = GetKey(); 

    if(u16Key>0)         //按下任何键唤醒进入待机
    {
        if(IsNewKey())
        {
            EnterDefault();  
            if(bKeyMusic_EN==0)PlaySound(MusicKeyDown);
            return;
        }
    }

//      LCDDispAll(0);      //关闭液晶显示，已在LCD驱动程序完成
    ClrLedBlank(BIT_LED_ALL|BIT_LED_BKL);//关闭指示灯和背光
    bHeatPwrReq =   0;      //关所有加热
    g_u8HiTemp	=	0;					
    bHeatTopReq	=	0;		//关闭顶加热
    IO_HEAT_TOP	=	0;	    //关闭顶加热
    bHeatArdReq =   0;      //关闭侧加热
    IO_HEAT_MID =   0;      //关闭侧加热
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      系统自检状态处理                                     				   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	SelfCheckProc(void)
{
	uint16	u16Key;
	u16Key =	GetKey();

	if(TstBit(u16Key,BIT_KEY_CANCEL))
	{
		if(IsNewKey())
		{	
		    if(m_u8SelfChkStep==8)
            {
                m_u8SelfChkStep++;
            }
            else
            {
                EnterSetup();
            }
		}
  	}

    if(TstBit(u16Key,BIT_KEY_PLUS))
    {
        if(IsNewKey())
        {
            if((m_u8SelfChkStep==0)||(m_u8SelfChkStep==5))
            {
                m_u8SelfChkStep++;
            }
        }
    }

    if(TstBit(u16Key,BIT_KEY_REHEAT))
    {
        if(IsNewKey())
        {
            if(m_u8SelfChkStep==1)
            {
                m_u8SelfChkStep++;
            }
        }
    }

    if(TstBit(u16Key,BIT_KEY_KEEPWARM))
    {
        if(IsNewKey())
        {
             if(m_u8SelfChkStep==2)
             {
                m_u8SelfChkStep++;
             }
        }
    }

    if(TstBit(u16Key,BIT_KEY_DELAY))
    {
        if(IsNewKey())
        {
            if(m_u8SelfChkStep==3)
            {
                 m_u8SelfChkStep++;
            }
        }
    }

    if(TstBit(u16Key,BIT_KEY_STOP))
    {
        if(IsNewKey())
        {
            if(m_u8SelfChkStep==4)
            {
                 m_u8SelfChkStep++;
            }
        }
    }

    if(TstBit(u16Key,BIT_KEY_START))
    {
        if(IsNewKey())
        {
            if(m_u8SelfChkStep==6)
            {
                 m_u8SelfChkStep++;
            }
        }
    }

    if(TstBit(u16Key,BIT_KEY_MINUS))
    {
        if(IsNewKey())
        {
            if(m_u8SelfChkStep==7)
            {
                 m_u8SelfChkStep++;
            }
        }
    }

    if(TstBit(u16Key,BIT_KEY_DIY))
    {
        if(IsNewKey())
        {
            if(m_u8SelfChkStep==9)
            {
                 m_u8SelfChkStep++;
            }
        }
    }

    if(TstBit(u16Key,BIT_KEY_TIMER))
    {
        if(IsNewKey())
        {
            if(m_u8SelfChkStep==10)
            {
                 m_u8SelfChkStep++;
            }
        }
    }
    
    if(TstBit(u16Key,BIT_KEY_MENU))
    {
        if(IsNewKey())
        {
            if((m_u8SelfChkStep==11)||(m_u8SelfChkStep==12)||(m_u8SelfChkStep==13)
            ||(m_u8SelfChkStep==14)||(m_u8SelfChkStep==15)||(m_u8SelfChkStep==16)||(m_u8SelfChkStep==17))
            {
                m_u8SelfChkStep++;  
                if(m_u8SelfChkStep==15)
                {
                    bHeatPwrReq =   1;         
                    g_u8HiTemp	=	0;
			        bHeatTopReq	=	1;	
                    bHeatArdReq =   0;
                }
                if(m_u8SelfChkStep==16)
                {
                    bHeatPwrReq =   1;                     
                    g_u8HiTemp	=	0;
			        bHeatTopReq	=	0;	
                    bHeatArdReq =   1;
                }
                if(m_u8SelfChkStep==17)
                {
                    bHeatPwrReq =   1;                       
                    g_u8HiTemp = 102;
    		        g_u8LoTemp = 100;
    		        g_u8OnTime_Set = 0xf0;
                    bHeatTopReq	=	0;
                    bHeatArdReq = 0;
                }
                if(m_u8SelfChkStep==18)
                {
                    bHeatPwrReq = 0;        //关所有加热
                    g_u8HiTemp = 0;	        //停止加热	
                    bHeatTopReq	= 0;
                    bHeatArdReq = 0;
                    g_u8OnTime_Set = 0;
                    PlaySound(MusicPoweron);
                }
            }
        }
    }
}

/*-------------------------------状态入口程序----------------------------------*
/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      进入复位模式                                     				       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	EnterReset(void)
{
    ReadFlash(0x0000,(uint8*)&Flash);		//读取类EEPROM
    if(Flash.RecordFlag!=RECORD_NOEMPTY)    //判断是否有初始值
	{
	    Flash.ROM[SSP_DIY1_TM1] = 30;       //若没有初始值，则需要赋值
		Flash.ROM[SSP_DIY1_TM2] = 0;
		Flash.ROM[SSP_DIY1_TMP] = 100;
        Flash.ROM[SSP_DIY1_COOKMODE] = 1;
        Flash.ROM[SSP_DIY1_TEXTURE] = 1;
        Flash.ROM[SSP_DIY1_PEOPLE] = 2;
        Flash.ROM[SSP_DIY1_AGE] = 0;
        
        Flash.ROM[SSP_DIY2_TM1] = 30;       
		Flash.ROM[SSP_DIY2_TM2] = 0;
		Flash.ROM[SSP_DIY2_TMP] = 100;
        Flash.ROM[SSP_DIY2_COOKMODE] = 1;
        Flash.ROM[SSP_DIY2_TEXTURE] = 1;
        Flash.ROM[SSP_DIY2_PEOPLE] = 2;
        Flash.ROM[SSP_DIY2_AGE] = 0;
        
        Flash.ROM[SSP_DIY3_TM1] = 30;       
		Flash.ROM[SSP_DIY3_TM2] = 0;
		Flash.ROM[SSP_DIY3_TMP] = 100;
        Flash.ROM[SSP_DIY3_COOKMODE] = 1;
        Flash.ROM[SSP_DIY3_TEXTURE] = 1;
        Flash.ROM[SSP_DIY3_PEOPLE] = 2;
        Flash.ROM[SSP_DIY3_AGE] = 0;
        
        Flash.ROM[SSP_DIY4_TM1] = 30;       
		Flash.ROM[SSP_DIY4_TM2] = 0;
		Flash.ROM[SSP_DIY4_TMP] = 100;
        Flash.ROM[SSP_DIY4_COOKMODE] = 1;
        Flash.ROM[SSP_DIY4_TEXTURE] = 1;
        Flash.ROM[SSP_DIY4_PEOPLE] = 2;
        Flash.ROM[SSP_DIY4_AGE] = 0;
        
        Flash.ROM[SSP_DIY5_TM1] = 30;       
		Flash.ROM[SSP_DIY5_TM2] = 0;
		Flash.ROM[SSP_DIY5_TMP] = 100;
        Flash.ROM[SSP_DIY5_COOKMODE] = 1;
        Flash.ROM[SSP_DIY5_TEXTURE] = 1;
        Flash.ROM[SSP_DIY5_PEOPLE] = 2;
        Flash.ROM[SSP_DIY5_AGE] = 0;
        
        Flash.ROM[SSP_LASTMENU] = FUNC_ID_WHITERICE;
        Flash.ROM[SSP_LASTDIY] = 0;
        Flash.ROM[SSP_LASTAGE] = 0;
        
        Flash.ROM[SSP_MUSIC_EN] = 0x00;     //有声音
        SSP_Flag = SSP_ERASE_FLAG;
		EraseFlash(0x0000);                 //擦除类EEPROM
	    SSP_Flag = SSP_WRITE_FLAG;
	    WriteFlash(0x0000,(uint8*)&Flash);
    }
    bKeyMusic_EN = 0;                       //复位中关闭按键声音
    u16PoweronCnt = 400; 
    bWarm_EN = 1;
	m_u8SysID	=	SYS_ID_RESET;
	bSys_1000MS	=	0;
    m_u8ResetCnt = 0;
    LedOn(BIT_LED_BKL);
    LedOn(BIT_LED_ALL);
	PlaySound(MusicPoweron);			
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      进入设置模式                                     				       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	EnterSetup(void)
{
	m_u8SysID	=	SYS_ID_SETUP;					//系统进入待机状态
	ResetCurScript();								//禁止Script运行
	bHeatPwrReq =   0;                              //关所有加热
	g_u8HiTemp	=	0;					
	bHeatTopReq	=	0;								//关闭顶加热
	IO_HEAT_TOP	=	0;								//关闭顶加热
	bHeatArdReq =   0;                              //关闭侧加热
	IO_HEAT_MID =   0;                              //关闭侧加热

    m_u8CookMode = FUNC_ID_WHITERICE;
    m_u8Feel = 1;
    m_u8People = 2;
    m_u8Foodtype = 0;
    m_u8Watertype = 0;
    m_u8Age = 0;
    m_u8DIYMode = 0;  
    m_u8SetStep = SET_PHASE_MENU;
    tmCookTime[0] = 59;
    tmCookTime[1] = 0;	
    tmCookTime[2] = 0;
    u8CookTemp = 0;			
    FuncSetDisp(m_u8SetStep);   
	tmBookTime[0]	=	59;	  						//清预约时间
	tmBookTime[1]	=	0;
	tmBookTime[2]	=	0;
	m_u8ChkCnt		=	0;							//清自检计数器
	g_u8ErrorType	=	0;							//清报警类型

	bBookTime_EN	=	0;
	bCalBookTime_EN	=	0;
	bTimeDisp_EN	=	0;							//时间不允许调整
	bTempDisp_EN    =   0;
    bTimeAdjSt      =   0;
    m_u8AutoRunCnt = 0;
    m_u8DispDelayCnt = 60;
    LedOn(BIT_LED_BKL);
	bStageChgReq = 0;
    bSoakStage = 0;
    bHeatStage = 0;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      进入默认设置(上一次运行的功能)                                         *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	EnterDefault(void)
{
    EnterSetup();
    m_u8CookMode = Flash.ROM[SSP_LASTMENU];   
    m_u8LastAge = Flash.ROM[SSP_LASTAGE];
    if(m_u8CookMode==FUNC_ID_DIY)               //默认功能是DIY功能
    {   
        m_u8DIYMode = Flash.ROM[SSP_LASTDIY];
        m_u8SetStep = SET_PHASE_DIY;
        SetDIYModeByDIY(m_u8DIYMode);
    }
    else                            //默认是菜单功能
    {
       if(m_u8CookMode==FUNC_ID_REHEAT)
       {
            m_u8SetStep = SET_PHASE_DEFAULT;
       }
       else
       {
            m_u8SetStep = SET_PHASE_MENU;  
       }
       if(m_u8CookMode>10)
       {
           SetDefCookTime(m_u8CookMode);
           SetDefCookTemp(m_u8CookMode);
           tmCookTime[0] = 59;
           tmCookTime[1] = tmDefCookTime[1];	//默认烹饪时间
           tmCookTime[2] = tmDefCookTime[2];
           u8CookTemp = DefCookTemp;			//默认烹饪温度
           m_u8Feel = 0;
           m_u8People = 0;
       }
    }
    FuncSetDisp(m_u8SetStep);
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      进入工作模式                                     				       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	EnterRun(void)
{
    uint8 i=0,j=0;

    if(m_u8DIYMode>0)
    {
        if(m_u8DIYMode==1) i = 0;
        if(m_u8DIYMode==2) i = 7;
        if(m_u8DIYMode==3) i = 14;
        if(m_u8DIYMode==4) i = 21;
        if(m_u8DIYMode==5) i = 28;   

        if(m_u8CookMode==FUNC_ID_DIY)
        {
            if((Flash.ROM[i+SSP_DIY1_TM1]!=tmCookTime[1])||(Flash.ROM[i+SSP_DIY1_TM2]!=tmCookTime[2])||(Flash.ROM[i+SSP_DIY1_TMP]!=u8CookTemp)
            ||(Flash.ROM[SSP_LASTMENU]!=m_u8CookMode)||(Flash.ROM[SSP_LASTDIY]!=m_u8DIYMode))
            {
                j = 1;  //需要写入EEPROM
                Flash.ROM[i+SSP_DIY1_TM1] = tmCookTime[1];
                Flash.ROM[i+SSP_DIY1_TM2] = tmCookTime[2];
                Flash.ROM[i+SSP_DIY1_TMP] = u8CookTemp;
                Flash.ROM[SSP_LASTMENU] = m_u8CookMode;
                Flash.ROM[SSP_LASTDIY] = m_u8DIYMode;
            }
        }
        else
        {
            if((m_u8CookMode>0)&&(m_u8CookMode<=21))
            {
                j = 1;  //需要写入EEPROM
                Flash.ROM[i+SSP_DIY1_COOKMODE] = m_u8CookMode;
                Flash.ROM[i+SSP_DIY1_TEXTURE] = m_u8Feel;
                Flash.ROM[i+SSP_DIY1_PEOPLE] = m_u8People;
                if(m_u8CookMode==FUNC_ID_BABYPORRIDGE)
                {
                    Flash.ROM[i+SSP_DIY1_AGE] = m_u8Age;
                }
                Flash.ROM[SSP_LASTMENU] = FUNC_ID_DIY;
                Flash.ROM[SSP_LASTDIY] = m_u8DIYMode;
            }
        }
    }
    else
    {
        if((Flash.ROM[SSP_LASTMENU]!=m_u8CookMode)||(Flash.ROM[SSP_LASTAGE]!=m_u8Age))
        {
            j = 1;      //需要写入EEPROM
            Flash.ROM[SSP_LASTMENU] = m_u8CookMode;
            if(m_u8CookMode==FUNC_ID_BABYPORRIDGE)
            {
                Flash.ROM[SSP_LASTAGE] = m_u8Age;
            }
        }
    }
    
    if(j==1)         
    {
        SSP_Flag = SSP_ERASE_FLAG;
        EraseFlash(0x0000);                 //擦除类EEPROM
    	SSP_Flag = SSP_WRITE_FLAG;
    	WriteFlash(0x0000,(uint8*)&Flash);
    }
    
    m_u8SysID =	SYS_ID_RUN;			//系统进入烹饪状态
    m_u8RunStep = RUN_PHASE_START;  //开始烹饪
    FuncRunDisp(RUN_PHASE_START);
    LoadVarParam(m_u8CookMode, m_u8Feel, m_u8People);
	SetCurScript(m_u8CookMode,m_u8Feel);
	bTimeDisp_EN = 0;		
	bTempDisp_EN = 0;		
    bTimeAdjSt = 0;
    bDebug_EN = 0;
	m_u8AutoRunCnt = 0;
    m_u8CurTimeDispCnt = 0;
	m_u8PreTmp = ADCGetData(AD_CH_BOT);	
    LedOn(BIT_LED_BKL);
    PlaySound(MusicPoweron);        //开始运行长叫一声
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      进入保温模式                                     				       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	EnterWarm(void)
{
	m_u8SysID	=	SYS_ID_WARM;
	ResetCurScript();								//禁止Script运行
	bHeatPwrReq =   0;                              //关所有加热
	g_u8HiTemp	=	0;						
	bHeatTopReq	=	0;								//关闭顶加热
	IO_HEAT_TOP	=	0;								//关闭顶加热
	bHeatArdReq =   0;                              //关闭侧加热
	IO_HEAT_MID =   0;                              //关闭侧加热
				
	tmCookTime[0]	=	0;							//清烹饪时间
	tmCookTime[1]	=	0;
	tmCookTime[2]	=	0;
	bBookTime_EN	=	0;
	bCalBookTime_EN	=	0;
	bTimeDisp_EN 	=	0;    						//时间不允许调整
	m_u8ChkCnt		=	0;						    //清自检计数器
	bWarm_Start     =   0;
    bSoakStage = 0;
    bHeatStage = 0;
    m_u8DispDelayCnt = 60;
    LCDDispZone1(0, 0, ZONE1_STAT_ALLOFF);
    LCDDispZone2(0, ZONE2_STAT_ALLOFF);
    LCDDispZone3(0, 0, 0, ZONE3_STAT_ALLOFF);
    LCDDispZone4(0,0,tmCookTime,ZONE4_STAT_SET_DEFAULT);
    PlayGif(NULL, 0 ,1);
    ClrLedBlank(BIT_LED_ALL);
    SetLedBlank(BIT_LED_START);
    SetLedBlank(BIT_LED_STOP);
    SetLedBlank(BIT_LED_WARM);
    LedOn(BIT_LED_BKL);
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      进入关机模式                                    				       *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
 void   EnterPowerDown(void)
 {
    m_u8SysID = SYS_ID_POWRDOWN;
    ResetCurScript();               //禁止Script运行
    bHeatPwrReq =   0;              //关所有加热
    g_u8HiTemp = 0;
    bHeatTopReq	=	0;				//关闭顶加热
	IO_HEAT_TOP	=	0;			    //关闭顶加热
	bHeatArdReq =   0;              //关闭侧加热
	IO_HEAT_MID =   0;              //关闭侧加热
	m_u8DispDelayCnt = 0;           //进入OFF模式
 }

/*-------------------------------故障检测程序----------------------------------*

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      故障检测                                      		   				   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	FaultCheckProc(void)
{
	uint8	u8BotDegree	=	ADCGetData(AD_CH_BOT);
	uint8   u8TopDegree =   ADCGetData(AD_CH_TOP);
	uint16	adBotData	=	ADCGetVal(AD_CH_BOT);
	uint16	adTopData	=	ADCGetVal(AD_CH_TOP);

	if(bCookBySctEN==1)
    {
        if(m_u8CookMode==FUNC_ID_PASTA)
        {
    	    if(bTempAch1st==0)
    	    {
    		    if(u8TopDegree>=90)
    		    {
    			    bTempAch1st = 1;
                    PlaySound(MusicAchievedTemp);
    		    }
    	    }	
        }
    }
    
    if((bCookBySctEN==1)||(m_u8SysID==SYS_ID_WARM))
	{   
	    if(bSys_1000MS)
		{
            if(u8BotDegree>=160)
            {
                u8ErrorCnt++;
                if(u8ErrorCnt>=3)
                {
                    if((m_u8CookMode!=FUNC_ID_DIY)&&(m_u8CookMode!=FUNC_ID_BAKING)&&(m_u8CookMode!=FUNC_ID_BROWNING)&&(m_u8CookMode!=FUNC_ID_DESSERT))
                    {
                        bPanOverTempErr	= 1;			    
                    }
                    if(m_u8SysID==SYS_ID_WARM)
                    {
                        bPanOverTempErr	= 1;
                    }
                }         
			}
            else
            {
                u8ErrorCnt = 0;
            }
        }
	}

	if((m_u8SysID==SYS_ID_CHECK)||(m_u8SysID==SYS_ID_ERROR)||(bCookBySctEN==1)||(m_u8SysID==SYS_ID_WARM))
	{
		if((adBotData<10)||(adBotData>1014))	bBotSensorErr=1;
		else    bBotSensorErr=0;	
		if((adTopData<10)||(adTopData>1014))	bTopSensorErr=1;
		else    bTopSensorErr=0;	
	}
			
	if((m_u8SysID==SYS_ID_RUN)||(m_u8SysID==SYS_ID_WARM)||(m_u8SysID==SYS_ID_CHECK))
	{
		if(g_u8ErrorType)
		{
			m_u8PrevSysID = m_u8SysID;
			m_u8SysID = SYS_ID_ERROR;
			PlaySound(MusicSysError); 				//开报警声音	
			bHeatPwrReq =   0;                     //关所有加热
			g_u8HiTemp	=	0;
			bHeatTopReq = 0;
			bHeatArdReq = 0;
		}
 	}
}

/*------------------------Autotest activate Process----------------------------*

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Plug in and in the same time  press the start botton during 2s         *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    AutotestActivate(void)
{
    if(u16PoweronCnt>0)
    {
        u16PoweronCnt--;
        if(m_u16KeyVal==VAL_KEY_START)
        {
            if(g_u16KeyRpyCnt>4)
            {
                if(m_u8SysID!=SYS_ID_CHECK)
                {
                    m_u8SysID = SYS_ID_CHECK;
			        m_u8SelfChkStep = 0;
                    PlaySound(MusicKeyDown);
                }
            }
        }
    }
}