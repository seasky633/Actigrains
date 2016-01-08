/*----------------------------*/
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
uint8 i2c_recbyte(void);
void  i2c_ack(void);
void  i2c_stop(void);
void  i2c_start(void);
void  i2c_read(void);
void  i2c_sendbyte(uint8 tms);
void  delays(uint8 tms);

/*-----------------------------*
 *  static functions prototypes*
 *----------------------------*/


/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/
Bool ack;		
uint8 xdata rdat1,rdat2;
/*-----------------------------*
 *  static variable declaration*
 *----------------------------*/

void i2c_read()
{
	iic_Pull();
    i2c_stop();
    i2c_start();
    i2c_sendbyte(0xa9);               //addr
    if (ack) 
	i2c_stop();
    else
	{
        rdat1=i2c_recbyte();           //dat1
        ack=0;
        i2c_ack();
        delays(20);
		rdat2=i2c_recbyte();           //dat2
		ack=1;
        i2c_ack();
        delays(20);
	}
	
	
	delays(100);
	i2c_stop();
    i2c_start();
    i2c_sendbyte(0xa8);               //addr
    if (ack) 
	i2c_stop();
    else
	{
//        i2c_sendbyte(g_u8LedTxDAT1);
//        if(ack)
//        i2c_stop();
//        else
//        {
//            i2c_sendbyte(g_u8LedTxDAT2);
//		    ack=1;
//            i2c_ack();
//            delays(20);
//        }
		i2c_sendbyte(g_u8LedTxDAT1);
		ack=1;
        i2c_ack();
        delays(20);
	}
}

void i2c_start()
{
    Sda_High();
    delays(15);
    Scl_High();
    delays(15);
    Sda_Low();
    delays(15);
    Scl_Low();
    delays(15);
}

void i2c_stop()
{
    Sda_Low();
    delays(15);
    Scl_High();
    delays(15);
    Sda_High();
    delays(15);
    Scl_Low();
    delays(15);
}

void i2c_ack()
{
    delays(15);
    if (ack==0)
    Sda_Low();
    if (ack==1)
    Sda_High();
    delays(15);
    Scl_High();
    delays(15);
    Scl_Low();
    delays(15);
}

void i2c_sendbyte(uint8 tms)
{
    uint8 i,sdat;
    sdat=tms;
    for (i=0;i<8;i++)
    {
	      if (sdat&0x80)
	      {
	          Sda_High();
	      }
	      else
	      {
              Sda_Low();
	      }
	      delays(15);
	      Scl_High();
	      delays(15);
	      Scl_Low();
	      sdat<<=1;
    }
	Sdac_High();
    delays(15);
    Scl_High();
    delays(15);
    if (Sda)
    ack=1;
    else
    ack=0;
    Scl_Low();
    delays(10);
}

uint8 i2c_recbyte()
{
    uint8 i,dat;
    dat=0;
	Sdac_High();
    for (i=0;i<8;i++)
    {
	      Scl_Low();
	      delays(15);
	      Scl_High();
	      delays(15);
	      dat<<=1;
	      if (Sda)
	      dat|=0x01;
	      delays(15);    
    }
    Scl_Low();
    delays(10);
    return(dat);
}

void delays(uint8 tms)
{
    volatile uint8 i;
    for (i=0;i<tms;i++);
}
