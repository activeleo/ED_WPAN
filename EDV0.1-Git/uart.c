
 
#include "system.h"
 

typedef enum
{
    EV_READY,                   /*!< Startup finished. */
    EV_FRAME_RECEIVED,          /*!< Frame received. */
    EV_EXECUTE,                 /*!< Execute function. */
    EV_FRAME_SENT               /*!< Frame sent. */
} eMBEventType;


/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void usart0_init(void)
{

  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                            // 1MHz 9600
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
   
  RxdCnt = 0;
}


/****************************************************************************
* 名    称：UartLpm
* 功    能：串口收/发等待过程中，将时钟系统的部分时钟该关掉，休眠省电
* 入口参数：无
* 出口参数：无
* 说    明: 若与其他外设的时钟冲突，可注释掉该函数，但会增加功耗。
****************************************************************************/
void UartLpm()
{
   /* if(U0TCTL&SSEL1) LPM3;  //若以ACLK 作时钟，进入LPM3休眠(仅打开ACLK)
    else             LPM0;  //若以SMCLK作时钟，进入LPM0休眠(不关闭SMCLK)
  */
}
/****************************************************************************
* 名    称：UartWriteChar
* 功    能：向串口写一个字节数据
* 入口参数：c:  待发送的一字节数据
* 出口参数：无d vwc
* 说    明: 在等待发送完毕的过程中，CPU会休眠
****************************************************************************/
void UartWriteChar(char c)
{ 
   // while (TxFlag==0) UartLpm();  // 等待上一字节发完，并休眠
 while (!(IFG2&UCA0TXIFG));   // TxFlag=0; 
    UCA0TXBUF=c;
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void UartWriteStr(char *s)
{
    while(*s)
    {
       	UartWriteChar(*s++);
	//DelayTime(50);	
    }
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void UartWriteStrNum(char *s,char num)
{
    while(num)
    {
        UartWriteChar(*s++);
        num--;
    }
}
/*********************************************************************************************************
** Function name:       	MsgAction
** Descriptions:		None   
** input parameters:	None    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
void UartClearRxd()
{
unsigned char i=0;
for(i=0;i<RxdBufSize;i++)
{
	RxdData[i] = '\0';
}
RxdCnt = 0;
//RxdState = 0;
}



/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void UartSendChar(char data)
{
 //while (!(IFG2&UTXIFG0));UCA0TXIFG
  while (!(IFG2&UCA0TXIFG));
 UCA0TXBUF = data;
}

//  Echo back RXed character, confirm TX buffer is ready first
//#pragma vector=USART0RX_VECTOR
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
if(XbeeBusy)//模块处于AT模式
{
    if(UCA0RXBUF == 0x0d)//recive 0x0D RxFlag = 0;
      RxFlag = 0;
    if(RxdCnt < RxdSize )
    {
      RxdData[RxdCnt] = UCA0RXBUF;   
      RxdCnt++;
    }
    else
    {
      RxdCnt = 0; 
      RxdData[RxdCnt] = UCA0RXBUF;
    }
}
else //串口模式
{
	if(RxdState==0)//初始化接收第一帧数据
	{
	RxdCnt = 0;
	UartClearRxd();
	RxdData[RxdCnt] = UCA0RXBUF;
	RxdState = 1;//接收地址完成
	}
	else if(RxdState == 1)
	{
        RxdCnt++;  
	RxdData[RxdCnt] = UCA0RXBUF;
	}

}
}
