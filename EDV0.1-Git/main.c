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
/*	G2553-EDV0.1
日期：2014-3-28  
1: modbus相似版本已经完成调试，主动发送数据部分还没有完善。

日期：2014-3-31 
1: 休眠和LCD部分已经增加和修改完毕，主动休眠部分没有增加。
日期：2014-4-1 
1: 所有基本功能已经完成，休眠部分已经完成。没有启用休眠，调试状态 ！
日期：2014-4-2 
1: Flash操作部分已经完成，可以进行数据采集操作！
*/
//#include <io430.h>
#include "msp430.h"
#include "system.h"
#define RxdBufSize 	32

struct ED_Device ED;
unsigned char HoldReg[64];
unsigned char ED_RunSta = 0;
unsigned int WRTU_LinkCnt = 1; 
unsigned char WRTU_LinkUP = 1;//0：成功 >1失败
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

rt_uint8_t XbeePowerStatus = 0;         //电源状态：1=Active 0=Sleep   
rt_uint8_t SleepRunStatus = 0;          //当前状态：1=Active 0=Sleep 
rt_uint8_t SleepReqStatus = 0;          //申请状态：1=Active 0=Sleep   
rt_uint8_t SleepTimeSet = 0;            //休眠的时间参数

extern char ReadFlash[8];
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
 
  InitKey();
  InitPowerContrl();
  InitSleepContrl();
}

void InitKey(  )
{//p2.1 p2.2 p3.2
  //设置输入方式
  P2DIR &=~0x06;
  P3DIR &=~0x04;
  //设定内置上拉电阻
  P2OUT |=0x06;
  P3OUT |=0x04;
  //开启上拉电阻
  P2REN |=0x06;
  P3REN |=0x04;
}

rt_uint8_t GetKey(  )
{
rt_uint8_t KeyA = 0,KeyB = 0;
//KeyA = ~(P2IN&0x06);
//KeyB = ~(P3IN&0x04);
KeyA = P2IN&0x06;
KeyB = P3IN&0x04;
return (KeyA+(KeyB<<4));

}


void ProcessKey()
{
 rt_uint8_t GetKeyStatus; 
 
 GetKeyStatus = GetKey();
 
if(GetKeyStatus!=0x46)
{
  switch (GetKeyStatus)
  {
  case 0x06:    //Set_Key
    SleepSet(ReqInSleep);
    break;
  case 0x44:    //Add_Key
    SleepSet(ReqExitSleep);
    break;  
  case 0x42:    //Zero_Key
    Adc10RefSet(1,0);
    Adc10Get(10);
    Adc10RefSet(0,0);
    LCD_Display_Detail(1,IntDegC,ADDR_U,12,0);
    break; 
  default:
    break;
  }
}
}

void InitHoldReg()
{
rt_uint8_t i = 0;
for(i=0;i<64;i++)
{
HoldReg[i] = 0xFF;
}
HoldReg[rTY] = 0x01;//0x01 压力传感器
HoldReg[rNS] = 0x3F;// 
HoldReg[rPW] = 0x20;// 
HoldReg[rSN] = 0x31;// 
HoldReg[rST] = 0x64;// 


}
void InitEdReg()
{
 ED.eIP = 0xFF;
ED.cIP =  0xFF;
ED.xPAN = 0xFF;
ED.xSC =  0xFF;
ED.xSD =  0xFF;
ED.xCE =  0xFF;
ED.xFlag =  0xFF;
ED.xSP =  0xFF;
ED.xSN =  0xFF;
ED.xSM = 0xFF;
ED.xST =  0xFF;
ED.xSO =  0xFF;
ED.xPL =  0xFF;
ED.xPM =  0xFF;
ED.xRSSI =  0xFF;

ED.eMACH =  0xFFFFFFFF;
ED.eMACL =  0xFFFFFFFF;
ED.cMACH =  0xFFFFFFFF;
ED.cMACL =  0xFFFFFFFF;
ED.dMACH =  0xFFFFFFFF;
ED.dMACL =  0xFFFFFFFF;


}

void AppProcess()
{
    Adc10RefSet(1,0);
    Adc10Get(10);
    Adc10RefSet(0,0);
    LCD_Display_Detail(1,IntDegC,ADDR_U,12,0);
    UpLoadMsg(0x03,0x00,0x10);
}

//debug-key
 rt_uint8_t Key = 0; 
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
  InitHoldReg();
  InitEdReg();
  LCD_Display();
  XBEE_ON;
  TimeraInit(); 
  TimerSrvRun();
  TimerSrvBusy();//启动AT模式的计数器用于标记Xbee模块是否处于AT模式
  usart0_init();
  InitAdc10();
  
  _EINT();
  

  
  /*ED.eIP = 0x01;
  ED.cIP = 0xF0;*/
  
  read_flash(ReadFlash,0x1000,8);
 if((ReadFlash[0]!=0xFF)&&(ReadFlash[0]<=0xF0))
 {
  InitFlash(0x00);
 }
 else
 {
  LongDelay(1); 
  Net_Init();
  ResetFlash(0xFF);
 }
 
 // UartWriteStr("1234567890");
  //__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
  while(1)
  {
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
   // LongDelay(1);
    /*DelayTime(200);      

        if((ActMsgFlag)&&(ED_RunSta == 5))
	    {
        	DelayTime(50);
        	ActMsgFlag = 0;
    	}
  */
  }
}


