#ifndef _LED_H_
#define	_LED_H_

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
//#define     IO_LED_START       U202_PC7
//#define     IO_LED_STOP        U202_PA1        
//#define     IO_LED_REHEAT      U202_PA7
//#define     IO_LED_WARM        U202_PA3
//#define     IO_LED_DELAY       U202_PA4    

#define     BIT_LED_STOP 	    (1<<0)
#define     BIT_LED_DELAY       (1<<1)
#define     BIT_LED_WARM        (1<<2)
#define     BIT_LED_REHEAT      (1<<3)
#define     BIT_LED_START		(1<<4)
#define     BIT_LED_BKL         (1<<5)
#define		BIT_LED_ALL			0x1f


#define     NUM_LED_STOP        0
#define     NUM_LED_DELAY       1
#define     NUM_LED_WARM        2
#define     NUM_LED_REHEAT      3
#define     NUM_LED_START       4
#define     NUM_LED_BKL         5

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
extern	void	SetLedBlank(uint8);
extern	void	ClrLedBlank(uint8);
extern	void	LedOn(uint8);
extern  void	LedProc(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/	
extern uint8 xdata g_u8LedTxDAT1;
extern uint8 xdata g_u8LedTxDAT2;
#endif
