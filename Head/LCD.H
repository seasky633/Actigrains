#ifndef _LCD_H_
#define	_LCD_H_

/*-----------------------------*
 *  include head files         *
 *----------------------------*/

/*-----------------------------*
 *  Macros for constants       *
 *----------------------------*/

/*-----------------------------*
 *  Macros for others          *
 *----------------------------*/

/*
	 a
   - - -
  f| g |b
   - - -
  e|   |c
   - - -
     d	 
*/

#define     Sega  0x80  //D1,D2,D12,D13,D14,D15,D16,D17
#define     Segb  0x20
#define     Segc  0x04
#define     Segd  0x02
#define     Sege  0x08
#define     Segf  0x40
#define     Segg  0x10

#define     Segax  0x80 //D5,D6,D7,D8,D9,D10,D11
#define     Segbx  0x40
#define     Segcx  0x20
#define     Segdx  0x10
#define     Segex  0x02
#define     Segfx  0x08
#define     Seggx  0x04

#define     Segay  0x01 //D4
#define     Segby  0x04
#define     Segcy  0x20
#define     Segdy  0x40
#define     Segey  0x10
#define     Segfy  0x02
#define     Seggy  0x08

#define		Num0	(Sega|Segb|Segc|Segd|Sege|Segf)
#define		Num1	(Segb|Segc)
#define		Num2	(Sega|Segb|Segd|Sege|Segg)
#define		Num3	(Sega|Segb|Segc|Segd|Segg)
#define		Num4	(Segb|Segc|Segf|Segg)
#define		Num5	(Sega|Segc|Segd|Segf|Segg)
#define		Num6	(Sega|Segc|Segd|Sege|Segf|Segg)
#define		Num7	(Sega|Segb|Segc)
#define		Num8	(Sega|Segb|Segc|Segd|Sege|Segf|Segg)
#define		Num9	(Sega|Segb|Segc|Segd|Segf|Segg)
#define		Numb    (Segc|Segd|Sege|Segf|Segg)
#define		Nume	(Sega|Segd|Sege|Segf|Segg)
#define     Numf    (Sega|Sege|Segf|Segg)
#define		Numt	(Segd|Sege|Segf|Segg)
#define		Numu	(Segb|Segc|Segd|Sege|Segf)
#define		Nump	(Sega|Segb|Sege|Segf|Segg)
#define		Numa 	(Sega|Segb|Segc|Sege|Segf|Segg)
#define     Numh    (Segb|Segc|Sege|Segf|Segg)


#define		Num0x	(Segax|Segbx|Segcx|Segdx|Segex|Segfx)
#define		Num1x	(Segbx|Segcx)
#define		Num2x	(Segax|Segbx|Segdx|Segex|Seggx)
#define		Num3x	(Segax|Segbx|Segcx|Segdx|Seggx)
#define		Num4x	(Segbx|Segcx|Segfx|Seggx)
#define		Num5x	(Segax|Segcx|Segdx|Segfx|Seggx)
#define		Num6x	(Segax|Segcx|Segdx|Segex|Segfx|Seggx)
#define		Num7x	(Segax|Segbx|Segcx)
#define		Num8x	(Segax|Segbx|Segcx|Segdx|Segex|Segfx|Seggx)
#define		Num9x	(Segax|Segbx|Segcx|Segdx|Segfx|Seggx)


#define		Num0y	(Segay|Segby|Segcy|Segdy|Segey|Segfy)
#define		Num1y	(Segby|Segcy)
#define		Num2y	(Segay|Segby|Segdy|Segey|Seggy)
#define		Num3y	(Segay|Segby|Segcy|Segdy|Seggy)
#define		Num4y	(Segby|Segcy|Segfy|Seggy)
#define		Num5y	(Segay|Segcy|Segdy|Segfy|Seggy)
#define		Num6y	(Segay|Segcy|Segdy|Segey|Segfy|Seggy)
#define		Num7y	(Segay|Segby|Segcy)
#define		Num8y	(Segay|Segby|Segcy|Segdy|Segey|Segfy|Seggy)
#define		Num9y	(Segay|Segby|Segcy|Segdy|Segfy|Seggy)

#define		VBUF_SIZE 28


#define		GIF1	0x0001     //T3 
#define		GIF2	0x0002     //T4
#define		GIF3	0x0004     //T5 
#define		GIF4	0x0008     //T6   
#define		GIF5	0x0010     //T7   
#define		GIF6	0x0020     //T8 
#define		GIF7	0x0040     //T9_T20  
#define		GIF8	0x0080     //T10_T21_T22  
#define		GIF9    0x0100     //T11_T21_T22  
#define		GIF10	0x0200     //T12_T23 
#define		GIF11   0x0400     //T13 
#define		GIF12	0x0800     //T14 
#define		GIF13   0x1000     //T15 
#define		GIF14	0x2000     //T16 
#define		GIF15	0x4000     //T17 
#define		GIF16	0x8000     //T18


//煲体(带篮子)
#define		GIF_Cooker_BSK_S1		(GIF12|GIF13|GIF14|GIF15|GIF16)  
//煲体(不带篮子)
#define     GIF_Cooker_NBSK_S1      (GIF12|GIF14|GIF15|GIF16)     
//煲体(不带篮子)(不带上盖)
#define     GIF_Cooker_NBSK_NLID_S1 (GIF14|GIF15|GIF16)
//放入食物(带篮子)            
#define     GIF_Food_BSK_S1  	    (GIF6|GIF13|GIF16)
#define     GIF_Food_BSK_S2  	    (GIF7|GIF13|GIF16)
#define		GIF_Food_BSK_S3  	    (GIF15|GIF13|GIF16)
//放入食物(不带篮子)
#define     GIF_Food_NBSK_S1 	    GIF16                    
#define     GIF_Food_NBSK_S2  	    (GIF6|GIF16)
#define     GIF_Food_NBSK_S3  	    (GIF7|GIF16)
#define		GIF_Food_NBSK_S4  	    (GIF15|GIF16)
//放入水(带篮子)
#define     GIF_Water_BSK_S1        (GIF11|GIF13|GIF15|GIF16)
#define     GIF_Water_BSK_S2        (GIF10|GIF13|GIF15|GIF16)    
#define     GIF_Water_BSK_S3        (GIF14|GIF13|GIF15|GIF16) 
//放入水(不带篮子)
#define     GIF_Water_NBSK_S1       (GIF15|GIF16)
#define     GIF_Water_NBSK_S2       (GIF11|GIF15|GIF16)
#define     GIF_Water_NBSK_S3       (GIF10|GIF15|GIF16)    
#define     GIF_Water_NBSK_S4       (GIF14|GIF15|GIF16) 
//放入篮子
#define     GIF_Putbasket_S1        (GIF2|GIF16)        
#define     GIF_Putbasket_S2        (GIF9|GIF16)  
#define     GIF_Putbasket_S3        (GIF13|GIF16) 
//取出食物(带篮子)&倒出水
#define     GIF_Outfood_S1          (GIF13|GIF14|GIF15|GIF16)
#define     GIF_Outfood_S2          (GIF8|GIF16)
#define     GIF_Outfood_S3          (GIF2|GIF3|GIF4|GIF16)
#define     GIF_Outwater_S1         (GIF14|GIF16)
#define     GIF_Outwater_S2         (GIF8|GIF16)
#define     GIF_Outwater_S3         (GIF5|GIF16)
//放入滤水后的食物(带篮子)
#define     GIF_Drainedfood_BSK_S1   (GIF2|GIF3|GIF16)
#define     GIF_Drainedfood_BSK_S2   (GIF9|GIF16)
#define     GIF_Drainedfood_BSK_S3   (GIF13|GIF15|GIF16)
//放入滤水后的食物(不带篮子)
#define     GIF_Drainedfood_NBSK_S1  (GIF1|GIF16)
#define     GIF_Drainedfood_NBSK_S2  (GIF9|GIF16)
#define     GIF_Drainedfood_NBSK_S3  (GIF15|GIF16)


//各区域显示状态
#define     ZONE1_STAT_ALLOFF               0
#define     ZONE1_STAT_NUTRIMENU_SELECT     1
#define     ZONE1_STAT_CLASSICMENU_SELECT   2
#define     ZONE1_STAT_TEXTURE_SELECT       3
#define     ZONE1_STAT_TEXTURE_CONFIRM      4

#define     ZONE2_STAT_ALLOFF               0
#define     ZONE2_STAT_PEOPLENUM_SELECT     1
#define     ZONE2_STAT_PEOPLENUM_CONFIRM    2

#define     ZONE3_STAT_ALLOFF               0
#define     ZONE3_STAT_PUTFOOD              1
#define     ZONE3_STAT_PUTWATER             2
#define     ZONE3_STAT_DEBUG                3

#define     ZONE4_STAT_ALLOFF               0
#define     ZONE4_STAT_SET_DEFAULT          1
#define     ZONE4_STAT_SET_CONFIRM          2
#define     ZONE4_STAT_DIY_SELECT           3
#define     ZONE4_STAT_TEMP_SELECT          4
#define     ZONE4_STAT_TIME_SELECT          5
#define     ZONE4_STAT_AUTO_CONFIRM         6
#define     ZONE4_STAT_DEBUG                7

/*-----------------------------*
 *  type definitions           *
 *----------------------------*/

/*-----------------------------*
 *  structure,union and enum   *
 *----------------------------*/
typedef	struct{
    uint8       u8Menu;
    uint8       u8texture;
    uint8       u8People;
    uint8       u8Cup;
    uint8       u8Water;
	uint8		u8DIYNum;
	uint8		u8Temp;
	uint8		u8Time[2];
}DispParam;
/*-----------------------------*
 *  public functions prototypes*
 *----------------------------*/
extern  void	LCD_INIT(void);
extern  void  	LCDProc(uint8);
extern  void	PlayGif(uint16 code *pGif,uint16 speed,uint8 stat);
extern  void    LCDDispZone1(uint8 menu,uint8 texture,uint8 state);
extern  void    LCDDispZone2(uint8 people,uint8 state);
extern  void    LCDDispZone3(uint16 cup,uint16 water,uint8 foodtype,uint8 state);
extern  void    LCDDispZone4(uint8 diynum,uint8 temp,uint8 *ptime,uint8 state);

/*-----------------------------*
 *  public variable declaration*
 *----------------------------*/	
extern  uint16  code GifCookerBSK[];    //煲体(带篮子)
extern  uint16  code GifCookerNBSK[];   //煲体(不带篮子)
extern  uint16  code GifCookerNBSKNLID[];//煲体(不带篮子)(不带上盖)
extern  uint16  code GifFoodBSK[];      //放入食物(带篮子)
extern  uint16  code GifFoodNBSK[];     //放入食物(不带篮子)
extern  uint16  code GifWaterBSK[];     //放入水(带篮子)
extern  uint16  code GifWaterNBSK[];    //放入水(不带篮子)
extern  uint16  code GifPutbasket[];    //放入篮子
extern  uint16  code GifOutfood[];      //取出食物(带篮子)  //
extern  uint16  code GifOutwater[];     //倒出水
extern  uint16  code GifDrainedfoodBSK[];   //放入滤水后的食物(带篮子)
extern  uint16  code GifDrainedfoodNBSK[];  //放入滤水后的食物(不带篮子)
extern  uint8   u8BlankCnt;
extern  uint8   idata m_u8SelfChkStep;          
#endif
