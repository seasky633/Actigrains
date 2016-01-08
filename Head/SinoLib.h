/*;***************************************************************************************
;	Model		: fixedpoint.h
;	Description	: Normal fixed point calculation.
;	Author		: Ma Hai Jun
;	Create Time	: 2009-01-01
;*****************************************************************************************/
#ifndef		_FIXEDPOINT_H
#define		_FIXEDPOINT_H

extern unsigned int 	FPM0808U(unsigned char i,unsigned char j);		//Byte * Byte
extern signed int 	FPM0808S(signed char i,signed char j);
extern unsigned long 	FPM1608U(unsigned short i,unsigned char j);		//Word * Byte
extern signed long	FPM1608S(signed short i,signed char j);
extern unsigned long	FPM1616U(unsigned short i,unsigned short j);		//Word * Word
extern signed long	FPM1616S(signed short i,signed short j);	
extern unsigned char	FPD0808U(unsigned char i,unsigned char j);		//Byte / Byte
extern signed char	FPD0808S(signed char i,signed char j);			
extern unsigned short	FPD1608U(unsigned short i,unsigned char j);		//Word / Byte
extern signed short	FPD1608S(signed short i,signed char j);
extern unsigned short	FPD1616U(unsigned short i,unsigned short j);		//Word / Word
extern signed short	FPD1616S(signed short i,signed short j);
extern unsigned long	FPD3208U(unsigned long i,unsigned char j);		//DWord / Byte
extern signed long	FPD3208S(signed long i,signed char j);
extern unsigned long	FPD3216U(unsigned long i,unsigned int j);		//DWord / Word
extern signed long	FPD3216S(signed long i,signed int j);
extern unsigned long	FPD3232U(unsigned long i,unsigned long j);		//DWord / Dword
extern signed long	FPD3232S(signed long i,signed long j);


extern	unsigned char BCD8toHEX(unsigned char BCD);				//BCD 00--99 ==> HEX 0x00--0x63.
extern 	unsigned int  BCD16toHEX(unsigned int BCD);				//BCD 00--9999==>HEX 0x00--0x270F
extern	unsigned long BCD32toHEX(unsigned long BCD);				//BCD 00--99999999==>HEX 0x00--0x5F5E0FF
 
extern	unsigned int  HEX8toBCD(unsigned char HEX);				//HEX 0x00--0xff==>BCD 00--255
extern	unsigned long HEX16toBCD(unsigned int HEX);				//HEX 0x00--0xffff==>BCD 00--65535
extern	unsigned long HEX32toBCD(unsigned long HEX);				//HEX 0x00--0x5f5e0ff==>BCD 00--99999999

extern void	BCDDec(unsigned char xdata *ArrayPtr,unsigned char ArrayLength);	//BCD++
extern void	BCDInc(unsigned char xdata *ArrayPtr,unsigned char ArrayLength);	//BCD--

extern	char xdata *memcpy_xdata(char xdata *source,char xdata *dest,unsigned char i); // copy source to dest,return dest address.	
extern	char data  *memcpy_data(char data *source,char data *dest,unsigned char i); // copy source to dest,return dest address.	
extern  unsigned char getstrlen_data(char data *string);
extern	unsigned char getstrlen_xdata(char xdata *string);
extern char data *memcpyfrom_ndata(char data *source,char data *dest,unsigned char from,unsigned char n); /*從From位置開始複製source數組中的n個數據到dest中。*/	
extern char xdata *memcpyfrom_nxdata(char xdata *source,char xdata *dest,unsigned char from,unsigned char n);
extern char data *memcpyfrom_tondata(char data *source,char data *dest,unsigned char sourcefrom,unsigned char destfrom,unsigned char n);
extern char xdata *memcpyfrom_tonxdata(char xdata *source,char xdata *dest,unsigned char sourcefrom,unsigned char destfrom,unsigned char n);
#endif
