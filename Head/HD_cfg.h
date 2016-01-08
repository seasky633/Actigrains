#ifndef _HD_CFG_H_
#define _HD_CFG_H_

/*-----------------------------*
 *  include head files         *
 *----------------------------*/
#include ".\Head\SH79F3283.h" 
#include <INTRINS.H>
#include ".\Head\Datatype.h"
#include ".\Head\CommDef.h"
#include ".\Head\SSP.h"
#include ".\Head\AD.h"
#include ".\Head\LCD.h"
#include ".\Head\LED.h"
#include ".\Head\IIC.h"
#include ".\Head\Key.h"
#include ".\Head\Buzzer.h"

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
extern	void	System_INIT(void);
extern 	void 	delay(uint16);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
extern  uint16 xdata   g_u16RelayDelayCnt;
#endif
