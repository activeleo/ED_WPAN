
 
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
* ��    �ƣ�UartLpm
* ��    �ܣ�������/���ȴ������У���ʱ��ϵͳ�Ĳ���ʱ�Ӹùص�������ʡ��
* ��ڲ�������
* ���ڲ�������
* ˵    ��: �������������ʱ�ӳ�ͻ����ע�͵��ú������������ӹ��ġ�
****************************************************************************/
void UartLpm()
{
   /* if(U0TCTL&SSEL1) LPM3;  //����ACLK ��ʱ�ӣ�����LPM3����(����ACLK)
    else             LPM0;  //����SMCLK��ʱ�ӣ�����LPM0����(���ر�SMCLK)
  */
}
/****************************************************************************
* ��    �ƣ�UartWriteChar
* ��    �ܣ��򴮿�дһ���ֽ�����
* ��ڲ�����c:  �����͵�һ�ֽ�����
* ���ڲ�������d vwc
* ˵    ��: �ڵȴ�������ϵĹ����У�CPU������
****************************************************************************/
void UartWriteChar(char c)
{ 
   // while (TxFlag==0) UartLpm();  // �ȴ���һ�ֽڷ��꣬������
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
if(XbeeBusy)//ģ�鴦��ATģʽ
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
else //����ģʽ
{
	if(RxdState==0)//��ʼ�����յ�һ֡����
	{
	RxdCnt = 0;
	UartClearRxd();
	RxdData[RxdCnt] = UCA0RXBUF;
	RxdState = 1;//���յ�ַ���
	}
	else if(RxdState == 1)
	{
        RxdCnt++;  
	RxdData[RxdCnt] = UCA0RXBUF;
	}

}
}
