/* EndDevice-V1.0.0  定型版
2014-3 版本变更履历
2014-3-27 全新版本的升级
说明 :由于更换硬件平台，所以全部进行模块化和标准化的升级。

*/

#include "msp430.h"
#include "type.h"
#include "uart.h"
#include "netapp.h"
#include "xbeeapp.h"
#include "timer.h"
#include "power.h"
#include "lcd.h"
#include "adc10.h"

#define rTY		0	//设备类型
#define rNS		0	//信号质量
#define rPW		0	//电源电压
#define rSN		0	//脱网时间
#define rST		0	//唤醒时间
#define rPs		0	//起始地址
#define rPL		0	//访问长度
#define rPG		0	//系统页
#define rDB0		0	//数据0
#define rDB1		0	//数据1
#define rDB2		0	//数据2
#define rDB3		0	//数据3
#define rDB4		0	//数据4
#define rDB5		0	//数据5
#define rDB6		0	//数据6
#define rDB7		0	//数据7

#define PG0		0	//系统页
#define PG1		0	//系统页
#define PG2		0	//系统页
#define PG3		0	//系统页
#define PG4		0	//系统页
#define PG5		0	//系统页
#define PG6		0	//系统页
#define PG7		0	//系统页


//P2OUT ^= 0x08;
#define LED1_ON         P2OUT |= 0x08
#define LED1_OFF         P2OUT &= ~0x08
#define LED2_ON         P2OUT |= 0x10
#define LED2_OFF         P2OUT &= ~0x10

#define XBEE_ON         P2OUT |= 0x20
#define XBEE_OFF         P2OUT &= ~0x20

#define TimCH0  0x00
#define TimCH1  0x01
#define TimCH2  0x02
#define TimCH3  0x03


#define RxdSize 32


#define ED_UC At_Da_Set(ED.RT_MACH,ED.RT_MACL,1)
#define ED_BC At_Da_Set(0x00000000,0x0000FFFF,1)


#define Data_Length 		16
#define Txd 			1
#define Rxd 			2
#define RxdBufSize 	        32

#define EDRegSize 	        64

#define  PowerOn                1
#define  PowerOff               0
#define  ReqInSleep             1
#define  ReqExitSleep           0
#define  Active                 1
#define  Sleep                  0

 struct ED_Device
{
	rt_uint8_t     	eIP;        	// 设备IP地址
  	rt_uint8_t    	cIP;        	// WRTU-IP地址 
   	rt_uint8_t    	rIP;        	// WRTU-IP地址 
   	rt_uint8_t     	xPAN;      	// PANID-网络ID
  	rt_uint8_t      	xSC;        	//
  	rt_uint8_t      	xSD;        	// 
   	rt_uint8_t      	xCE;        	// 
   	rt_uint8_t      	xPL;        	// 

   	rt_uint8_t      	xPM;        	// 
   	
   	rt_uint8_t      	xSP;        	//
   	rt_uint8_t      	xSN;        	//
   	rt_uint8_t      	xSM;        	//
   	rt_uint16_t      xST;        	//
    	rt_uint8_t      	xSO;        	//  	   	   	   	
 
  	rt_uint32_t     	eMACH;      // 设备MAC地址
  	rt_uint32_t     	eMACL;      // 设备MAC地址
  	rt_uint32_t     	cMACH;      // WRTU-MAC地址
  	rt_uint32_t     	cMACL;      // WRTU-MAC地址  
  	rt_uint32_t     	dMACH;      // 目标MAC地址
  	rt_uint32_t     	dMACL;      // 目标MAC地址
  
  	unsigned char 	*pReg;//寄存器指针
  	unsigned char 	*pRxd;//接收缓冲区指针
  	unsigned char 	*pTxd;//发送缓冲区指针

};

 struct Net_Farme
{	
rt_uint8_t     	DA;     //目标地址
rt_uint8_t      	FC;     //功能码
rt_uint16_t      SD;     //起始地址
rt_uint16_t      LN;     //数据长度
rt_uint8_t		 DATA[Data_Length];
rt_uint16_t     CRC;
}; 

//struct Net_Farme DevMsg;


//#include "xbeeapp.h"
/*#define SET_BIT(x,y) ((x)|=(0x0001<<(y)))  //置x的第ｙ位为1
#define CLR_BIT(x,y) ((x)&=~(0x0001<<(y)))  //清x的第ｙ位为0
#define CPL_BIT(x,y) ((x)^=(0x0001<<(y)))  //取反x的第ｙ位
#define GET_BIT(x,y) ((x)&(0x0001<<(y)))   //读取x的第ｙ位*/

extern struct Net_Farme MsgTxd;
extern struct Net_Farme MsgRxd;
 
extern unsigned char RxdData[32];
// extern ED_Device_t ED;
extern unsigned char InitFlash(unsigned char Mode);
extern unsigned int WRTU_LinkCnt; 
extern unsigned char ED_RunSta;
extern unsigned int WRTU_LinkCnt; 
extern unsigned char FlashChk;/* 标记Flash参数是否正确 */
extern unsigned char TxFlag;
extern unsigned char RxFlag;
extern unsigned int XbeeBusy;
extern unsigned char RxdOk;/* 接收数据OK标记 */
extern unsigned char RxdCnt;/* 接收数据的计数 每一次接收会清空 */
extern unsigned char RxdCntOld;
extern unsigned char WRTU_LinkUP;
extern unsigned int TimerCnt0;/*  */
extern unsigned int TimerUp;/*  */
extern unsigned int TimerCtrl;/*  */
extern unsigned int TimerCntRun;/*  */
extern unsigned char InitFlag; 
extern unsigned int InitCnt;
extern unsigned char ActMsgFlag;
extern unsigned int ActMsgCnt;
extern unsigned int ActMsgEn;
extern unsigned char ADCFlag;
extern unsigned int ADCCnt;
extern unsigned char RxdState;/* 接收数据OK标记 */

extern struct ED_Device ED;
extern unsigned char HoldReg[64];
//extern unsigned char EdReg[16];
//extern unsigned char PgReg[8][8];

extern rt_uint8_t XbeePowerStatus;         //电源状态：1=Active 0=Sleep   
extern rt_uint8_t SleepRunStatus;          //当前状态：1=Active 0=Sleep 
extern rt_uint8_t SleepReqStatus;          //申请状态：1=Active 0=Sleep   
extern rt_uint8_t SleepTimeSet;            //休眠的时间参数


void InitKey(void);
rt_uint8_t GetKey(void);
void ProcessKey(void);

extern unsigned char  Net_Process();
extern void erase_flash(unsigned int Addr);
extern unsigned char write_flash( char *value,unsigned int Add,char Len);
extern unsigned char read_flash(char *value,unsigned int Add,char Len);

extern void Set_DA(void);
extern void ClearRxd(void);
extern void process(void);
extern void init_xbee_reg(void);
extern unsigned char  Net_Init(void);
extern unsigned char Net_Transmission(unsigned char MsgType,unsigned int DestAdd, unsigned int MsgCode);
extern void erase_flash(unsigned int Addr);
