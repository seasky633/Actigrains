/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"
#include ".\head\SinoLib.h"

/*-----------------------------*
 *  public functions prototypes*
 *----------------------------*/
void	LCD_INIT(void);
void	LCDDispAll(uint8 mode);
void    LCDDispMenuNum(uint8 menu,Bool lcdon);
void    LCDDispTexture(uint8 texture,Bool lcdon);
void    LCDDispPeopleNum(uint8 count,Bool lcdon);
void    LCDDispFoodCups(uint16 cups,Bool lcdon);
void    LCDDispWaterML(uint16 ml,Bool lcdon);
void    LCDDispCookTemp(uint8 temp,Bool lcdon);
void    LCDDispCookTime(uint8 *ptime,Bool lcdon);
void	LCDDispNum_HI(uint8 hdigit);
void    LCDDispNum_LO(uint8 ldigit);
void  	LCDClearNum(void);
void    LCDProc(uint8 sysid);
void    LCDDispError(void);
void	LCDDispGif(void);
void	PlayGif(uint16 code *pGif,uint16 speed,uint8 stat);
void    LCDDispZone1(uint8 menu,uint8 texture,uint8 state);
void    LCDDispZone2(uint8 people,uint8 state);
void    LCDDispZone3(uint16 cup,uint16 water,uint8 foodtype,uint8 state);
void    LCDDispZone4(uint8 diynum,uint8 temp,uint8 *ptime,uint8 state);
void    LCDDispSelfChk(uint8 step);
void    LCDDispMovie(uint8 stat);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/


/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint8  bdata    g_u8LCDVal[2];
sbit bLCD_HealthIcon  = g_u8LCDVal[0]^0;
sbit bLCD_ClassicIcon = g_u8LCDVal[0]^1;
sbit bLCD_MenuNum     = g_u8LCDVal[0]^2;
sbit bLCD_Texture     = g_u8LCDVal[0]^3;
sbit bLCD_PeopleIcon  = g_u8LCDVal[0]^4; 
sbit bLCD_PeopleNum   = g_u8LCDVal[0]^5;
sbit bLCD_TempIcon    = g_u8LCDVal[0]^6;
sbit bLCD_TimeIcon    = g_u8LCDVal[0]^7;

sbit bLCD_DIYNum1     = g_u8LCDVal[1]^0;
sbit bLCD_DIYNum2     = g_u8LCDVal[1]^1;
sbit bLCD_DIYNum3     = g_u8LCDVal[1]^2;
sbit bLCD_DIYNum4     = g_u8LCDVal[1]^3;
sbit bLCD_DIYNum5     = g_u8LCDVal[1]^4;
sbit bLCD_TempNum     = g_u8LCDVal[1]^5;
sbit bLCD_TimeNum     = g_u8LCDVal[1]^6;
sbit bLCD_COL         = g_u8LCDVal[1]^7;

uint8  bdata    g_u8BlankVal[2];	
sbit bLCD_BNK_HealthIcon  = g_u8BlankVal[0]^0;
sbit bLCD_BNK_ClassicIcon = g_u8BlankVal[0]^1;
sbit bLCD_BNK_MenuNum     = g_u8BlankVal[0]^2;
sbit bLCD_BNK_Texture     = g_u8BlankVal[0]^3;
sbit bLCD_BNK_PeopleIcon  = g_u8BlankVal[0]^4; 
sbit bLCD_BNK_PeopleNum   = g_u8BlankVal[0]^5;
sbit bLCD_BNK_TempIcon    = g_u8BlankVal[0]^6;
sbit bLCD_BNK_TimeIcon    = g_u8BlankVal[0]^7;

sbit bLCD_BNK_DIYNum1     = g_u8BlankVal[1]^0;
sbit bLCD_BNK_DIYNum2     = g_u8BlankVal[1]^1;
sbit bLCD_BNK_DIYNum3     = g_u8BlankVal[1]^2;
sbit bLCD_BNK_DIYNum4     = g_u8BlankVal[1]^3;
sbit bLCD_BNK_DIYNum5     = g_u8BlankVal[1]^4;
sbit bLCD_BNK_TempNum     = g_u8BlankVal[1]^5;
sbit bLCD_BNK_TimeNum     = g_u8BlankVal[1]^6;
sbit bLCD_BNK_COL         = g_u8BlankVal[1]^7;

uint8	xdata	SEG[28] _at_ 0x500;
uint8 	xdata	LCD_Buf[28];
uint8	u8BlankCnt;
uint8   u8lcdcnt ;
DispParam idata dpParam;
uint8	u8GifCnt;
uint8	idata u8GifSpeed;
uint8   xdata m_u8Framestat;       //旋转方框状态，0x00:清除与动画无关的部分 0x01:全清 0x02:旋转 other:全显
uint16  code *m_u16pGif = NULL;
uint16  code *m_u16pGifBak =  NULL;		
uint8 	code LCD_TAB[] = 
{
	Num0,Num1,Num2,Num3,Num4,Num5,Num6,Num7,
	Num8,Num9,Numa,Numb,Numu,Numt,Nume,Numf
};

uint8 	code LCD_TABX[] = 
{
	Num0x,Num1x,Num2x,Num3x,Num4x,Num5x,Num6x,Num7x,Num8x,Num9x
};

uint8 	code LCD_TABY[] = 
{
	Num0y,Num1y,Num2y,Num3y,Num4y,Num5y,Num6y,Num7y,Num8y,Num9y
};
//煲体(带篮子)
uint16  code GifCookerBSK[] = {GIF_Cooker_BSK_S1,0};
//煲体(不带篮子)
uint16  code GifCookerNBSK[] = {GIF_Cooker_NBSK_S1,0};
//煲体(不带篮子)(不带上盖)
uint16  code GifCookerNBSKNLID[] = {GIF_Cooker_NBSK_NLID_S1,0};
//放入食物(带篮子)
uint16  code GifFoodBSK[] = {GIF_Food_BSK_S1,GIF_Food_BSK_S2,GIF_Food_BSK_S3,0xffff};
//放入食物(不带篮子)
uint16  code GifFoodNBSK[] = {GIF_Food_NBSK_S1,GIF_Food_NBSK_S2,GIF_Food_NBSK_S3,GIF_Food_NBSK_S4,0xffff};
//放入水(带篮子)
uint16  code GifWaterBSK[] = {GIF_Water_BSK_S1,GIF_Water_BSK_S2,GIF_Water_BSK_S3,0xffff};
//放入水(不带篮子)
uint16  code GifWaterNBSK[] = {GIF_Water_NBSK_S1,GIF_Water_NBSK_S2,GIF_Water_NBSK_S3,GIF_Water_NBSK_S4,0xffff};
//放入篮子
uint16  code GifPutbasket[] = {GIF_Putbasket_S1,GIF_Putbasket_S2,GIF_Putbasket_S3,0xffff};
//取出食物(带篮子)
uint16  code GifOutfood[] = {GIF_Outfood_S1,GIF_Outfood_S2,GIF_Outfood_S3,GIF_Outwater_S1,GIF_Outwater_S2,GIF_Outwater_S3,0xffff};
//倒出水
uint16  code GifOutwater[] = {GIF_Outwater_S1,GIF_Outwater_S2,GIF_Outwater_S3,0xffff};
//放入滤水后的食物(带篮子)
uint16  code GifDrainedfoodBSK[] = {GIF_Drainedfood_BSK_S1,GIF_Drainedfood_BSK_S2,GIF_Drainedfood_BSK_S3,0xffff};
//放入滤水后的食物(不带篮子)
uint16  code GifDrainedfoodNBSK[] = {GIF_Drainedfood_NBSK_S1,GIF_Drainedfood_NBSK_S2,GIF_Drainedfood_NBSK_S3,0xffff};
/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/






/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the LCD.                                                    *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void LCD_INIT(void)
{
	uint8	i;

    //使能LCD模块
	DISPCON  =	Bin(00111111);		// 选择LCD驱动器，Vlcd=1.000Vdd，1/8占空比，1/4偏置
	DISPCON1 =  Bin(00000001);      // 偏置电阻总和60K
    DISPCLK0 =	Bin(00000000);      // LCD时钟频率 = 32.768Khz(帧频64hz)	
	DISPCON	|=	Bin(01000000);		// 使能LCD模块

	//显示内容初始化
    for(i=0;i<VBUF_SIZE;i++)				// LCD RAM全部清零
	{
		SEG[i] = Bin(00000000);
		LCD_Buf[i] = Bin(00000000);
	}
}


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ALL LED显示模式                                                        *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LCDDispAll(uint8 mode)
{	
	uint8	i;
			
	for(i=0;i<VBUF_SIZE;i++)
	{
		if(mode>0)
		{
			LCD_Buf[i] = Bin(11111111);
		}
		else
		{
			LCD_Buf[i] = Bin(00000000);
		}
	}
}	

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      显示菜单号(P1-P8,01-12)                                                *
 *  Parameters:                                                                *
 *      menu,lcdon                                                             *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LCDDispMenuNum(uint8 menu,Bool lcdon)
{
    uint8 i,j;
    
    LCD_Buf[10] = 0x00;             //MenuNum低位
    LCD_Buf[11] = 0x00;             //MenuNum高位

    if(lcdon)
    {
        if(menu<9)
        {
            LCD_Buf[10] = LCD_TAB[menu];    //显示MenuNum低位
            LCD_Buf[11] = LCD_TAB[0];       //高位显示"0"
        }
        else
        {
            if(menu<22)
            {
                if(menu==9)                 //chickpeas
                {
                    LCD_Buf[10] = LCD_TAB[0];  
                    LCD_Buf[11] = LCD_TAB[0];       //高位显示"0"
                }
                else
                {
                    i = LOBYTE(HEX8toBCD(menu-1));
                    j = i & 0x0f;
                    LCD_Buf[10] = LCD_TAB[j];           //显示MenuNum低位
                    j = (i>>4) & 0x0f;
                    LCD_Buf[11] = LCD_TAB[j];           //显示MenuNum高位
                }
            }
        }
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      显示口感(Nutri,Quick,Normal,Soft)                                      *
 *  Parameters:                                                                *
 *      texture,lcdon                                                          *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LCDDispTexture(uint8 texture, Bool lcdon)
{
    LCD_Buf[3] &= Bin(11100001);    //清除口感显示

    if(lcdon)
    {
        switch(texture)
        {
            case    1:              //nutritious
                LCD_Buf[3] |= Bin(00011110);
                break;
            case    2:              //quick
                LCD_Buf[3] |= Bin(00001110);
                break;
            case    3:              //normal
                LCD_Buf[3] |= Bin(00000110);
                break;
            case    4:              //soft
                LCD_Buf[3] |= Bin(00000010);
                break;
            default:
                break;
        }
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      显示人数(2-12)                                                         *
 *  Parameters:                                                                *
 *      count,lcdon                                                            *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LCDDispPeopleNum(uint8 count,Bool lcdon)
{
     uint8 i;
     
     LCD_Buf[9] &= Bin(10000000);             //PeopleNum低位
     LCD_Buf[4] &= Bin(11111011);             //PeopleNum高位

    if(lcdon)
    {
        if(count>0)
        {
            if(count<10)
            {
                i = count;
            }
            else
            {
                LCD_Buf[4] |= 0x04;      //显示PeopleNum高位
                i = count - 10;
            }

            LCD_Buf[9] |= LCD_TABY[i];   //显示PeopleNum低位
        }
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      显示需要放入的食物量                                                   *
 *  Parameters:                                                                *
 *      cups,lcdon                                                             *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LCDDispFoodCups(uint16 cups,Bool lcdon)
{
    uint8 i;
    
    LCD_Buf[18] &= 0x0f;    //D5
    LCD_Buf[17] &= 0x0f;
    LCD_Buf[16] &= 0x0f;    //D6
    LCD_Buf[15] &= 0x0f;
    LCD_Buf[14] &= 0x0f;    //D7
    LCD_Buf[13] &= 0x0f;

    if(lcdon)
    {
        i = ((uint8)cups) & 0x0f;
        LCD_Buf[13] |= LCD_TABX[i] & 0xf0;      //D7(A/B/C/D)
        LCD_Buf[14] |= (LCD_TABX[i]<<4) & 0xf0; //D7(F/G/E)

        i = ((uint8)cups>>4) & 0x0f;
        LCD_Buf[15] |= LCD_TABX[i] & 0xf0;      //D6(A/B/C/D)
        LCD_Buf[16] |= (LCD_TABX[i]<<4) & 0xf0; //D6(F/G/E)

        i = (uint8)(cups>>8) & 0x0f;
        LCD_Buf[17] |= LCD_TABX[i] & 0xf0;      //D5(A/B/C/D)
        LCD_Buf[18] |= (LCD_TABX[i]<<4) & 0xf0; //D5(F/G/E)

        LCD_Buf[16] |= 0x10;                    //P1
    }
}


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      显示需要放入的水量                                                     *
 *  Parameters:                                                                *
 *      ml,lcdon                                                               *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LCDDispWaterML(uint16 ml,Bool lcdon)
{
    uint8 i;

    LCD_Buf[18] &= 0xf0;    //D8
    LCD_Buf[17] &= 0xf0;    
    LCD_Buf[16] &= 0xf0;    //D9
    LCD_Buf[15] &= 0xf0;
    LCD_Buf[14] &= 0xf0;    //D10
    LCD_Buf[13] &= 0xf0;
    LCD_Buf[12] &= 0xf0;    //D11
    LCD_Buf[0]  &= 0xf1;

    if(lcdon)
    {
        i = ((uint8)ml) & 0x0f;
        LCD_Buf[0]  |= LCD_TABX[i] & 0x0e;      //D11(F/G/E)
        LCD_Buf[12] |= (LCD_TABX[i]>>4) & 0x0f; //D11(A/B/C/D)

        i = ((uint8)ml>>4) & 0x0f;
        LCD_Buf[14] |= LCD_TABX[i] & 0x0f;      //D10(F/G/E)
        LCD_Buf[13] |= (LCD_TABX[i]>>4) & 0x0f; //D10(A/B/C/D)

        i = (uint8)(ml>>8) & 0x0f;
        LCD_Buf[16] |= LCD_TABX[i] & 0x0f;      //D9(F/G/E)
        LCD_Buf[15] |= (LCD_TABX[i]>>4) & 0x0f; //D9(A/B/C/D)
        
        i = (uint8)(ml>>12) & 0x0f;
        if(i>0)
        {
            LCD_Buf[18] |= LCD_TABX[i] & 0x0f;      //D8(F/G/E)
            LCD_Buf[17] |= (LCD_TABX[i]>>4) & 0x0f; //D8(A/B/C/D)
        }
    }
}
    

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      显示设置的烹饪温度                                                     *
 *  Parameters:                                                                *
 *      temp,lcdon                                                             *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LCDDispCookTemp(uint8 temp,Bool lcdon)
{
    uint8 i,j;
    
    LCD_Buf[23] = 0;
    LCD_Buf[22] &= 0x01;

    if(lcdon)
    {
        if(temp>0)
        {
            i =	LOBYTE(HEX8toBCD(temp));		  	//显示温度低二位
            j = i & 0x0f;
            LCD_Buf[22] |= LCD_TAB[j] & 0xfe;       //D13
            j = (i>>4) & 0x0f;
            LCD_Buf[23] |= LCD_TAB[j] & 0xfe;       //D12

            if(temp>=100)
            {
              LCD_Buf[23] |= 0x01;                  //显示温度高位
            }
        }
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      显示设置的烹饪时间                                                     *
 *  Parameters:                                                                *
 *      *ptime,lcdon                                                           *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LCDDispCookTime(uint8 *pTime,Bool lcdon)
{
    uint8	i;
    
    LCDClearNum();
    
    if(lcdon)
    {
        if((pTime[0]==0xff)&&(pTime[1]==0xff))
		{
			LCDDispNum_LO(0xd0);  
			LCDDispNum_HI(0xac);
		}
        else
        {
    	    i =	LOBYTE(HEX8toBCD(pTime[0]));
    	    LCDDispNum_LO(i);				//显示分钟

    	    i =	LOBYTE(HEX8toBCD(pTime[1]));
    	    LCDDispNum_HI(i);				//显示小时
        }
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      显示数字高两位D14-D15                                                  *
 *  Parameters:                                                                *
 *      hdigit                                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LCDDispNum_HI(uint8 hdigit)
{
	uint8	i;

    LCD_Buf[21] &= 0x01;                //清D14-D15缓冲器
    LCD_Buf[20] &= 0x01;

    i = hdigit & 0x0f;                  //D15
    LCD_Buf[20] |= LCD_TAB[i] & 0xfe;				
   	
	i =	(hdigit>>4) & 0x0f;             //D14
	LCD_Buf[21] |=	LCD_TAB[i] & 0xfe;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      显示数字低两位D16-D17                                                  *
 *  Parameters:                                                                *
 *      ldigit                                                                 *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LCDDispNum_LO(uint8	ldigit)
{
	uint8	i;

    LCD_Buf[19] &= 0x01;                 //清D16-D17缓冲区
    LCD_Buf[1] &= 0x01;         

    i = ldigit & 0x0f;                  //D17
    LCD_Buf[1] |= LCD_TAB[i] & 0xfe;

    i = (ldigit>>4) & 0x0f;             //D16
    LCD_Buf[19] |= LCD_TAB[i] &0xfe; 				
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      清除数字显示                                                           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void  	LCDClearNum(void)
{	
    LCD_Buf[21] &= 0x01;                //清D14-D15缓冲器
    LCD_Buf[20] &= 0x01;
    LCD_Buf[19] &= 0x01;                //清D16-D17缓冲区
    LCD_Buf[1] &= 0x01;  
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      报警显示                                          					   *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	LCDDispError(void)
{
    LCDDispAll(0);
    g_u8LCDVal[0] = 0;
    g_u8LCDVal[1] = 0;
    g_u8BlankVal[0] = 0;
    g_u8BlankVal[1] = 0;
    
	if(bTopSensorErr)			//显示E0-顶部传感器开路或短路
	{
		LCDDispNum_LO(0xe0);
		return;
	}

	if(bBotSensorErr) 			//显示E1-底部传感器开路或短路
	{
		LCDDispNum_LO(0xe1);
		return;
	}

	if(bPanOverTempErr)	 		//显示E3-锅底温度过高
	{
		LCDDispNum_LO(0xe3);
		return;
  	}	
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      控制Zone1显示内容                                        			   *
 *  Parameters:                                                                *
 *      menu,texture,state                                                     *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LCDDispZone1(uint8 menu,uint8 texture,uint8 state)
{
    g_u8LCDVal[0] &= 0xf0;
    g_u8BlankVal[0] &= 0xf0;
    LCD_Buf[3] |= 0x81;            //显示ZONE1外框
	
    switch(state)
    {
        case    ZONE1_STAT_ALLOFF:
            LCD_Buf[3] &= 0x7e;    //关ZONE1外框
            break;
        case    ZONE1_STAT_NUTRIMENU_SELECT:
            bLCD_HealthIcon = 1;
            bLCD_BNK_HealthIcon = 1;
            bLCD_MenuNum = 1;
            bLCD_BNK_MenuNum = 1;
            dpParam.u8Menu = menu;
            break;
        case    ZONE1_STAT_CLASSICMENU_SELECT:
            bLCD_ClassicIcon = 1;
            bLCD_BNK_ClassicIcon = 1;
            bLCD_MenuNum = 1;
            bLCD_BNK_MenuNum = 1;
            dpParam.u8Menu = menu;
            break;
        case    ZONE1_STAT_TEXTURE_SELECT:
            if(menu<=9) bLCD_HealthIcon = 1;
            else bLCD_ClassicIcon = 1;               
            bLCD_MenuNum = 1;
            dpParam.u8Menu = menu;
            bLCD_Texture = 1;
            bLCD_BNK_Texture = 1;
            dpParam.u8texture = texture;
            break;
        case    ZONE1_STAT_TEXTURE_CONFIRM:
            if((menu==22)||(menu==23))
            {
                LCD_Buf[3] &= 0x7e;    //关ZONE1外框
            }
            else
            {
                if(menu<=9) bLCD_HealthIcon = 1;
                else bLCD_ClassicIcon = 1; 
                bLCD_MenuNum = 1;
                dpParam.u8Menu = menu;
                bLCD_Texture = 1;
                dpParam.u8texture = texture;
            }
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      控制Zone2显示内容                                        			   *
 *  Parameters:                                                                *
 *      people ,state                                                          *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LCDDispZone2(uint8 people,uint8 state)
{
    g_u8LCDVal[0] &= 0xcf;
    g_u8BlankVal[0] &= 0xcf;
    LCD_Buf[4] |= 0x01;             //显示zone2外框 Y7
    LCD_Buf[9] |= 0x80;             //Y8
    
    switch(state)
    {
        case    ZONE2_STAT_ALLOFF:
            LCD_Buf[4] &= 0xfe;     //关zone2外框    
            LCD_Buf[9] &= 0x7f;
            break;
        case    ZONE2_STAT_PEOPLENUM_SELECT:
            if(people>0)
            {    
                bLCD_PeopleIcon = 1;
                bLCD_BNK_PeopleIcon = 1;
                bLCD_PeopleNum = 1;
                bLCD_BNK_PeopleNum = 1;
                dpParam.u8People = people;
            }
            else
            {
                LCD_Buf[4] &= 0xfe;     //关zone2外框    
                LCD_Buf[9] &= 0x7f;
            }
            break;
        case    ZONE2_STAT_PEOPLENUM_CONFIRM:
            if(people>0)
            {
                bLCD_PeopleIcon = 1;
                bLCD_PeopleNum = 1;
                dpParam.u8People = people;
            }
            else
            {
                LCD_Buf[4] &= 0xfe;     //关zone2外框    
                LCD_Buf[9] &= 0x7f;
            }
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      控制Zone3显示内容                                        			   *
 *  Parameters:                                                                *
 *      cup,water,foodtype,state                                               *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LCDDispZone3(uint16 cup,uint16 water,uint8 foodtype,uint8 state)
{
    LCD_Buf[0] |= 0x81;             //显示zone3外框
    LCD_Buf[0] &= 0x8f;             //s8 s9 =0
    LCD_Buf[12] &= 0x0f;            //s10 s11 s12 s13 =0
    
    switch(state)
    {
        case    ZONE3_STAT_ALLOFF:
            LCD_Buf[0] &= 0x7e;     //关zone3外框
            LCDDispFoodCups(cup,0);
            LCDDispWaterML(water,0);
            if(foodtype>0)
            {
                if(foodtype==1) LCD_Buf[12] |= 0x40; //s11 = 1
                if(foodtype==2) LCD_Buf[12] |= 0x20; //s12 = 1
            }
            break;
        case    ZONE3_STAT_PUTFOOD:
            LCD_Buf[0] |= 0x40;     //s8 =1
            LCD_Buf[12] |= 0x80;    //s10 = 1
            if(foodtype==1) LCD_Buf[12] |= 0x40; //s11 = 1
            if(foodtype==2) LCD_Buf[12] |= 0x20; //s12 = 1
            LCDDispFoodCups(cup,1);
            LCDDispWaterML(water,0);
            break;
        case    ZONE3_STAT_PUTWATER:
            LCD_Buf[0] |= 0x20;     //s9 =1
            LCD_Buf[12] |= 0x10;    //s13 = 1
            if(foodtype==1) LCD_Buf[12] |= 0x40; //s11 = 1
            if(foodtype==2) LCD_Buf[12] |= 0x20; //s12 = 1
            LCDDispFoodCups(cup,0);
            LCDDispWaterML(water,1);
            break;
        case    ZONE3_STAT_DEBUG:
            #ifdef  DEBUG0
            LCDDispFoodCups(cup,1);
            LCDDispWaterML(water,1);
            #endif
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      控制Zone4显示内容                                        			   *
 *  Parameters:                                                                *
 *      diynum,temp,*ptime,state                                               *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/    
void    LCDDispZone4(uint8 diynum,uint8 temp,uint8 *ptime,uint8 state)
{
    g_u8LCDVal[0] &= 0x3f;
    g_u8BlankVal[0] &= 0x3f;
    g_u8LCDVal[1] = 0;
    g_u8BlankVal[1] = 0;
    LCD_Buf[2] |= 0x80;             //显示zone4外框 Y5
    LCD_Buf[22] |= 0x01;            //Y6
    LCD_Buf[21] &= 0xfe;            //s21 = 0
    LCD_Buf[20] &= 0xfe;            //s22 = 0
    LCD_Buf[1] &= 0xfe;             //s23 = 0
    
    if(diynum>0)
    {
        if(diynum==1) bLCD_DIYNum1 = 1;
        if(diynum==2) bLCD_DIYNum2 = 1;
        if(diynum==3) bLCD_DIYNum3 = 1;
        if(diynum==4) bLCD_DIYNum4 = 1;
        if(diynum==5) bLCD_DIYNum5 = 1;
        if(diynum==0x81) bLCD_DIYNum1 = 1;  //porridge 4-6months
        if(diynum==0x82) bLCD_DIYNum2 = 1;  //porridge 7-9months
        if(diynum==0x83) bLCD_DIYNum3 = 1;  //porridge 10-12months
        if(diynum==0x84) bLCD_DIYNum4 = 1;  //porridge 13-14months
    }
    
    switch(state)
    {
        case    ZONE4_STAT_ALLOFF:
            if(diynum==0)
            {
                g_u8LCDVal[1] = 0;
                LCD_Buf[2] &= 0x7f;         //关zone4外框 Y5
                LCD_Buf[22] &= 0xfe;        //Y6
            }
            break;
        case    ZONE4_STAT_SET_DEFAULT:
            if(temp>0)                  //温度可设置
            {
                bLCD_TempIcon = 1;
                bLCD_TempNum = 1;
                LCD_Buf[21] |= 0x01;    //s21 = 1
                dpParam.u8Temp = temp;  //显示温度
            }
            bLCD_TimeIcon = 1;
            bLCD_TimeNum = 1;
            bLCD_COL = 1;
            LCD_Buf[20] |= 0x01;         //s22 = 1
            LCD_Buf[1]  |= 0x01;         //s23 = 1
            dpParam.u8Time[0] = ptime[1];   //显示分钟
            dpParam.u8Time[1] = ptime[2];   //显示小时
            break;
        case    ZONE4_STAT_SET_CONFIRM:
            if((temp>0)&&(bBookTime_EN==0)) //温度可设置且无预约
            {
                bLCD_TempIcon = 1;
                bLCD_TempNum = 1;
                LCD_Buf[21] |= 0x01;    //s21 = 1
                dpParam.u8Temp = temp;  //显示温度
            }
            bLCD_TimeIcon = 1;
            bLCD_TimeNum = 1;
            bLCD_COL = 1;
            if(bBookTime_EN) bLCD_BNK_COL = 0;
            else bLCD_BNK_COL = 1;
            LCD_Buf[20] |= 0x01;         //s22 = 1
            LCD_Buf[1]  |= 0x01;         //s23 = 1
            dpParam.u8Time[0] = ptime[1];   //显示分钟
            dpParam.u8Time[1] = ptime[2];   //显示小时
            break;
        case    ZONE4_STAT_DIY_SELECT:
            if(diynum>0)
            {
                if(diynum&0x80)
                {
                    g_u8LCDVal[1] |= 0x0f;  //porridge
                }
                else
                {
                    g_u8LCDVal[1] |= 0x1f;  
                }
                if(diynum==1) bLCD_BNK_DIYNum1 = 1;
                if(diynum==2) bLCD_BNK_DIYNum2 = 1;
                if(diynum==3) bLCD_BNK_DIYNum3 = 1;
                if(diynum==4) bLCD_BNK_DIYNum4 = 1;
                if(diynum==5) bLCD_BNK_DIYNum5 = 1;
                if(diynum==0x81) bLCD_BNK_DIYNum1 = 1;  //porridge 4-6months
                if(diynum==0x82) bLCD_BNK_DIYNum2 = 1;  //porridge 7-9months
                if(diynum==0x83) bLCD_BNK_DIYNum3 = 1;  //porridge 10-12months
                if(diynum==0x84) bLCD_BNK_DIYNum4 = 1;  //porridge 13-14months
            }
            break;
       case     ZONE4_STAT_TEMP_SELECT:
           bLCD_TempIcon = 1;
           bLCD_BNK_TempIcon = 1;
           bLCD_TempNum = 1;
           bLCD_BNK_TempNum = 1;
           LCD_Buf[21] |= 0x01;    //s21 = 1
           dpParam.u8Temp = temp;  //显示温度
           bLCD_TimeIcon = 1;
           bLCD_TimeNum = 1;
           bLCD_COL = 1;
           LCD_Buf[20] |= 0x01;         //s22 = 1
           LCD_Buf[1]  |= 0x01;         //s23 = 1
           dpParam.u8Time[0] = ptime[1];   //显示分钟
           dpParam.u8Time[1] = ptime[2];   //显示小时     
           break;
        case    ZONE4_STAT_TIME_SELECT:
            if((temp>0)&&(bBookTime_EN==0)) //温度可设置且无预约
            {
                bLCD_TempIcon = 1;
                bLCD_TempNum = 1;
                LCD_Buf[21] |= 0x01;    //s21 = 1
                dpParam.u8Temp = temp;  //显示温度
            }
            bLCD_TimeIcon = 1;
            bLCD_BNK_TimeIcon = 1;
            bLCD_TimeNum = 1;
            bLCD_BNK_TimeNum = 1;
            bLCD_COL = 1;
            LCD_Buf[20] |= 0x01;        //s22 = 1
            LCD_Buf[1] |= 0x01;         //s23 = 1
            dpParam.u8Time[0] = ptime[1];   //显示分钟
            dpParam.u8Time[1] = ptime[2];   //显示小时   
            break;
        case    ZONE4_STAT_AUTO_CONFIRM:
			bLCD_TimeNum = 1;
			dpParam.u8Time[0] = 0xff;       //display"auto"
            dpParam.u8Time[1] = 0xff;    
            break;
        case    ZONE4_STAT_DEBUG:
            #ifdef  DEBUG0
            bLCD_TempNum = 1;
            dpParam.u8Temp = temp;  //显示温度
            bLCD_COL = 1;
            bLCD_BNK_COL = 1;
            bLCD_TimeNum = 1;
            dpParam.u8Time[0] = ptime[1];   //显示分钟
            dpParam.u8Time[1] = ptime[2];   //显示小时  
            #endif
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      LCD动态显示图标                                       				   *
 *  Parameters:                                                                *
 *      显示模式                                                               *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	PlayGif(uint16 code *pGif,uint16 speed,uint8 stat)
{
	m_u16pGif = m_u16pGifBak = pGif;

    m_u8Framestat = stat;
    
    if(pGif==NULL)      //清除GIF显示
    {
        LCD_Buf[8] &= Bin(10010000);
        LCD_Buf[7] &= Bin(00000000);
        LCD_Buf[6] &= Bin(10000011);
        LCD_Buf[5] &= Bin(11111110);
    }
    
	if(speed<100)
	{
		u8GifSpeed = 12;
	}
	else
	{
		if(speed>1000)
		{
			u8GifSpeed = 125;
		}
		else
		{
			u8GifSpeed = speed/8;
		}
	}	
	u8GifCnt = u8GifSpeed-1;
}

void	LCDDispGif(void)
{
	uint16 tmp=0;
	
	if(m_u16pGif!=NULL)
	{
		u8GifCnt++;

		if(u8GifCnt>=u8GifSpeed)
		{
			u8GifCnt = 0;
			tmp = *m_u16pGif++;
			if(tmp==0)
			{
				m_u16pGif = NULL;
				return;
			}
			else
			{
				if(tmp==0xffff)
				{
					m_u16pGif = m_u16pGifBak;
					u8GifCnt = u8GifSpeed-1;
				}
				else
				{
					if(TstBit(tmp,0))   SetBit(LCD_Buf[8], 1); else ClrBit(LCD_Buf[8], 1); //T3
					if(TstBit(tmp,1))   SetBit(LCD_Buf[8], 3); else ClrBit(LCD_Buf[8], 3); //T4
					if(TstBit(tmp,2))   SetBit(LCD_Buf[8], 5); else ClrBit(LCD_Buf[8], 5); //T5
					if(TstBit(tmp,3))   SetBit(LCD_Buf[8], 2); else ClrBit(LCD_Buf[8], 2); //T6
					if(TstBit(tmp,4))   SetBit(LCD_Buf[8], 6); else ClrBit(LCD_Buf[8], 6); //T7
					if(TstBit(tmp,5))   SetBit(LCD_Buf[8], 0); else ClrBit(LCD_Buf[8], 0); //T8
					if(TstBit(tmp,6))   
                    {
                        SetBit(LCD_Buf[7], 0);  //T9
                        SetBit(LCD_Buf[5], 0);  //T20
                    }
                    else 
                    {
                        ClrBit(LCD_Buf[7], 0);  //T9
                        ClrBit(LCD_Buf[5], 0);  //T20
                    }
                    if(TstBit(tmp,7))   
                    {
                        SetBit(LCD_Buf[7], 5);  //T10 
                        SetBit(LCD_Buf[6], 4);  //T21
                        SetBit(LCD_Buf[6], 5);  //T22
                    }
                    else 
                    {
                        ClrBit(LCD_Buf[7], 5);  //T10
                        ClrBit(LCD_Buf[6], 4);  //T21
                        ClrBit(LCD_Buf[6], 5);  //T22
                    }
                    if(TstBit(tmp,8))   
                    {
                        SetBit(LCD_Buf[7], 4);  //T11
                        SetBit(LCD_Buf[6], 4);  //T21
                        SetBit(LCD_Buf[6], 5);  //T22
                    }
                    else 
                    {
                        ClrBit(LCD_Buf[7], 4);  //T11
                        if(TstBit(tmp,7))  
                        {
                            SetBit(LCD_Buf[6], 4);  //T21
                            SetBit(LCD_Buf[6], 5);  //T22
                        }
                        else
                        {
                            ClrBit(LCD_Buf[6], 4);  //T21
                            ClrBit(LCD_Buf[6], 5);  //T22
                        }
                    }
                    if(TstBit(tmp,9))   
                    {
                        SetBit(LCD_Buf[7], 6);  //T12
                        SetBit(LCD_Buf[6], 2);  //T23
                    }
                    else 
                    {
                        ClrBit(LCD_Buf[7], 6);  //T12
                        ClrBit(LCD_Buf[6], 2);  //T23
                    }
                    if(TstBit(tmp,10))  SetBit(LCD_Buf[7], 7); else ClrBit(LCD_Buf[7], 7); //T13
                    if(TstBit(tmp,11))  SetBit(LCD_Buf[7], 1); else ClrBit(LCD_Buf[7], 1); //T14
                    if(TstBit(tmp,12))  SetBit(LCD_Buf[6], 3); else ClrBit(LCD_Buf[6], 3); //T15
                    if(TstBit(tmp,13))  SetBit(LCD_Buf[7], 2); else ClrBit(LCD_Buf[7], 2); //T16
                    if(TstBit(tmp,14))  SetBit(LCD_Buf[7], 3); else ClrBit(LCD_Buf[7], 3); //T17
                    if(TstBit(tmp,15))  SetBit(LCD_Buf[6], 6); else ClrBit(LCD_Buf[6], 6); //T18
				}
			}
		}
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      煲体周围的旋转方框                                         			   *
 *  Parameters:                                                                *
 *      sequence                                                               *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void  	LCDDispMovie(uint8 stat)
{
    static uint8 sequence = 0;

    if(stat==0)         //清除与动画无关的部分
    {
        LCD_Buf[5] &= Bin(00000001);
        LCD_Buf[6] &= Bin(01111101);
        return;
    }
    else
    {
        if(stat==1)     //清除
        {
            sequence = 0;
        }
        else
        {
            if(stat==2) //旋转
            {
                if(u8lcdcnt==15)
                {
                    sequence++;
                    if(sequence>11) sequence = 1;
                }
            }
            else        //全显
            {
                sequence = 0xff;
            }
        }
    }

    LCD_Buf[5]	|= Bin(11011110);   //light on  circular frame
    LCD_Buf[6]  |= Bin(10100010);
    LCD_Buf[7]  |= Bin(01000001);
    
	switch(sequence)
	{
		case	0:                  //Clear circular frame
			LCD_Buf[5]	&= Bin(00000001);
            LCD_Buf[6]  &= Bin(01011100);
            LCD_Buf[7]  &= Bin(10111110);
			break;
		case	1:		
            LCD_Buf[6]  &= Bin(11011111);   //CLR T22
			break;
		case	2:
            LCD_Buf[7]  &= Bin(10111111);   //CLR T12
			break;
		case	3:
            LCD_Buf[6]  &= Bin(01111111);   //CLR R8
			break;
		case	4:
            LCD_Buf[6]  &= Bin(11111101);   //CLR R7
			break;
		case	5:
            LCD_Buf[5]  &= Bin(01111111);   //CLR R6
			break;
		case	6:
            LCD_Buf[5]  &= Bin(10111111);   //CLR R5
			break;
		case	7:
			LCD_Buf[5]  &= Bin(11101111);   //CLR R4
			break;
		case	8:
			LCD_Buf[5]  &= Bin(11110111);   //CLR R3
			break;
		case	9:
            LCD_Buf[5]  &= Bin(11111011);   //CLR R2
			break;
		case	10:
            LCD_Buf[5]  &= Bin(11111101);   //CLR R1
			break;
		case	11:
			LCD_Buf[7]  &= Bin(11111110);   //CLR T9
			break;
		default:
			break;
	}
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      按步骤显示自检内容                                        			   *
 *  Parameters:                                                                *
 *      自检步骤                                                               *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
 void    LCDDispSelfChk(uint8 step)
{
    uint8	u8BotDegree	=	ADCGetData(AD_CH_BOT);
	uint8   u8TopDegree =   ADCGetData(AD_CH_TOP);
    uint8   i;
    
    LCDDispAll(0);                      //清除所有显示
    switch(step)
    {
        case    0:
            LCDDispNum_HI(0x23);	 	//显示序列号--Plug in & Press"start"
			LCDDispNum_LO(0x01);    
			ClrLedBlank(BIT_LED_ALL);
            break;
        case    1:
            LCDDispAll(1);              //LCD全显--Press">"
            break;
        case    2:                      //显示"b1b1"--Press"Reheat"
        case    3:                      //显示"b2b2"--Press"Keepwarm"
        case    4:                      //显示"b3b3"--Press"Delay"
        case    5:                      //显示"b4b4"--Press"Stop"
        case    6:                      //显示"b5b5"--Press"+"
        case    7:                      //显示"b6b6"--Press"Start"
        case    8:                      //显示"b7b7"--Press"-"
        case    9:                      //显示"b8b8"--Press"Cancel"
	    case    10:                     //显示"b9b9"--Press"My Recipe"
	    case    11:                     //显示"baba"--Press"Timer/Temperature"
        case    12:                     //显示"bbbb"--Press"Menu"   
			ClrLedBlank(BIT_LED_ALL);
		    if(step==2) LedOn(BIT_LED_REHEAT);
		    if(step==3) LedOn(BIT_LED_WARM);
		    if(step==4) LedOn(BIT_LED_DELAY);
		    if(step==5) LedOn(BIT_LED_STOP);
		    if(step==7) LedOn(BIT_LED_START);
            i = (step-1)&0x0f;
            i |= 0xb0;
            LCDDispNum_HI(i);
            LCDDispNum_LO(i);        
            break;    
        case    13:
            LCDDispCookTemp(u8BotDegree,1);   //显示"temperature of bottom NTC"--Press"Menu"
            break;
        case    14:
            LCDDispCookTemp(u8TopDegree,1);   //显示"temperature of top NTC"--Press"Menu"
            break;
        case    15:
            LCDDispNum_LO(0xC1);        //显示"U1"--Press"Menu"
            break;
        case    16:
            LCDDispNum_LO(0xC2);        //显示"U2"--Press"Menu"
            break;
        case    17:
            LCDDispNum_LO(0xC3);        //显示"U3"--Press"Menu"
            break;
        case    18:
            LCDDispNum_HI(0x00);        //显示"OOFF"--Press"Menu"
            LCDDispNum_LO(0xFF);
			ClrLedBlank(BIT_LED_ALL|BIT_LED_BKL);
            break;
    }
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      LCD处理(功能菜单闪烁控制&&系统状态控制)                                *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void    LCDProc(uint8 sysid)
{
    uint8 i,j;
    
	u8BlankCnt++;

	if(u8BlankCnt >= 63)
	{
		if(u8BlankCnt >= 126)
		{
			u8BlankCnt	=	0;
		}    
        i =	g_u8LCDVal[0];
        j = g_u8LCDVal[1];
	}
    else
    {
        i = g_u8LCDVal[0]^g_u8BlankVal[0];
        j = g_u8LCDVal[1]^g_u8BlankVal[1];
    }

    if(TstBit(i, 0)) LCD_Buf[3] |= (1<<6); else LCD_Buf[3] &= ~(1<<6);                  //HealthIcon
    if(TstBit(i, 1)) LCD_Buf[3] |= (1<<5); else LCD_Buf[3] &= ~(1<<5);                  //ClassicIcon         
    if(TstBit(i, 2)) LCDDispMenuNum(dpParam.u8Menu,1); else LCDDispMenuNum(0,0);        //MenuNum
    if(TstBit(i, 3)) LCDDispTexture(dpParam.u8texture,1); else LCDDispTexture(0,0);     //Texture
    if(TstBit(i, 4)) LCD_Buf[4] |= (1<<1); else LCD_Buf[4] &= ~(1<<1);                  //PeopleIcon
    if(TstBit(i, 5)) LCDDispPeopleNum(dpParam.u8People,1); else LCDDispPeopleNum(0,0);  //PeopleNum
    if(TstBit(i, 6)) LCD_Buf[2] |= (1<<5); else LCD_Buf[2] &= ~(1<<5);                  //TempIcon
    if(TstBit(i, 7)) LCD_Buf[2] |= (1<<6); else LCD_Buf[2] &= ~(1<<6);                  //TimeIcon

    if(TstBit(j, 0)) LCD_Buf[2] |= (1<<0); else LCD_Buf[2] &= ~(1<<0);                  //DIYNum1
    if(TstBit(j, 1)) LCD_Buf[2] |= (1<<1); else LCD_Buf[2] &= ~(1<<1);                  //DIYNum2
    if(TstBit(j, 2)) LCD_Buf[2] |= (1<<2); else LCD_Buf[2] &= ~(1<<2);                  //DIYNum3
    if(TstBit(j, 3)) LCD_Buf[2] |= (1<<3); else LCD_Buf[2] &= ~(1<<3);                  //DIYNum4
    if(TstBit(j, 4)) LCD_Buf[2] |= (1<<4); else LCD_Buf[2] &= ~(1<<4);                  //DIYNum5
    if(TstBit(j, 5)) LCDDispCookTemp(dpParam.u8Temp,1); else LCDDispCookTemp(0,0);      //TempNum
    if(TstBit(j, 6)) LCDDispCookTime(dpParam.u8Time,1); else LCDDispCookTime(0,0);      //TimeNum
    if(TstBit(j, 7)) LCD_Buf[19] |= (1<<0); else LCD_Buf[19] &= ~(1<<0);                //COL

    if(bKeyMusic_EN) LCD_Buf[4] &= ~(1<<7); else LCD_Buf[4] |= (1<<7);  //声音图标
    if(bSoakStage)   LCD_Buf[8] |= (1<<4); else LCD_Buf[8] &= ~(1<<4);  //吸水图标
    if(bHeatStage)   LCD_Buf[8] |= (1<<7); else LCD_Buf[8] &= ~(1<<7);  //加热图标

	LCDDispGif();					   //动画显示

    LCDDispMovie(m_u8Framestat);       //旋转方框
	
	//系统状态控制
	switch(sysid)
	{
		case	0:					//复位
			LCDDispAll(1);
			break;	
        case    5:                  //关机
            LCDDispAll(0);
            break;
        case    6:                  //报警
            LCDDispError();
            break;
        case    7:					//自检
            LCDDispSelfChk(m_u8SelfChkStep);
			break;
		default:	
			break;	
	}

    u8lcdcnt++;
    if(u8lcdcnt>15)
    {
        u8lcdcnt = 0;
        for(i=0;i<VBUF_SIZE;i++)				
	    {
		    SEG[i] = LCD_Buf[i];
	    }
    }
}
