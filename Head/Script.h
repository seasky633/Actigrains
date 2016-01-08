#ifndef _SCRIPT_H
#define	_SCRIPT_H

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
#define     SOAK_TIME_RISE_S1   0
#define     SOAK_TIME_KEEP_S1   1
#define     SOAK_TEMP_KEEP_S1   2
#define     SOAK_TIME_RISE_S2   3   //Brownrice NutritiousÓÐ2¶ÎÎüË®
#define     SOAK_TIME_KEEP_S2   4
#define     SOAK_TEMP_KEEP_S2   5
#define     MASH_TIME           6
#define     COUNTDOWN_TIME      7

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
extern	void 	ScriptProc(void);
//extern	void	BoilCheckProc(void);
#ifdef	DEBUG0
extern  uint8	GetScriptStage(void);
#endif
extern	void	ResetCurScript(void);
extern	void	SetCurScript(uint8,uint8);
extern	uint8	m_u8CookMode;
extern  uint8   m_u8Feel;

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern  uint16	idata g_u16VarParam[];

#endif