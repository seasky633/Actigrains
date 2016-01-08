#ifndef _AD_H_
#define	_AD_H_

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
//PIN22   AN0/P4.0   AD-RT-BOT	 Bottom Temperature
//PIN21   AN1/P4.1   AD-RT-TOP   TOP Temperature
//PIN20   AN2/P4.2   AD-POWER_IN 12V Power input

#define	 AD_CH_BOT	0
#define  AD_CH_TOP	1

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
extern	uint8	ADCGetData(uint8 u8CH);
extern	uint16	ADCGetVal(uint8 u8CH);
//extern	void	Sort8(uint8	*pDat,uint8	u8Size);
extern	void	ADC_INIT(void);
extern	void	ADCProc(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern	code	uint16	TempTable3950_50K[];
extern  code    uint16  TempTable4200_103K[];

#endif


