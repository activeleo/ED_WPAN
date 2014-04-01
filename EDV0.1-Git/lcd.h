#ifndef _LCD_H
#define _LCD_H
#include "msp430.h"
 #include "type.h"

#define LCD_AA   BIT7                       // LCD SEG 'A'
#define LCD_AB   BIT6                       // LCD SEG 'B'
#define LCD_AC   BIT5                       // LCD SEG 'C'
#define LCD_AD   BIT4                       // LCD SEG 'D'
#define LCD_AE   BIT1                       // LCD SEG 'E'
#define LCD_AF   BIT3                       // LCD SEG 'F'
#define LCD_AG   BIT2                       // LCD SEG 'G'
#define LCD_ADP  BIT0                       // LCD SEG 'DP'

#define LCD_MA   BITC                       // LCD SEG 'A'
#define LCD_MB   BIT5                       // LCD SEG 'B'
#define LCD_MC   BIT3                       // LCD SEG 'C'
#define LCD_MD   BITF                       // LCD SEG 'D'
#define LCD_ME   BITB                       // LCD SEG 'E'
#define LCD_MF   BIT8                       // LCD SEG 'F'
#define LCD_MG   BITD                       // LCD SEG 'G'
#define LCD_MH   BIT0                       // LCD SEG 'H'
#define LCD_MI   BIT4                       // LCD SEG 'I'
#define LCD_MJ   BIT9                       // LCD SEG 'J'
#define LCD_MK   BIT1                       // LCD SEG 'K'
#define LCD_ML   BITA                       // LCD SEG 'L'
#define LCD_MM   BITE                       // LCD SEG 'M'
#define LCD_MN   BIT2                       // LCD SEG 'N'
#define LCD_MO   BIT6                       // LCD SEG 'O' 
#define LCD_MP   BIT7                       // LCD SEG 'R'

/*#define LCDCS_H  	P4OUT |=  BIT3         //LCD_CS=1 P43
#define LCDCS_L  	P4OUT &= ~BIT3         //LCD_CS=0 

#define LCDWR_H  	P4OUT |=  BIT4         //LCD_WR=1 P44
#define LCDWR_L  	P4OUT &= ~BIT4         //LCD_WR=0

#define LCDDI_H  	P4OUT |=  BIT5         //LCD_DI=1 P45
#define LCDDI_L  	P4OUT &= ~BIT5         //LCD_DI=0 

#define LCD_OPEN 	P4DIR |= BIT3 + BIT4 + BIT5;
*/

#define LCDCS_H  	P2OUT |=  BIT0         //LCD_CS=1 P20
#define LCDCS_L  	P2OUT &= ~BIT0         //LCD_CS=0 

#define LCDWR_H  	P3OUT |=  BIT0         //LCD_WR=1 P30
#define LCDWR_L  	P3OUT &= ~BIT0         //LCD_WR=0

#define LCDDI_H  	P3OUT |=  BIT1         //LCD_DI=1 P31
#define LCDDI_L  	P3OUT &= ~BIT1         //LCD_DI=0 

#define LCD_OPEN 	P3DIR |= BIT0 + BIT1;P2DIR |=  BIT0;

typedef enum
{
	DB_U,
	XDB_U,
	ADDR_U,
	TIME_U,
	THRESHOLD_U,
	KEY_U
}LCD_Unit;

 

void LCD_Display(void);
void LCD_DisplayClear(void);
void LCD_DisplayAll(void);
void LCD_Display_Detail(uchar dp, long number, LCD_Unit uint, uchar percent, uchar ep);


#endif
