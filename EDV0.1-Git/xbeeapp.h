#ifndef __xbeeapp_h
#define __xbeeapp_h
#include "type.h"
/* RT-Thread basic data type definitions */
//typedef signed   char                   rt_int8_t;      /**<  8bit integer type */
//typedef signed   short                  rt_int16_t;     /**< 16bit integer type */
//typedef signed   long                   rt_int32_t;     /**< 32bit integer type */
//typedef unsigned char                   rt_uint8_t;     /**<  8bit unsigned integer type */
//typedef unsigned short                  rt_uint16_t;    /**< 16bit unsigned integer type */
//typedef unsigned long                   rt_uint32_t;    /**< 32bit unsigned integer type */
//typedef int                             rt_bool_t;      /**< boolean type */

//typedef	rt_int8_t		int8_t;
//typedef	rt_uint8_t		uint8_t;
//typedef	rt_int16_t		int16_t;
//typedef	rt_uint16_t		uint16_t;
//typedef	rt_int32_t		int32_t;
//typedef	rt_uint32_t		uint32_t;
//typedef long long		int64_t;
//typedef unsigned long long      uint64_t;


 
 
extern unsigned char RxdCnt ;
extern unsigned char RxdData[32];
extern void UartWriteChar(char c);
extern void UartWriteStr(char *s);
extern void TimerSrv(unsigned int Delay);
extern void DelayTime(unsigned int delay);
unsigned char At_WR(void);

extern unsigned char HexToChar(unsigned char bChar);
extern unsigned char CharToHex(unsigned char bHex);
void ClearRxd(void);
extern void At_Busy();
extern void process();
extern unsigned char At_Init();
extern unsigned char At_Exit();
extern unsigned char At_Reset();
extern unsigned char At_NetReset();
extern unsigned char At_ResDef();
extern unsigned char At_Write(unsigned char *cmd,unsigned char delay);
extern unsigned char At_Msg_Proc();
extern void At_Msg_Get(unsigned char *cmd ,unsigned int delay,uint32_t *data,unsigned char wr );
extern unsigned char  At_Msg_Set(unsigned char *cmd ,unsigned char delay,uint32_t data,unsigned char wr );
extern unsigned char write_flash( char *value,unsigned int Add,char Len);
extern unsigned char  At_Da_Set(uint32_t DaH,uint32_t DaL,unsigned char wr );
extern unsigned char  At_Da_Get(void);

/*
#define XbeeRegSize  32 // 0X90
unsigned char XbeeReg[XbeeRegSize];
//Set/Read the ZigBee extended PAN ID 0 - 0xFFFFFFFFFFFFFFFF
#define xbee_ID0   0 //0x00 
#define xbee_ID1   1 //0x01
#define xbee_ID2   2 //0x02
#define xbee_ID3   3 //0x03
#define xbee_ID4   4 //0x04
#define xbee_ID5   5 //0x05
#define xbee_ID6   6 //0x06
#define xbee_ID7   7 //0x07
#define xbee_SC0   8 //0x08 RANGE:1-0XFFFF   BITFIELD
#define xbee_SC1   9 //0x09
#define xbee_SD    10 //0x0a RANGE:0-0X07   EXPONENT
#define xbee_ZS    11 //0x0b RANGE:0-2
#define xbee_NJ    12 //0x0c RANGE:0-0XFF    X 1 SEC
#define xbee_JN    13 //0x0d 
#define xbee_OP    14 //0x0e Read the operating PAN ID (ZigBee extended PAN ID)
#define xbee_OI0   15 //0x0f Read the 16-bit operating PAN ID.
#define xbee_OI1   16 //0x10
#define xbee_CH    17 //0x11
#define xbee_NC    18 //0x12
#define xbee_CE    19 //0x13 Set/read if this device is a coordinator (1) or not (0)
//ADDRESSING
#define xbee_SH0    20 //0x14 Read high 32 bits of modems unique IEEE 64-bit Extended Address.
#define xbee_SH1    21 //0x15
#define xbee_SH2    22 //0x16
#define xbee_SH3    23 //0x17
#define xbee_SL0    24 //0x18
#define xbee_SL1    25 //0x19
#define xbee_SL2    26 //0x1a
#define xbee_SL3    27 //0x1b
#define xbee_MY0    28 //0x1c
#define xbee_MY1    29 //0x1d
#define xbee_DH0    30 //0x1e destination extended address
#define xbee_DH1    31 //0x1f
#define xbee_DH2    32 //0x20
#define xbee_DH3    33 //0x21
#define xbee_DL0    34 //0x22
#define xbee_DL1    35 //0x23
#define xbee_DL2    36 //0x24
#define xbee_DL3    37 //0x25
#define xbee_NI0    38 //0x26
#define xbee_NI1    39 //0x27
#define xbee_NI2    40 //0x28
#define xbee_NI3    41 //0x29
#define xbee_NI4    42 //0x2a
#define xbee_NI5    43 //0x2b
#define xbee_NI6    44 //0x2c
#define xbee_NI7    45 //0x2d
#define xbee_NH     46 //0x2e
#define xbee_BH     47 //0x2f
#define xbee_AR     48 //0x30
#define xbee_DD0    49 //0x31
#define xbee_DD1    50 //0x32
#define xbee_NT     51 //0x33
#define xbee_NO     52 //0x34
#define xbee_NP     53 //0x35
//ZIGBEE ADDRESSING
#define xbee_SE    54 //0x36
#define xbee_DE    55 //0x37
#define xbee_CI    56 //0x38
//RF interface
#define xbee_PL    57 //0x39 Select/Read transmitter output power. 
#define xbee_PM    58 //0x3A Select/Read module boost mode setting. 
#define xbee_PP    59 //0x3B Read dBm output when PL4 is selected.
//security 
#define xbee_EE    60 //0x3C 
#define xbee_EO    61 //0x3D 
// --ZigBee trust center link key
#define xbee_KY0    62 //0x3E 1
#define xbee_KY1    63 //0x3F 2
#define xbee_KY2    64 //0x40 3
#define xbee_KY3    65 //0x41 4
#define xbee_KY4    66 //0x42 5
#define xbee_KY5    67 //0x43 6
#define xbee_KY6    68 //0x44 7
#define xbee_KY7    69 //0x45 8
#define xbee_KY8    70 //0x46 9
#define xbee_KY9    71 //0x47 10
#define xbee_KYA    72 //0x48 11
#define xbee_KYB    73 //0x49 12
#define xbee_KYC    74 //0x4A 13
#define xbee_KYD    75 //0x4B 14
#define xbee_KYE    76 //0x4C 15
#define xbee_KYF    77 //0x4D 16
#define xbee_NK     78 //0x4E 
//Change modem interfacing options
#define xbee_BD     79 //0x4F
#define xbee_NB     80 //0x50
#define xbee_SB     81 //0x51
#define xbee_RO     82 //0x52
#define xbee_D7     83 //0x53//CTS flow control,or RS-485 enable control.
#define xbee_D6     84 //0x54
#define xbee_AP     85 //0x55
#define xbee_AO     86 //0x56
//Change AT command mode behavior
#define xbee_CT0    87 //0x57
#define xbee_CT1    88 //0x58
#define xbee_GT0    89 //0x59
#define xbee_GT1    90 //0x5A
#define xbee_CC     91 //0x5B
//Configure low power options to support end device children
#define xbee_SP     92 //0x5C
#define xbee_SN     93 //0x5D
#define xbee_SM     94 //0x5E
#define xbee_ST0    95 //0x5F
#define xbee_ST1    96 //0x60
#define xbee_SO     97//0x61
//Modify DIO and ADC options
#define xbee_D0     98 //0x62
#define xbee_D1     99 //0x63
#define xbee_D2     100 //0x64
#define xbee_D3     101 //0x65
#define xbee_D4     102 //0x66
#define xbee_D5     103 //0x67
#define xbee_D8     104 //0x68
#define xbee_D9     105 //0x69
#define xbee_P0     106 //0x6A
#define xbee_P1     107 //0x6B
#define xbee_P2     108 //0x6C
#define xbee_P3     109 //0x6D
#define xbee_P4     110 //0x6E
#define xbee_P5     111 //0x6F
#define xbee_P6     112 //0x70
#define xbee_P7     113 //0x71
#define xbee_P8     114 //0x72
#define xbee_P9     115 //0x73
#define xbee_PR0    116 //0x74
#define xbee_PR1    117 //0x75
#define xbee_PD0    118 //0x76
#define xbee_PD1    119 //0x77
#define xbee_LT     120 //0x78
#define xbee_RP     121 //0x79
#define xbee_IR     122 //0x7A
#define xbee_IC     123 //0x7B
#define xbee_VS0    124 //0x7C Configure the supply voltage high threshold
#define xbee_VS1    125 //0x7D Configure the supply voltage high threshold
//Access diagnostic parameters
#define xbee_VR0    126 //0x7E
#define xbee_VR1    127 //0x7F
#define xbee_HV0    128 //0x80
#define xbee_HV1    129 //0x81
#define xbee_AI     130 //0x82
#define xbee_DB     131 //0x83
#define xbee_SV0    132 //0x84
#define xbee_SV1    133 //0x85

*/
#endif /* __xbeeapp_h */

/* end of xbeeapp.h */





