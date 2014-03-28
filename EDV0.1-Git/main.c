//   MSP430G2xx3 Demo - USCI_A0, 115200 UART Echo ISR, DCO SMCLK
//
//   Description: Echo a received character, RX ISR used. Normal mode is LPM0.
//   USCI_A0 RX interrupt triggers TX Echo.
//   Baud rate divider with 1MHz = 1MHz/115200 = ~8.7
//   Baud rate divider with 1MHz = 1MHz/9600 = ~104.2
//   ACLK = n/a, MCLK = SMCLK = CALxxx_1MHZ = 1MHz
//
//                MSP430G2xx3
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |     P1.2/UCA0TXD|------------>
//            |                 | 9600 - 8N1
//            |     P1.1/UCA0RXD|<------------
//
//   D. Dang
//   Ancn Instruments Inc.
//   February 2012
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
//#include <io430.h>
#include "msp430.h"
#include "system.h"
#define RxdBufSize 	32

struct ED_Device ED;
unsigned char HoldReg[64];
//unsigned char ED_Reg[32];
//unsigned char EdReg[16];
//unsigned char PgReg[8][8];

unsigned char ED_RunSta = 0;
unsigned int WRTU_LinkCnt = 1; 
unsigned char WRTU_LinkUP = 1;//0：成功 >1失败
//unsigned char WRTU_BC = 1;//0：成功 >1失败
/* unsigned char ActMsgSta = 0x00;通过定时器来发送ActMsg消息 */

unsigned char ActMsgFlag = 0;
unsigned int ActMsgCnt = 0;
unsigned int ActMsgEn = 0;
unsigned char ADCFlag = 0;
unsigned int ADCCnt = 0;
int long IntDegF;
int long IntDegC;
unsigned char InitFlag = 0;
unsigned int InitCnt = 0;
unsigned char TxFlag=1;
unsigned char RxFlag=1;
unsigned int XbeeBusy = 0;
/* 用于标记模块是否在AT模式 大于0是代表活跃的秒数，由定时器自检 */
/* 模块操作时一般是主动退出AT模式，还有未能主动退出的则依靠本变量实现自动退出。 */

//unsigned char TempFlag=0;/* 片内温度传感器采集标记 */
unsigned int TimerCnt0 = 0;/*  */
unsigned int TimerUp = 0;/*  */
unsigned int TimerCtrl = 0;/*  */
unsigned int TimerCntRun = 0;/*  */
unsigned char RxdOk = 0;/* 接收数据OK标记 */
unsigned char RxdCnt = 0;/* 接收数据的计数 每一次接收会清空 */
unsigned char RxdCntOld = 0;
unsigned char RxdState = 0;/* 接收数据OK标记 */

unsigned char RxdData[RxdBufSize];/* 用于AT命令的接收缓冲区 */
unsigned char FlashChk = 0x00;/* 标记Flash参数是否正确 */

struct Net_Farme MsgTxd;
struct Net_Farme MsgRxd;

/*********************************************************************************************************
** Function name:       SystemClock
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void SystemClock()
{
  if (CALBC1_1MHZ==0xFF)	// If calibration constant erased
  {											
    while(1);                               // do not load, trap CPU!!	
  }
  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;
}
/*********************************************************************************************************
** Function name:       SystemPortConfig
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void SystemPortConfig()
{
 P1OUT=0x00;
 P1DIR=0x01;
 P1IES=0x00;
 P1IE=0x00;
 P1SEL=0x00;
 P2OUT=0x00;
 P2DIR=0x38;
 P2IES=0x00;
 P2IE=0x00;
 P2SEL=0x00;
 P3OUT=0x00;
 P3DIR=0x00;
 P3SEL=0x00;

}

/*********************************************************************************************************
** Function name:       main
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  _DINT(); 
  SystemClock();
  SystemPortConfig();
  XBEE_ON;
  TimeraInit(); 
  TimerSrvRun();
  TimerSrvBusy();//启动AT模式的计数器用于标记Xbee模块是否处于AT模式
  usart0_init();
  _EINT();
  
  //LongDelay(2); 
  //Net_Init();
  
  ED.eIP = 0x01;
  ED.rIP = 0xF0;
  
  
 // UartWriteStr("1234567890");
  //__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
  while(1)
  {
    /*DelayTime(200);      
    //Chk_Link();
    //Chk_Act();
	
	if((ED_RunSta==2)&&(WRTU_LinkUP==0))
       	{
              	//At_Da_Set(ED.RT_MACH,ED.RT_MACL,0x01);
              	ED_RunSta = 5;
           	}
   
        if((ActMsgFlag)&&(ED_RunSta == 5))
	    {
        	DelayTime(50);
        	ActMsgFlag = 0;
    	}
  */
  }
}


