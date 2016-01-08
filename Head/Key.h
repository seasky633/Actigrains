#ifndef _KEY_H_
#define	_KEY_H_

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

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
extern  void	KeyProc(void);
extern	uint16  GetKey(void);
extern	uint8  	IsNewKey(void);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern	uint16	g_u16KeyRpyCnt;
extern  uint16	m_u16KeyVal;
extern  uint16  g_u16KeyUpAct;
extern  Bool bLidOCDEn;

#endif

