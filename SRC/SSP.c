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
/* ����˵��:	����������FLASH�ռ�
/* ����˵��:	
/* ����:		��Ҫ����������ַ
/* ���:		
 *-------------------------------------------------------------------------*/
void EraseFlash(uint16 flashaddr)
{
	Bool	EA_BAK = EA;				//�����ж�״̬
	//---------------------------------------
//step 1
	EA = 0;								//�����ж�
//step 2
	XPAGE = (flashaddr>>8) & 0x07;		//ѡ������
	FLASHCON =1;                        //������EEPROM����
//step 3
	if(SSP_Flag==SSP_ERASE_FLAG)
	{	
		IB_CON1	 = 0xE6;				//����SSP����Ϊ��������
		IB_CON2	 = 0x05;				//���̿���
		IB_CON3	 = 0x0A;
		IB_CON4	 = 0x09;
		IB_CON5	 = 0x06;
//step 4
		_nop_();						//�ȶ�FLASH���
		_nop_();
		_nop_();
		_nop_();
	}
	else
	{
		IB_CON2	 = 0x00;                //��ֹFlash���
	}
//step 5
	SSP_Flag  = 0;		
	FLASHCON  = 0;
	XPAGE	  = 0;
	//---------------------------------------
	EA = EA_BAK;						//�ָ��ж�״̬
}

/*--------------------------------------------------------------------------*
/* 			void WriteFlash(uint16 flashaddr,uint8 *ptr);
/* ����˵��:	д���ݵ�FLASH
/* ����˵��:	
/* ����:	    д���FLASH��ַ ������ָ��
/* ���:		
 *-------------------------------------------------------------------------*/
void WriteFlash(uint16 flashaddr,uint8 *ptr)
{
	uint8 	i;
	Bool	EA_BAK = EA;				//�����ж�״̬
//step 1
	EA = 0;								//�����ж�
//step 2
	XPAGE 	 = (flashaddr>>8) & 0x07;	//ѡ������
	IB_OFFSET= flashaddr&0xff;			//����ROM��ַ
	FLASHCON =1;                        //������EEPROM����
//step 3
	IB_DATA	 = RECORD_NOEMPTY;			//��������
//step 4
	if(SSP_Flag==SSP_WRITE_FLAG)
	{
		IB_CON1	 = 0x6E;				//����Ϊ��̲���
		IB_CON2	 = 0x05;				//���̿���
		IB_CON3	 = 0x0A;
		IB_CON4	 = 0x09;
		IB_CON5	 = 0x06;
//step 5
		_nop_();						//�ȶ�FLASH���
		_nop_();
		_nop_();
		_nop_();
//step 6
		flashaddr ++;
		for(i=1;i<=FLASH_RECORD_BYTES;i++)	//�ظ�����2
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
		IB_CON2	 = 0x00;                //��ֹFlash���
	}
//step 7
	SSP_Flag  = 0;		
	FLASHCON  = 0;
	XPAGE	  = 0;
	//---------------------------------------
	EA = EA_BAK;						//�ָ��ж�״̬
}

/*--------------------------------------------------------------------------*
/* 			void ReadFlash(uint16 flashaddr,uint8 *ptr)
/* ����˵��:	��FLASH ROM����
/* ����˵��:	
/* ����:		ROM��ַ ������ָ��
/* ���:		
 *-------------------------------------------------------------------------*/
void ReadFlash(uint16 flashaddr,uint8 *ptr)
{
	uint8 i;

	FLASHCON  = 1;								//������EEPROM����
	for(i=0;i<=FLASH_RECORD_BYTES;i++)			//��ȡ(FLASH_RECORD_BYTES+1)���ֽ�����
	{
		*ptr = *(uint8 code *)(flashaddr);
		ptr ++;
		flashaddr ++;
	}
	FLASHCON  = 0;	
}




