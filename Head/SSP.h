#ifndef _SSP_H_
#define	_SSP_H_

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
#define		RECORD_NOEMPTY		0x5A
#define     SSP_WRITE_FLAG      0x5A
#define     SSP_ERASE_FLAG      0xA5
#define		FLASH_RECORD_BYTES	40


#define SSP_DIY1_TM1            0
#define SSP_DIY1_TM2            1
#define SSP_DIY1_TMP            2
#define SSP_DIY1_COOKMODE       3
#define SSP_DIY1_TEXTURE        4
#define SSP_DIY1_PEOPLE         5
#define SSP_DIY1_AGE            6

#define SSP_DIY2_TM1            7
#define SSP_DIY2_TM2            8
#define SSP_DIY2_TMP            9
#define SSP_DIY2_COOKMODE       10
#define SSP_DIY2_TEXTURE        11
#define SSP_DIY2_PEOPLE         12
#define SSP_DIY2_AGE            13

#define SSP_DIY3_TM1            14
#define SSP_DIY3_TM2            15
#define SSP_DIY3_TMP            16
#define SSP_DIY3_COOKMODE       17
#define SSP_DIY3_TEXTURE        18
#define SSP_DIY3_PEOPLE         19
#define SSP_DIY3_AGE            20

#define SSP_DIY4_TM1            21
#define SSP_DIY4_TM2            22
#define SSP_DIY4_TMP            23
#define SSP_DIY4_COOKMODE       24
#define SSP_DIY4_TEXTURE        25
#define SSP_DIY4_PEOPLE         26
#define SSP_DIY4_AGE            27

#define SSP_DIY5_TM1            28
#define SSP_DIY5_TM2            29
#define SSP_DIY5_TMP            30
#define SSP_DIY5_COOKMODE       31
#define SSP_DIY5_TEXTURE        32
#define SSP_DIY5_PEOPLE         33
#define SSP_DIY5_AGE            34

#define SSP_LASTMENU            35
#define SSP_LASTDIY             36
#define SSP_LASTAGE             37
#define SSP_MUSIC_EN            38

/*-----------------------------*
 *  Macros for others          *
 *----------------------------*/

/*-----------------------------*
 *  type definitions           *
 *----------------------------*/

/*-----------------------------*
 *  structure,union and enum   *
 *----------------------------*/
typedef struct 
{
  uint8	RecordFlag;				   	//RecordFlag == 0x5A ==> Record is empty;RecordFlag !=0x5A ==> Record is not empty*/
  uint8	ROM[FLASH_RECORD_BYTES];  	
}FLASH_MEMORY;

/*-----------------------------*
 *  public functions prototypes*
 *----------------------------*/
extern	void ReadFlash(uint16 flashaddr,uint8 *ptr);
extern	void WriteFlash(uint16 flashaddr,uint8 *ptr);
extern  void EraseFlash(uint16 flashaddr);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern FLASH_MEMORY xdata 	Flash; 
extern uint8 idata SSP_Flag;

#endif