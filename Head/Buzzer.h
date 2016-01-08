#ifndef _BUZZER_H
#define	_BUZZER_H

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
//打开时间
#define		M_0			0
#define		M_80		1
#define 	M_200       2
#define 	M_500       3
#define 	M_1000      4
#define 	M_2000      5

//关闭时间
#define 	S_0         (0<<4)
#define		S_80		(1<<4)
#define		S_200		(2<<4)
#define		S_500		(3<<4)
#define		S_1000		(4<<4)
#define 	S_2000      (5<<4)

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
extern	void	SoundProc(void);
extern	void	PlaySound(uint8	code *pMusic);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern  uint8   code	MusicPoweron[];
extern  uint8   code	MusicKeyDown[];
extern  uint8   code	MusicSysError[];
extern  uint8   code	MusicCookEnd[];
extern	uint8   code    MusicCountDown[];
extern	uint8   code    MusicInputFood[];
extern	uint8   code	MusicCycleEnd[];
extern  uint8   code    MusicOFF[];
extern	uint8   code    MusicAchievedTemp[];
#endif
