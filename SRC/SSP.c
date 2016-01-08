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
void ReadFlash(uint16 flashaddr,uint8 *ptr);
void WriteFlash(uint16 flashaddr,uint8 *ptr);
void EraseFlash(uint16 flashaddr);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/


/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
FLASH_MEMORY xdata	Flash; 	
uint8	idata SSP_Flag;

/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/


/*--------------------------------------------------------------------------*
/* 			void EraseFlash(uint16 flashaddr)
/* 功能说明:	擦除扇区的FLASH空间
/* 其他说明:	
/* 输入:		需要擦除扇区地址
/* 输出:		
 *-------------------------------------------------------------------------*/
void EraseFlash(uint16 flashaddr)
{
	Bool	EA_BAK = EA;				//保存中断状态
	//---------------------------------------
//step 1
	EA = 0;								//禁用中断
//step 2
	XPAGE = (flashaddr>>8) & 0x07;		//选择扇区
	FLASHCON =1;                        //访问类EEPROM区域
//step 3
	if(SSP_Flag==SSP_ERASE_FLAG)
	{	
		IB_CON1	 = 0xE6;				//设置SSP操作为擦除操作
		IB_CON2	 = 0x05;				//流程控制
		IB_CON3	 = 0x0A;
		IB_CON4	 = 0x09;
		IB_CON5	 = 0x06;
//step 4
		_nop_();						//稳定FLASH编程
		_nop_();
		_nop_();
		_nop_();
	}
	else
	{
		IB_CON2	 = 0x00;                //终止Flash编程
	}
//step 5
	SSP_Flag  = 0;		
	FLASHCON  = 0;
	XPAGE	  = 0;
	//---------------------------------------
	EA = EA_BAK;						//恢复中断状态
}

/*--------------------------------------------------------------------------*
/* 			void WriteFlash(uint16 flashaddr,uint8 *ptr);
/* 功能说明:	写数据到FLASH
/* 其他说明:	
/* 输入:	    写入的FLASH地址 ，数据指针
/* 输出:		
 *-------------------------------------------------------------------------*/
void WriteFlash(uint16 flashaddr,uint8 *ptr)
{
	uint8 	i;
	Bool	EA_BAK = EA;				//保存中断状态
//step 1
	EA = 0;								//禁用中断
//step 2
	XPAGE 	 = (flashaddr>>8) & 0x07;	//选择扇区
	IB_OFFSET= flashaddr&0xff;			//载入ROM地址
	FLASHCON =1;                        //访问类EEPROM区域
//step 3
	IB_DATA	 = RECORD_NOEMPTY;			//载入数据
//step 4
	if(SSP_Flag==SSP_WRITE_FLAG)
	{
		IB_CON1	 = 0x6E;				//设置为编程操作
		IB_CON2	 = 0x05;				//流程控制
		IB_CON3	 = 0x0A;
		IB_CON4	 = 0x09;
		IB_CON5	 = 0x06;
//step 5
		_nop_();						//稳定FLASH编程
		_nop_();
		_nop_();
		_nop_();
//step 6
		flashaddr ++;
		for(i=1;i<=FLASH_RECORD_BYTES;i++)	//重复步骤2
		{
			XPAGE = (flashaddr>>8) & 0x07;	//step 2
			IB_OFFSET= flashaddr&0xff;
			FLASHCON =1; 
			IB_DATA	 = *(ptr+i);			//step 3
		    IB_CON1	 = 0x6E;				//step 4
		    IB_CON2	 = 0x05;
		    IB_CON3	 = 0x0A;
		    IB_CON4	 = 0x09;
		    IB_CON5	 = 0x06;
		    _nop_();						//step 5
		    _nop_();
		    _nop_();
		    _nop_();
		    flashaddr ++;				    //step 6
		}
	}
	else
	{
		IB_CON2	 = 0x00;                //终止Flash编程
	}
//step 7
	SSP_Flag  = 0;		
	FLASHCON  = 0;
	XPAGE	  = 0;
	//---------------------------------------
	EA = EA_BAK;						//恢复中断状态
}

/*--------------------------------------------------------------------------*
/* 			void ReadFlash(uint16 flashaddr,uint8 *ptr)
/* 功能说明:	读FLASH ROM数据
/* 其他说明:	
/* 输入:		ROM地址 ，数据指针
/* 输出:		
 *-------------------------------------------------------------------------*/
void ReadFlash(uint16 flashaddr,uint8 *ptr)
{
	uint8 i;

	FLASHCON  = 1;								//访问类EEPROM区域
	for(i=0;i<=FLASH_RECORD_BYTES;i++)			//读取(FLASH_RECORD_BYTES+1)个字节数据
	{
		*ptr = *(uint8 code *)(flashaddr);
		ptr ++;
		flashaddr ++;
	}
	FLASHCON  = 0;	
}




