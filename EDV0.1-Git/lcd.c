#include "lcd.h"

/* 
 *
 * 显示缓冲区
 * [0...7]: 	4位字符显示
 * [8...10]: 	百分比显示条
 * [11...15]: 	5位数字显示		[11]-H...[15]-L
 * 
 */
uchar LCD_MEM_BUF[16];

// FOR SETUP LCD
const uchar LCD_INIT_BUF[] = {0X02, 0X06, 0X52};

const uchar LCD_DIS_TAB[] = 
{
	LCD_AA+LCD_AB+LCD_AC+LCD_AD+LCD_AE+LCD_AF,		                // 0	0
	LCD_AB+LCD_AC,								// 1	1
	LCD_AA+LCD_AB+LCD_AD+LCD_AE+LCD_AG, 					// 2	2
	LCD_AA+LCD_AB+LCD_AC+LCD_AD+LCD_AG,        				// 3 	3
	LCD_AB+LCD_AC+LCD_AF+LCD_AG, 						// 4	4
	LCD_AA+LCD_AC+LCD_AD+LCD_AF+LCD_AG, 					// 5	5
	LCD_AA+LCD_AC+LCD_AD+LCD_AE+LCD_AF+LCD_AG, 				// 6	6
	LCD_AA+LCD_AB+LCD_AC,                   				// 7 	7
	LCD_AA+LCD_AB+LCD_AC+LCD_AD+LCD_AE+LCD_AF+LCD_AG,			// 8	8
	LCD_AA+LCD_AB+LCD_AC+LCD_AD+LCD_AF+LCD_AG,				// 9	9
	LCD_AA+LCD_AB+LCD_AC+LCD_AE+LCD_AF+LCD_AG,				// A	10
	LCD_AC+LCD_AD+LCD_AE+LCD_AF+LCD_AG, 					// b	11
	LCD_AA+LCD_AD+LCD_AE+LCD_AF, 						// c	12
	LCD_AB+LCD_AC+LCD_AD+LCD_AE+LCD_AG, 					// d	13
	LCD_AA+LCD_AD+LCD_AE+LCD_AF+LCD_AG,					// E	14
	LCD_AA+LCD_AE+LCD_AF+LCD_AG,                 				// F 	15
	LCD_AA+LCD_AB+LCD_AC+LCD_AD+LCD_AE+LCD_AF+LCD_ADP,			// 0.	16
	LCD_AB+LCD_AC+LCD_ADP,							// 1.	17
	LCD_AA+LCD_AB+LCD_AD+LCD_AE+LCD_AG+LCD_ADP, 				// 2.	18
	LCD_AA+LCD_AB+LCD_AC+LCD_AD+LCD_AG+LCD_ADP,  				// 3.	19
	LCD_AB+LCD_AC+LCD_AF+LCD_AG+LCD_ADP,					// 4.	20
	LCD_AA+LCD_AC+LCD_AD+LCD_AF+LCD_AG+LCD_ADP,				// 5.	21
	LCD_AA+LCD_AC+LCD_AD+LCD_AE+LCD_AF+LCD_AG+LCD_ADP,			// 6.	22
	LCD_AA+LCD_AB+LCD_AC+LCD_ADP,   					// 7.	23
	LCD_AA+LCD_AB+LCD_AC+LCD_AD+LCD_AE+LCD_AF+LCD_AG+LCD_ADP,	        // 8.	24
	LCD_AA+LCD_AB+LCD_AC+LCD_AD+LCD_AF+LCD_AG+LCD_ADP,			// 9.	25
	LCD_AB+LCD_AC+LCD_AE+LCD_AF+LCD_AG,					// H	26
	LCD_AD+LCD_AE+LCD_AF,  							// L	27
	LCD_AC+LCD_AD+LCD_AE+LCD_AG, 						// o	28
	LCD_AE+LCD_AG, 								// r	29
	LCD_AG, 													// -	30
	LCD_AA+LCD_AB+LCD_AC+LCD_AE+LCD_AF					// n	31
	//0x00,                                             		        // 空	32
};

const uint LCD_DIS_ASCII[] = 
{
	LCD_MA+LCD_MB+LCD_MC+LCD_ME+LCD_MF+LCD_MJ+LCD_MK,			// A	0
	LCD_MC+LCD_MD+LCD_ME+LCD_MF+LCD_MJ+LCD_MK,				// b	1
	LCD_MA+LCD_MD+LCD_ME+LCD_MF,						// C	2
	LCD_MA+LCD_MB+LCD_MC+LCD_MD+LCD_MH+LCD_MM, 				// D	3
	LCD_MA+LCD_MD+LCD_ME+LCD_MF+LCD_MJ+LCD_MK,				// E	4
	LCD_MA+LCD_ME+LCD_MF+LCD_MJ+LCD_MK,					// F	5
	LCD_MA+LCD_MC+LCD_MD+LCD_ME+LCD_MF+LCD_MK,				// G	6
	LCD_MB+LCD_MC+LCD_ME+LCD_MF+LCD_MJ+LCD_MK,				// H	7
	LCD_MA+LCD_MD+LCD_MH+LCD_MM,						// I	8
	LCD_MA+LCD_MH+LCD_MJ,							// J	9
	LCD_ME+LCD_MF+LCD_MJ+LCD_MI+LCD_MN,					// K	10
	LCD_MD+LCD_ME+LCD_MF,							// L	11
	LCD_MB+LCD_MC+LCD_ME+LCD_MF+LCD_MG+LCD_MI,      			// M	12
	LCD_MB+LCD_MC+LCD_ME+LCD_MF+LCD_MG+LCD_MN,				// N	13
	LCD_MA+LCD_MB+LCD_MC+LCD_MD+LCD_ME+LCD_MF,				// O	14
	LCD_MA+LCD_MB+LCD_ME+LCD_MF+LCD_MJ+LCD_MK,				// P	15
	LCD_MA+LCD_MB+LCD_MC+LCD_MD+LCD_ME+LCD_MF+LCD_MN, 			// Q	16
	LCD_MA+LCD_MB+LCD_ME+LCD_MF+LCD_MJ+LCD_MK+LCD_MN,			// R	17
	LCD_MA+LCD_MC+LCD_MD+LCD_MF+LCD_MJ+LCD_MK,				// S	18
	LCD_MA+LCD_MH+LCD_MM, 							// T	19
	LCD_MB+LCD_MC+LCD_MD+LCD_ME+LCD_MF, 					// U	20
	LCD_MB+LCD_MC+LCD_MG+LCD_MN,						// V	21
	LCD_MB+LCD_MC+LCD_ME+LCD_MF+LCD_MN+LCD_ML,				// W	22
	LCD_MG+LCD_MI+LCD_ML+LCD_MN,						// X	23
	LCD_MG+LCD_MI+LCD_MM,							// Y	24
	LCD_MA+LCD_MD+LCD_ML+LCD_MI, 						// Z	25
	
	LCD_MB+LCD_MC+LCD_MD+LCD_ME+LCD_MJ+LCD_MK,				// d	26
	LCD_MC+LCD_ME+LCD_MM+LCD_MJ+LCD_MK,					// m	27
	LCD_MC+LCD_ME+LCD_MJ+LCD_MK,						// n	28
	LCD_MC+LCD_MD+LCD_ME+LCD_MJ+LCD_MK,    					// O下 	29
	LCD_MA+LCD_MB+LCD_MF+LCD_MJ+LCD_MK,					// O上	30
	LCD_ME+LCD_MJ+LCD_MK,							// r	31
	LCD_MM+LCD_MJ+LCD_MK							// t	32
};

static void delay(uchar t)
{
	for(; t>0; t--)
		_NOP();
}

/*-------------------------------------------------------------------
Name :void LCD_WrByte_bit7(void)
Description: LCM046 SETUP INITAB
Input : None
Output: BIT7 -> BIT6 ->... ->BIT0 ;BIT7 IS FIRST    !!!!
Date  : 2004-12-01
-------------------------------------------------------------------*/   
static void LCD_WrByte_bit7(uchar WriteD)
{
	uchar i;
	
	for(i=0; i<8; i++)
	{
		LCDWR_L;			// WR=0
		delay(2);
		if((WriteD & 0X80) == 0)	// BIT7 =0 ?
		{
			LCDDI_L;            	// DI=0
		}
		else
			{
				LCDDI_H;       	// DI=1
			}
		
		delay(2);
		LCDWR_H;             		// WR=1
		WriteD  <<= 1;         		// bit7 -> bit0
		delay(2);  
	}
	LCDWR_L;
}

/*-------------------------------------------------------------------
Name :void LCD_WrByte_bit0 (uchar WriteData)
Description: LCM046 display mem
Input : None
Output: BIT0 -> BIT1 ->... ->BIT7;BIT0 IS FIRST    !!!!
Date  : 2004-12-01
-------------------------------------------------------------------*/   
static void LCD_WrByte_bit0(uchar WriteData)
{
	uchar i;
  
	for(i=0; i<8; i++)
	{
		LCDWR_L;                 	// WR=0
		delay(2);
		if((WriteData & 0X01) == 0)	// BIT0 =0 ?
		{
			LCDDI_L;        	// DI=0
		}
		else
			{
				LCDDI_H;       	// DI=1
			}
		delay(2);
		LCDWR_H;               		// WR=1
		WriteData >>= 1;         	// bit0 -> bit7
		delay(2);
	}
	LCDWR_L;
}

/*-------------------------------------------------------------------
Name :void LCD_Init(void)
Description: DGM0416 SETUP
Input : None
Date  : 2004-12-01
-------------------------------------------------------------------*/   
static void LCD_Init(void)
{
	uchar i;
	
	LCD_OPEN;
	
    LCDCS_H;       	// CS=1
    LCDWR_L;       	// WR=0
    LCDDI_H;      	// DI=1
    delay(3);
    LCDCS_L;        	// CS=0
    delay(2);
    LCDWR_H;         	// WR=1
    delay(2);
    LCDWR_L;         	// WR=0  DATA=1
    delay(2);
    LCDDI_L;         	// DI=0  
    delay(2); 
    LCDWR_H;        	// WR=1
    delay(2);
    LCDWR_L;         	// WR=0  DATA=0
    delay(2);  
    LCDWR_H;         	// WR=1
    delay(2); 
    LCDWR_L;       	// WR=0  DATA=0
    
    for(i=0; i<3; i++)
	{ 
	    delay(2);
	    LCDDI_L;     	// OUT=0
	    delay(2);
	    LCDWR_H;         	// WR=1
	    delay(2);
	    LCDWR_L;         	// WR=0
	    delay(2);
	    LCD_WrByte_bit7(LCD_INIT_BUF[i]);	// lcd init
    }
    LCDCS_H;         	// CS=1
    LCDDI_H;          	// DI=1
}

/*-------------------------------------------------------------------
Name :void LCD_Display(void)
Description: DGM0416 SETUP 
Input : None
Date  : 2004-12-01
-------------------------------------------------------------------*/   
void LCD_Display(void)
{
	uchar i;
	
	LCD_Init();
		
	LCDCS_H;            // CS=1
	LCDWR_L;            // WR=0
	LCDDI_H;            // DI=1
	delay(3);
	LCDCS_L;            // CS=0
	delay(2);
	LCDWR_H;            // WR=1
	delay(2);
	LCDWR_L;            // WR=0  DATA=1
	delay(2);
	
	LCD_MEM_BUF[11] |= LCD_ADP;    		// ANCN 
	LCD_WrByte_bit7(0X40);  		// START ADDR |= 0X40
	for(i=0; i<16; i++)
		LCD_WrByte_bit0(LCD_MEM_BUF[i]);
	/*
	LCD_WrByte_bit0(LCD_MEM_BUF[0]);  	// 0
	LCD_WrByte_bit0(LCD_MEM_BUF[1]);  	// 1
	LCD_WrByte_bit0(LCD_MEM_BUF[2]);  	// 2
	LCD_WrByte_bit0(LCD_MEM_BUF[3]);  	// 3
	LCD_WrByte_bit0(LCD_MEM_BUF[4]);  	// 4 
	LCD_WrByte_bit0(LCD_MEM_BUF[5]);  	// 5
	LCD_WrByte_bit0(LCD_MEM_BUF[6]);  	// 6
	LCD_WrByte_bit0(LCD_MEM_BUF[7]);  	// 7
	LCD_WrByte_bit0(LCD_MEM_BUF[8]);  	// 8
	LCD_WrByte_bit0(LCD_MEM_BUF[9]);  	// 1
	LCD_WrByte_bit0(LCD_MEM_BUF[10]);  	// 2
	LCD_WrByte_bit0(LCD_MEM_BUF[11]);  	// 3 
	LCD_WrByte_bit0(LCD_MEM_BUF[12]);  	// 4 
	LCD_WrByte_bit0(LCD_MEM_BUF[13]);  	// 5
	LCD_WrByte_bit0(LCD_MEM_BUF[14]);  	// 6
	LCD_WrByte_bit0(LCD_MEM_BUF[15]);  	// 7
	*/
	delay(3);
	LCDCS_H;            // CS=1
	LCDDI_H;            // DI=1
	LCDWR_H;            // WR=1
}

// Clear the display buf
void LCD_DisplayClear(void)
{
	uchar i;
	
	for(i=0; i<16; i++)
	{
		LCD_MEM_BUF[i] = 0;
	}
}

void LCD_DisplayAll(void)
{
	uchar i;
	
	for(i=0; i<16; i++)
	{
		LCD_MEM_BUF[i] = 0xFF;
	}
}

/* 
 *
 * 显示细节
 * dp		- 小数点位置
 * number	- 显示数字
 * uint		- 显示单位
 * percent	- 显示百分比, 0 ~ 24
 * ep		- 惊叹号和数字消隐，bit7=1-显示!，bit7=0不显示!
 * 			- bit[0:4] 消隐数字位
 */
void LCD_Display_Detail(uchar dp, long number, LCD_Unit uint, uchar percent, uchar ep)
{
	///////////////////////////////////////////////////////////
	// 百分比
	if(percent == 0)
	{
		LCD_MEM_BUF[8] = 0;
		LCD_MEM_BUF[9] = 0;
		LCD_MEM_BUF[10] = 0;
	}
	else if((percent > 0) && (percent < 9))
		{
			LCD_MEM_BUF[8] = 0xFF >> (8 - percent);
			LCD_MEM_BUF[9] = 0;
			LCD_MEM_BUF[10] = 0;
		}
	else if((percent > 8) && (percent < 17))
		{
			LCD_MEM_BUF[8] = 0xFF;
			LCD_MEM_BUF[9] = 0xFF >> (16 - percent);
			LCD_MEM_BUF[10] = 0;
		}
	else if((percent > 16) && (percent < 25))
		{
			LCD_MEM_BUF[8] = 0xFF;
			LCD_MEM_BUF[9] = 0xFF;
			LCD_MEM_BUF[10] = 0xFF >> (24 - percent);
		}
	else if(percent > 24)
		{
			LCD_MEM_BUF[8] = 0xFF;
			LCD_MEM_BUF[9] = 0xFF;
			LCD_MEM_BUF[10] = 0xFF;
		}

	///////////////////////////////////////////////////////////
	// 显示数
	if(number >= 0)
	{
		number %= 100000;

		LCD_MEM_BUF[11] = number/10000;
		LCD_MEM_BUF[12] = number/1000%10;
		LCD_MEM_BUF[13] = number/100%10;
		LCD_MEM_BUF[14] = number/10%10;
		LCD_MEM_BUF[15] = number%10;

		LCD_MEM_BUF[11] = LCD_DIS_TAB[LCD_MEM_BUF[11]];
		LCD_MEM_BUF[12] = LCD_DIS_TAB[LCD_MEM_BUF[12]];
		LCD_MEM_BUF[13] = LCD_DIS_TAB[LCD_MEM_BUF[13]];
		LCD_MEM_BUF[14] = LCD_DIS_TAB[LCD_MEM_BUF[14]];
		LCD_MEM_BUF[15] = LCD_DIS_TAB[LCD_MEM_BUF[15]];

		if(ep & BIT0)
			LCD_MEM_BUF[15] = 0;
		if(ep & BIT1)
			LCD_MEM_BUF[14] = 0;
		if(ep & BIT2)
			LCD_MEM_BUF[13] = 0;
		if(ep & BIT3)
			LCD_MEM_BUF[12] = 0;
		if(ep & BIT4)
			LCD_MEM_BUF[11] = 0;
	}

	///////////////////////////////////////////////////////////
	// 小数点
	
	if(dp == 1)
		{
			LCD_MEM_BUF[15] |= LCD_ADP;
		}
	else if(dp == 2)
		{
			LCD_MEM_BUF[14] |= LCD_ADP;
		}
	else if(dp == 3)
		{
			LCD_MEM_BUF[13] |= LCD_ADP;
		}
	else if(dp == 4)
		{
			LCD_MEM_BUF[12] |= LCD_ADP;
		}
	
	///////////////////////////////////////////////////////////
	// 单位
	switch(uint)
	{
		case ADDR_U:
			//LCD_MEM_BUF[11] = 0;
			//LCD_MEM_BUF[12] = 0;
			
			LCD_MEM_BUF[0] = LCD_DIS_ASCII['A'-'A']/256;
			LCD_MEM_BUF[1] = LCD_DIS_ASCII['A'-'A']%256;
			LCD_MEM_BUF[2] = LCD_DIS_ASCII['D'-'A']/256;
			LCD_MEM_BUF[3] = LCD_DIS_ASCII['D'-'A']%256;
			LCD_MEM_BUF[4] = LCD_DIS_ASCII['D'-'A']/256;
			LCD_MEM_BUF[5] = LCD_DIS_ASCII['D'-'A']%256;
			LCD_MEM_BUF[6] = LCD_DIS_ASCII['R'-'A']/256;
			LCD_MEM_BUF[7] = LCD_DIS_ASCII['R'-'A']%256;
			break;

		case DB_U:
			//LCD_MEM_BUF[11] = 0;
			//LCD_MEM_BUF[12] = 0;
			//LCD_MEM_BUF[13] = 0;
			
			LCD_MEM_BUF[0] = 0;
			LCD_MEM_BUF[1] = 0;
			LCD_MEM_BUF[2] = 0;
			LCD_MEM_BUF[3] = 0;
			LCD_MEM_BUF[4] = LCD_DIS_ASCII[26]/256;
			LCD_MEM_BUF[5] = LCD_DIS_ASCII[26]%256;
			LCD_MEM_BUF[6] = LCD_DIS_ASCII[1]/256;
			LCD_MEM_BUF[7] = LCD_DIS_ASCII[1]%256;
			break;

		case XDB_U:
			//LCD_MEM_BUF[11] = 0;
			//LCD_MEM_BUF[12] = 0;
			//LCD_MEM_BUF[13] = 0;
			
			LCD_MEM_BUF[0] = 0;
			LCD_MEM_BUF[1] = 0;
			LCD_MEM_BUF[2] = LCD_DIS_ASCII[23]/256;
			LCD_MEM_BUF[3] = LCD_DIS_ASCII[23]%256;
			LCD_MEM_BUF[4] = LCD_DIS_ASCII[26]/256;
			LCD_MEM_BUF[5] = LCD_DIS_ASCII[26]%256;
			LCD_MEM_BUF[6] = LCD_DIS_ASCII[1]/256;
			LCD_MEM_BUF[7] = LCD_DIS_ASCII[1]%256;
			break;
			
		case TIME_U:
			//LCD_MEM_BUF[11] = 0;
			
			LCD_MEM_BUF[0] = LCD_DIS_ASCII[19]/256;
			LCD_MEM_BUF[1] = LCD_DIS_ASCII[19]%256;
			LCD_MEM_BUF[2] = LCD_DIS_ASCII[8]/256;
			LCD_MEM_BUF[3] = LCD_DIS_ASCII[8]%256;
			LCD_MEM_BUF[4] = LCD_DIS_ASCII[12]/256;
			LCD_MEM_BUF[5] = LCD_DIS_ASCII[12]%256;
			LCD_MEM_BUF[6] = LCD_DIS_ASCII[4]/256;
			LCD_MEM_BUF[7] = LCD_DIS_ASCII[4]%256;
			break;

		case THRESHOLD_U:
			//LCD_MEM_BUF[11] = 0;
			
			LCD_MEM_BUF[0] = LCD_DIS_ASCII[19]/256;
			LCD_MEM_BUF[1] = LCD_DIS_ASCII[19]%256;
			LCD_MEM_BUF[2] = LCD_DIS_ASCII[7]/256;
			LCD_MEM_BUF[3] = LCD_DIS_ASCII[7]%256;
			LCD_MEM_BUF[4] = LCD_DIS_ASCII[17]/256;
			LCD_MEM_BUF[5] = LCD_DIS_ASCII[17]%256;
			LCD_MEM_BUF[6] = LCD_DIS_ASCII[4]/256;
			LCD_MEM_BUF[7] = LCD_DIS_ASCII[4]%256;
			break;

		case KEY_U:
			//LCD_MEM_BUF[11] = 0;
			
			LCD_MEM_BUF[0] = 0;
			LCD_MEM_BUF[1] = 0;
			LCD_MEM_BUF[2] = LCD_DIS_ASCII[10]/256;
			LCD_MEM_BUF[3] = LCD_DIS_ASCII[10]%256;
			LCD_MEM_BUF[4] = LCD_DIS_ASCII[4]/256;
			LCD_MEM_BUF[5] = LCD_DIS_ASCII[4]%256;
			LCD_MEM_BUF[6] = LCD_DIS_ASCII[24]/256;
			LCD_MEM_BUF[7] = LCD_DIS_ASCII[24]%256;
			break;

		default:
			break;
	}

	///////////////////////////////////////////////////////////
	// 惊叹号
	if(ep & BIT7)
		LCD_MEM_BUF[7] |= LCD_MP;
	
	///////////////////////////////////////////////////////////
	LCD_Display();
}

