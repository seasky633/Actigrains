#ifndef _IIC_H_
#define	_IIC_H_

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/
#define	 Sda           P4_3
#define	 Sda_High()	   {SetBit(P4CR,3);Sda=1;}
#define	 Sda_Low()     {SetBit(P4CR,3);Sda=0;}
#define  Sdac_High()   {ClrBit(P4CR,3);}

#define	 Scl           P4_4
#define	 Scl_High()	   {SetBit(P4CR,4);Scl=1;}
#define	 Scl_Low()     {SetBit(P4CR,4);Scl=0;}
#define  iic_Pull()    {SetBit(P4PCR,3);SetBit(P4PCR,4);}

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
extern void i2c_read();
extern void delays(uint8 tms);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/	
extern   xdata uint8 rdat1,rdat2;
#endif
