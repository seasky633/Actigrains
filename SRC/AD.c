/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
#define		AD_TEMP_SIZE		3		//临时数据缓冲的大小
#define     AD_CH_NUM			2		//通道个数
#define     AD_STAT_SIZE		8		//每个通道堆栈的大小

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
void	ADC_INIT(void);
uint8	ADCGetData(uint8 u8CH);
uint16	ADCGetVal(uint8 u8CH);
//void	Sort8(uint8	*pDat,uint8	u8Size);
void	Sort16(uint16 *pDat,uint8 u8Size);
void	ADCProc(void);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/
static	void	ADCSelChannel(uint8  u8Ch);							
static	uint8	GetDegree(uint16 adData,uint8 u8CH);
static	uint8	Dichotomy_Search(uint16 u8Array[],uint8 n,uint16 u8Key);  

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
uint16	idata	u16ADBuf[AD_CH_NUM][AD_STAT_SIZE];	//ADC缓冲器
uint16	idata	u16AdTempBuf[AD_TEMP_SIZE];		    //临时数据缓冲
uint8	u8CurCH		=	0;							//当前通道
uint8	u8CurCHID;									//当前通道对应的ID
uint8	u8ADCnt		=	0;							//连续转换的次数
uint8	u8ChgCnt	=	0;							//转换次数
uint8	idata	g_u8ADResult[AD_CH_NUM];			//AD转换最终结果(温度)
uint16	idata	g_u16ADValue[AD_CH_NUM];			//AD转换最终结果(AD)
uint8   code	CHNO2ID[]	=	{0,2};  
uint16  idata   u16AdVBuf;
uint16  idata   u16AdVValue;
uint8   idata   u8AdVCnt;
/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/


/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      AD convert complete interrupt handler.                                 *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	ADC_ISP(void) interrupt 6
{
	uint16	u16AdTemp;
    uint8   u8Bankbak = INSCON;
    
	Enter_Bank0;
	EA	=	0;
	ADCON	&=	Bin(10111111);			//清ADC中断请求标志,ADCIF=0

    if(bADOK)
    {
        //AN2电压检测处理
        u16AdTemp = (ADDH << 2) | ADDL;		//read ADC
        u16AdVBuf += u16AdTemp;
        if(++u8AdVCnt>=16)
        {
            u8AdVCnt = 0;
            u16AdVValue = u16AdVBuf>>4;    
			u16AdVBuf = 0;
            if(u16AdVValue<750)     //9v
            {
                bPwroffline = 0;    //power down
            }
            else
            {
                bPwroffline = 0;
            }
        }
        ADCON |= Bin(00000001);			    //GO/DONE标志位置一，开始下一次ADC转换
    }
    else
    {
	    u16AdTemp = (ADDH << 2) | ADDL;		//read ADC
        u16AdTempBuf[u8ADCnt++]	= u16AdTemp;//将转换结果保存到临时缓冲区中
	
        if(u8ADCnt>=AD_TEMP_SIZE)           //是否连续采集了(AD_TEMP_SIZE)次
	    {					
		    u8ADCnt  =	0;
		    bADOK	 =	1;		            //当临时缓冲区已存满数据时，置ADOK位为1

            //切换到AN2检测电压
            ADCH = 0x04; 
		    ADCON = Bin(10000101);      
	    }
        else
	    {
		    ADCON |= Bin(00000001);			//GO/DONE标志位置一，开始下一次ADC转换
	    }
    }
	
	EA	=	1;
    INSCON = u8Bankbak;
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Initialize the ADC.                                                    *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	ADC_INIT(void)
{
	ADCON	|=	Bin(10000000);			//打开ADC模块
	ADCON1  =   Bin(00000100);          //分辨率为10bit
	ADT		=	Bin(11101111);		    /* ADC时钟为系统时钟的32分频，T(ad) = 32*(1/8M) = 4us
										ADC采样时间为15个ADC时钟，T(s)= (14+1)*T(ad) = 60us
										ADC转换时间为12*T(ad) + T(s) = 108us */
	u8CurCH		=   0;                  //u8CurCH : 当前通道序号
	u8CurCHID	=	CHNO2ID[u8CurCH];	//u8CurCHID: 当前通道ID
	ADCSelChannel(u8CurCHID);			//选择通道
	ADCON	|=  Bin(00000001);			//开始ADC转换
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      Select ADC Channel.                                                    *
 *  Parameters:                                                                *
 *      u8Ch                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
static	void	ADCSelChannel(uint8  u8CH)
{
	ADCH	=	Bin(00000000);			//切换前设置为IO状态
	ADCON	&=	Bin(11110001);			//选择通道之前先清除
	ADCON	|=  u8CH;
	
	if(u8CH==0)	ADCH = 0x01;			//AN0
	if(u8CH==2)	ADCH = 0x02;			//AN1
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      查询AD转换最终结果.                                                    *
 *  Parameters:                                                                *
 *      转换通道                                                               *
 *  Return                                                                     *
 *      AD转换最终结果(温度)                                                   *
 *----------------------------------------------------------------------------*/
uint8	ADCGetData(uint8 u8CH)
{
	return	g_u8ADResult[u8CH];	
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      查询AD转换最终结果.                                                    *
 *  Parameters:                                                                *
 *      转换通道                                                               *
 *  Return                                                                     *
 *      AD转换最终结果(AD)                                               	   *
 *----------------------------------------------------------------------------*/
uint16	ADCGetVal(uint8	u8CH)
{
	return	g_u16ADValue[u8CH];
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      AD值转换成温度值并处理.                                                *
 *  Parameters:                                                                *
 *      AD值,转换通道                                                          *
 *  Return                                                                     *
 *      温度值                                                                 *
 *----------------------------------------------------------------------------*/
static	uint8	GetDegree(uint16 adData,uint8 u8CH)
{
	uint8	i=0;
	adData	= adData^0x03ff;
    
    switch(u8CH)
	{
	    case    AD_CH_BOT:
			//判断AD是否在查找范围内(0度到180度)
			if((adData>=31)&&(adData<948))
			{
				//对分法查找相应温度
				i =	Dichotomy_Search((uint16*)TempTable3950_50K,181,adData);
			}
			else
			{
				if(adData<31)	 i = 0;
				if(adData>=948)  i = 181;
			}
			break;

		case	AD_CH_TOP:
			//判断AD是否在查找范围内(0度到120度)
			if((adData>=27)&&(adData<784))
		    {
				//对分法查找相应温度
				i =	Dichotomy_Search((uint16*)TempTable4200_103K,121,adData);
			}
			else
			{
				if(adData<27)	 i = 0;
				if(adData>=784)  i = 121;
			}
			
		default:
		    break;
	}

	return	i;
}

/*                     下面程序可移植                    */

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      冒泡排序.                                                              *
 *  Parameters:                                                                *
 *      pDat需要排序的数据指针，u8Size需要排序的个数.                          *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
//void	Sort8(uint8	*pDat,uint8	u8Size)
//{
//	uint8	i,j,temp,tag=1;

//	for(i = 0; tag==1&&i<u8Size-1; i++)
//	{
//		tag = 0;
//		for(j=0; j<u8Size-i-1; j++)
//		{
//			if(pDat[j]>pDat[j+1])
//			{
//				temp	=	pDat[j];
//				pDat[j]  =  pDat[j+1];
//				pDat[j+1] = temp;	
//				tag = 1;				
//			}
//		}
//	}
//}

void	Sort16(uint16 *pDat,uint8 u8Size)
{
	uint8	i,j,tag=1;
	uint16	temp;

	for(i = 0; tag==1&&i<u8Size-1; i++)
	{
		tag = 0;
		for(j=0; j<u8Size-i-1; j++)
		{
			if(pDat[j]>pDat[j+1])
			{
				temp	=	pDat[j];
				pDat[j]  =  pDat[j+1];
				pDat[j+1] = temp;	
				tag = 1;				
			}
		}
	}
}

/*-----------------------------------------------------------------------------* 				                         
 *  Function Description:                                                                    
 *  	有序数组二分查找算法
 *	Parameters: 
 * 		u8Array[]:被查找数组,  	n:被查找数组元素个数,	u8Key:被查找的关键值
 * 	Return:   
 *		对应的温度值
 *----------------------------------------------------------------------------*/
static	uint8	Dichotomy_Search(uint16 u8Array[],uint8 n,uint16 u8Key)    
{    
    uint8 		u8Low,u8High,u8Mid;    
    u8Low = 0;    
    u8High = n - 1;    
	
    while(1) {    
		u8Mid = (u8High+u8Low)/2;    
		if(u8Key>=u8Array[u8Mid]){    
		   if(u8Mid==u8High||u8Key < u8Array[u8Mid+1])
		    return(u8Mid);    
		}
		/*key>array[mid] 表明要求查找的值在[mid+1,high]*/   
		/*否则,在[low,mid-1]*/   
		if(u8Low==u8High)break;
		if(u8Key > u8Array[u8Mid])    
		    u8Low = u8Mid + 1;    
		else   
		    u8High = u8Mid - 1;    
	}
	return(0);    
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ADC采集处理	                                                           *
 *  Parameters:                                                                *
 *      None                                                                   *
 *  Return                                                                     *
 *      None                                                                   *
 *----------------------------------------------------------------------------*/
void	ADCProc(void)
{
	uint16	u16AdTemp;
	uint8	i,j;
	uint16	u16Tmp;

	if(bADOK==1)
	{
	    ADCON = 0;      //停止转换     
		bADOK=0;		//清除ADOK标志位

		//中值滤波
		Sort16(u16AdTempBuf,AD_TEMP_SIZE);
		u16AdTemp =	u16AdTempBuf[AD_TEMP_SIZE/2];
		u16ADBuf[u8CurCH][u8ChgCnt]	=	u16AdTemp;
	
        //滑动平均值滤波
		u8CurCH++;		//转到下一个通道
		if(u8CurCH>=AD_CH_NUM)
		{
			u8CurCH	 =	0;
			u8ChgCnt++;
			if(u8ChgCnt==AD_STAT_SIZE)
			{
				u8ChgCnt = u8ChgCnt-1; 
				//得到AD的最终值
				for(j=0;j<AD_CH_NUM;j++)
				{
					u16Tmp  =	0;
					for(i=0;i<AD_STAT_SIZE;i++)
					{
						u16Tmp	+=	u16ADBuf[j][i];
					}
					u16Tmp = u16Tmp/AD_STAT_SIZE;
					g_u16ADValue[j] = u16Tmp;				//(AD值)
					g_u8ADResult[j]	= GetDegree(u16Tmp,j);	//(温度值)
				}
								
				for(j=0;j<AD_CH_NUM;j++)
				{
					for(i=0;i<AD_STAT_SIZE-1;i++)
					{
						u16ADBuf[j][i]	=	u16ADBuf[j][i+1];
					}
				} 
			}
		}

		u8CurCHID =	CHNO2ID[u8CurCH];
		ADCSelChannel(u8CurCHID);				//选择通道
		ADCON	|=  Bin(10000000);				//打开ADC模块
		delay(5);
		ADCON	|=  Bin(00000001);				//开始转换
	}
}
