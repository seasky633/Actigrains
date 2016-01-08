/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\HD_cfg.h"

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
#define		AD_TEMP_SIZE		3		//��ʱ���ݻ���Ĵ�С
#define     AD_CH_NUM			2		//ͨ������
#define     AD_STAT_SIZE		8		//ÿ��ͨ����ջ�Ĵ�С

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
uint16	idata	u16ADBuf[AD_CH_NUM][AD_STAT_SIZE];	//ADC������
uint16	idata	u16AdTempBuf[AD_TEMP_SIZE];		    //��ʱ���ݻ���
uint8	u8CurCH		=	0;							//��ǰͨ��
uint8	u8CurCHID;									//��ǰͨ����Ӧ��ID
uint8	u8ADCnt		=	0;							//����ת���Ĵ���
uint8	u8ChgCnt	=	0;							//ת������
uint8	idata	g_u8ADResult[AD_CH_NUM];			//ADת�����ս��(�¶�)
uint16	idata	g_u16ADValue[AD_CH_NUM];			//ADת�����ս��(AD)
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
	ADCON	&=	Bin(10111111);			//��ADC�ж������־,ADCIF=0

    if(bADOK)
    {
        //AN2��ѹ��⴦��
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
        ADCON |= Bin(00000001);			    //GO/DONE��־λ��һ����ʼ��һ��ADCת��
    }
    else
    {
	    u16AdTemp = (ADDH << 2) | ADDL;		//read ADC
        u16AdTempBuf[u8ADCnt++]	= u16AdTemp;//��ת��������浽��ʱ��������
	
        if(u8ADCnt>=AD_TEMP_SIZE)           //�Ƿ������ɼ���(AD_TEMP_SIZE)��
	    {					
		    u8ADCnt  =	0;
		    bADOK	 =	1;		            //����ʱ�������Ѵ�������ʱ����ADOKλΪ1

            //�л���AN2����ѹ
            ADCH = 0x04; 
		    ADCON = Bin(10000101);      
	    }
        else
	    {
		    ADCON |= Bin(00000001);			//GO/DONE��־λ��һ����ʼ��һ��ADCת��
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
	ADCON	|=	Bin(10000000);			//��ADCģ��
	ADCON1  =   Bin(00000100);          //�ֱ���Ϊ10bit
	ADT		=	Bin(11101111);		    /* ADCʱ��Ϊϵͳʱ�ӵ�32��Ƶ��T(ad) = 32*(1/8M) = 4us
										ADC����ʱ��Ϊ15��ADCʱ�ӣ�T(s)= (14+1)*T(ad) = 60us
										ADCת��ʱ��Ϊ12*T(ad) + T(s) = 108us */
	u8CurCH		=   0;                  //u8CurCH : ��ǰͨ�����
	u8CurCHID	=	CHNO2ID[u8CurCH];	//u8CurCHID: ��ǰͨ��ID
	ADCSelChannel(u8CurCHID);			//ѡ��ͨ��
	ADCON	|=  Bin(00000001);			//��ʼADCת��
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
	ADCH	=	Bin(00000000);			//�л�ǰ����ΪIO״̬
	ADCON	&=	Bin(11110001);			//ѡ��ͨ��֮ǰ�����
	ADCON	|=  u8CH;
	
	if(u8CH==0)	ADCH = 0x01;			//AN0
	if(u8CH==2)	ADCH = 0x02;			//AN1
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ��ѯADת�����ս��.                                                    *
 *  Parameters:                                                                *
 *      ת��ͨ��                                                               *
 *  Return                                                                     *
 *      ADת�����ս��(�¶�)                                                   *
 *----------------------------------------------------------------------------*/
uint8	ADCGetData(uint8 u8CH)
{
	return	g_u8ADResult[u8CH];	
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ��ѯADת�����ս��.                                                    *
 *  Parameters:                                                                *
 *      ת��ͨ��                                                               *
 *  Return                                                                     *
 *      ADת�����ս��(AD)                                               	   *
 *----------------------------------------------------------------------------*/
uint16	ADCGetVal(uint8	u8CH)
{
	return	g_u16ADValue[u8CH];
}

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ADֵת�����¶�ֵ������.                                                *
 *  Parameters:                                                                *
 *      ADֵ,ת��ͨ��                                                          *
 *  Return                                                                     *
 *      �¶�ֵ                                                                 *
 *----------------------------------------------------------------------------*/
static	uint8	GetDegree(uint16 adData,uint8 u8CH)
{
	uint8	i=0;
	adData	= adData^0x03ff;
    
    switch(u8CH)
	{
	    case    AD_CH_BOT:
			//�ж�AD�Ƿ��ڲ��ҷ�Χ��(0�ȵ�180��)
			if((adData>=31)&&(adData<948))
			{
				//�Էַ�������Ӧ�¶�
				i =	Dichotomy_Search((uint16*)TempTable3950_50K,181,adData);
			}
			else
			{
				if(adData<31)	 i = 0;
				if(adData>=948)  i = 181;
			}
			break;

		case	AD_CH_TOP:
			//�ж�AD�Ƿ��ڲ��ҷ�Χ��(0�ȵ�120��)
			if((adData>=27)&&(adData<784))
		    {
				//�Էַ�������Ӧ�¶�
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

/*                     ����������ֲ                    */

/*-----------------------------------------------------------------------------*
 *  Function Description:                                                      *
 *      ð������.                                                              *
 *  Parameters:                                                                *
 *      pDat��Ҫ���������ָ�룬u8Size��Ҫ����ĸ���.                          *
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
 *  	����������ֲ����㷨
 *	Parameters: 
 * 		u8Array[]:����������,  	n:����������Ԫ�ظ���,	u8Key:�����ҵĹؼ�ֵ
 * 	Return:   
 *		��Ӧ���¶�ֵ
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
		/*key>array[mid] ����Ҫ����ҵ�ֵ��[mid+1,high]*/   
		/*����,��[low,mid-1]*/   
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
 *      ADC�ɼ�����	                                                           *
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
	    ADCON = 0;      //ֹͣת��     
		bADOK=0;		//���ADOK��־λ

		//��ֵ�˲�
		Sort16(u16AdTempBuf,AD_TEMP_SIZE);
		u16AdTemp =	u16AdTempBuf[AD_TEMP_SIZE/2];
		u16ADBuf[u8CurCH][u8ChgCnt]	=	u16AdTemp;
	
        //����ƽ��ֵ�˲�
		u8CurCH++;		//ת����һ��ͨ��
		if(u8CurCH>=AD_CH_NUM)
		{
			u8CurCH	 =	0;
			u8ChgCnt++;
			if(u8ChgCnt==AD_STAT_SIZE)
			{
				u8ChgCnt = u8ChgCnt-1; 
				//�õ�AD������ֵ
				for(j=0;j<AD_CH_NUM;j++)
				{
					u16Tmp  =	0;
					for(i=0;i<AD_STAT_SIZE;i++)
					{
						u16Tmp	+=	u16ADBuf[j][i];
					}
					u16Tmp = u16Tmp/AD_STAT_SIZE;
					g_u16ADValue[j] = u16Tmp;				//(ADֵ)
					g_u8ADResult[j]	= GetDegree(u16Tmp,j);	//(�¶�ֵ)
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
		ADCSelChannel(u8CurCHID);				//ѡ��ͨ��
		ADCON	|=  Bin(10000000);				//��ADCģ��
		delay(5);
		ADCON	|=  Bin(00000001);				//��ʼת��
	}
}
