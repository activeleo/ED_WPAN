#include "system.h"

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void TimeraInit()
{

//Clock source=ACLK
//Divider=8
//TIMACLK=125.000KHz
//Frequency=10.0Hz (100.00mS)

 TACTL=0x00; //stop timer
 //channel0
 TACCTL0=0x10;//TACCIE0
 TACCR0=0x30D4;
 //channel1
 TACCTL1=0x00;
 TACCR1=0x00;
 //channel2
 TACCTL2=0x00;
 TACCR2=0x00;
 TAR=0x0000;//clear timer counter
 TACTL=0x2D4;//TASSEL.0,TAID.1,TAID.0,TAMC.0,1

}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
//这里定时服务到时这返回
void TimerSrv(unsigned int Delay)
{
  if(Delay)
    timera_start(TimCH0,Delay);//启动定时器
  while(TimerCnt0&&RxFlag)
  	DelayTime(1000);//如果接收成功RxFlag=0，TimerCnt0 数据接收超时控制
    timera_stop(TimCH0);//关闭定时器
  if(TimerCnt0)
  TimerCnt0 = 1;// rxd ok
  else
  {
  	TimerCnt0 = 0;  //rxd timeout
	RxFlag = 0;
  }
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
//Xbee模块的设定和转发计数
void TimerSrvBusy( )
{
  TimerCtrl |= 0x01<<TimCH1;//启动定时器
//  
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
//基础定时器1S一次-->RUN-LED
void TimerSrvRun( )
{
  TimerCtrl |= 0x01<<TimCH2;//启动定时器
//  
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void timera_start(unsigned int TimCh,unsigned int Delay)
{
  TimerCnt0 = Delay;
  TimerCtrl |= 0x01<<TimCh;
  //TACTL=0x214;//TASSEL.1,TAMC.0,1
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void timera_stop(unsigned int TimCh)
{
  TimerCtrl &= ~(0x01<<TimCh);
  //TACTL=0x00; //stop timer 
}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    N Sec
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void LongDelay(unsigned int CNT)
{
  unsigned char  i = 0;
  for(i=0;i<CNT;i++)
  {
    DelayTime(50000);DelayTime(50000);DelayTime(50000);DelayTime(50000);DelayTime(50000);
  }
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void DelayTime(unsigned int delay)
{
 //volatile unsigned int i;            // volatile to prevent optimization

    do delay--;
    while (delay != 0);// SW Delay    

}



//interrupt handlers 
// Timer A0 interrupt service routine
//#pragma vector=TIMERA0_VECTOR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)//100ms
{
//P2OUT ^= 0x08;                            // Toggle P2.3 
//UartWriteStr("1234567890");// 
 //TACCR0 = 0;
    ProcessKey();
    if((TimerCtrl&0x01)&&(TimerCnt0!=0))//CH0
    {
        TimerCnt0--;
    }
    else
    {
      TimerCnt0 = 0;      
    }
 
    if((TimerCtrl&0x02)&&(XbeeBusy!=0))//CH1
    {    
        XbeeBusy--;
        if(XbeeBusy == 0)
          ClearRxd(); 
    }    
 
    if(TimerCtrl&0x04)//CH2
    {      
        if(TimerCntRun!=0)
        {
          TimerCntRun--;
        }
        else
        {
          TimerCntRun = 10;
         // TempFlag |= 0x01;//采集温度传感器-标记           
        }       
    }
  
    if((XbeeBusy==0)&&(RxdState))//判断接收数据完成
    {
      if(RxdCntOld!=RxdCnt)
        {
          RxdCntOld = RxdCnt;
        }
      else
        {//接收数据完成
          //Net_Process();          
          //ClearRxd();     
          MsgProcess();
          RxdCntOld = 0;
          RxdCnt = 0;
          RxdOk = 0;
          RxdState = 0;
         }
    }

/*-------------------------------------------  
    if(ActMsgCnt<(ED_Reg[ACT]*100))//Act.Msg----send  10*100*100mS=10SEC
    {
        ActMsgCnt ++;
    }
    else
    {
        ActMsgCnt = 0;
        ActMsgFlag = 1;
        //   P2OUT ^= 0x08;                            // Toggle P2.3 
    } */ 
/*-------------------------------------------*/ 
if(InitCnt<100)//10Second
{
InitCnt++;
}
else
{
  
InitCnt = 0;
InitFlag = 1;
} 
/*-------------------------------------------*/ 
}






