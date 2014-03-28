#include "system.h"


#define FlashAdd 0x1000
#define FlashXbee 0x1010
char  value;                                // 8-bit value to write to segment A

unsigned long DataH,DataL; /* 该变量只要保存返回的数据，使用前请调用ClearRxd */
extern unsigned int XbeeBusy;
 
unsigned char XbeeATTime = 0;


// Function prototypes
void  write_SegA (char value);
void  copy_A2B (void);
/*--------------------------------------------------------------------------*/
/* 简单的无线节点具有 
ID--PANID
ZS--Zigbee
CE--Cood
SH--Extended Address.
SL--Extended Address.
MY--16 bit Network Address
DH--destination extended address
DL--destination extended address
DB--RSSI
*/
/*--------------------------------------------------------------------------*/


/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char HexToChar(unsigned char bChar)
{
    if((bChar>=0x30)&&(bChar<=0x39))
        bChar -= 0x30;
    else if((bChar>=0x41)&&(bChar<=0x46))//大写字母
        bChar -= 0x37;
    else if((bChar>=0x61)&&(bChar<=0x66))//小写字母
        bChar -= 0x57;
    else bChar = 0xff;
    return bChar;
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char CharToHex(unsigned  char bHex)
{
    //if((bHex>=0)&&(bHex<=9))
  if((bHex<=9))
        bHex += 0x30;
    else if((bHex>=10)&&(bHex<=15))//大写字母
        bHex += 0x37;
    else bHex = 0xff;
    return bHex;
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void ClearRxd()
{
  unsigned char i = 0;

  for(i=0;i<32;i++)
    RxdData[i] = '\0';
  RxdCnt = 0;
  RxFlag = 1;//start recive 0x0D
  DataH = 0;
  DataL = 0;
}
/*********************************************************************************************************
** Function name:       At_Busy
** Descriptions:   	设定模块的活跃
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void At_Busy()
{
 if((XbeeATTime)>10)
  {
    XbeeBusy = (XbeeATTime)*10;// Delay CT=*100mS XbeeBusy=100×100mS
  }
  else
  {
    XbeeBusy = 100;// Delay 10S Timer--100*100MS=10Sec
  }
}

/*********************************************************************************************************
** Function name:       process
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void process()
{
  uint32_t TEMP = 0;
  ClearRxd();
  At_Busy();
  if(At_Init())
  {
   At_Busy();  

  //ED.RT_IP
  At_Msg_Get("atsh",100,&(ED.eMACH),1);/* 获取数据atsh-->SH中 */
  At_Msg_Get("atsl",100,&(ED.eMACL),1);
  At_Msg_Get("atdh",100,&(ED.dMACH),1);
  At_Msg_Get("atdl",100,&(ED.dMACL),1);
  //At_Msg_Get("atid",100,(uint32_t *)&(ED.PAN_ID),1);
  //At_Msg_Get("atce",100,(uint32_t *)&(ED.ED_CE),1);//指针访问取对应变量的地址
  //At_Msg_Get("atsc",100,(uint32_t *)&(ED.ED_SD),1);
  //At_Msg_Get("atct",100,(uint32_t *)&(ED_Reg[ACT]),1);//Command Mode Timeout
  At_Msg_Get("atid",100,&(TEMP),1);
  ED.xPAN = TEMP;
  //ED_Reg[PANID] = TEMP;
  At_Msg_Get("atce",100,&(TEMP),1);
  //ED.ED_CE = TEMP;
  //ED_Reg[CE] = TEMP;
  At_Msg_Get("atsd",100,&(TEMP),1);
  //ED.ED_SD= TEMP;
  //ED_Reg[SD] = TEMP; 
  At_Msg_Get("atct",100,&(TEMP),1);
  //ED_Reg[ACT] = TEMP;  
  At_Busy();
  ClearRxd();
  At_Exit();
   }
 /*
  设定对应的参数，首先先设定参数，其次读取参数，确认成功设定。
  
  At_Msg_Set("atid",100,4,1);
  ClearRxd();
  At_Msg_Get("atid",100,&ID,1);
  ClearRxd(); */ 
  /*At_Msg_Set("atdh",100,0x13A200,1);
  ClearRxd();
  At_Msg_Get("atdh",100,&DH,1);
  ClearRxd(); 
  At_Msg_Set("atdl",100,0x4076E9CB,1);
  ClearRxd();
  At_Msg_Get("atdl",100,&DL,1);
  ClearRxd();*/  
  
}
/*********************************************************************************************************
** Function name:       At_Init
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char At_Init()
{ 
  ClearRxd();
  At_Busy();
  UartWriteStr("+++");

 /* UartWriteChar('+');
  UartWriteChar('+');
  UartWriteChar('+');
  UartWriteChar('\r'); */
  //P2OUT ^= 0x10;  
  //TimerSrv(100);//5Sec 
  LongDelay(5);
  //P2OUT ^= 0x10;  
  if((RxdData[0]==0x4F)&&(RxdData[1]==0x4B)&&(RxdData[2]==0x0D))
  { 
    return 1;
  }
  else
  {
    return 0;  
  }  
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char At_Exit()
{

  /*UartWriteStr("atcn");// 
  TimerSrv(500);//5Sec*/
  ClearRxd();
  DelayTime(1000);
  At_Write("atcn",200);
  DelayTime(1000);
  if((RxdData[0]==0x4F)&&(RxdData[1]==0x4B)&&(RxdData[2]==0x0D))
  { 
    XbeeBusy = 10;
     DelayTime(5000);
    return 1;
  }
  else
    return 0; 
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char At_Reset()//Software Reset
{
  UartWriteStr("atfr");// 
  TimerSrv(100);//1Sec 
  if((RxdData[0]==0x4F)&&(RxdData[1]==0x4B)&&(RxdData[2]==0x0D))
    return 1;
  else
    return 0; 
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char At_NetReset()//Resets network
{
  UartWriteStr("atnr");//
 
  TimerSrv(100);//1Sec
 
  if((RxdData[0]==0x4F)&&(RxdData[1]==0x4B)&&(RxdData[2]==0x0D))
    return 1;
  else
    return 0; 
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char At_ResDef()//Restore Defaults
{
  UartWriteStr("atre");//
 
  TimerSrv(5);//5Sec
 
  if((RxdData[0]==0x4F)&&(RxdData[1]==0x4B)&&(RxdData[2]==0x0D))
    return 1;
  else
    return 0; 
}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char At_WR()
{
  UartWriteStr("ATWR");//
 
  TimerSrv(100);//1Sec
 
  if((RxdData[0]==0x4F)&&(RxdData[1]==0x4B)&&(RxdData[2]==0x0D))
    return 1;
  else
    return 0;    
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char At_Write(unsigned char *cmd,unsigned char delay)
{

    while(*cmd)
    {
        UartWriteChar(*cmd++);
         DelayTime(100);
    }
    DelayTime(200);
    UartWriteChar('\r');
    DelayTime(200);
    //UartWriteChar('\n');//Add By leo
      
    TimerSrv(delay);       
    return 1;
}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char At_Msg_Proc()
{
  unsigned char i,j;
  char prodata[32];  
  
if(RxFlag == 0)
{
    for(i=0;i<(RxdCnt-1);i++) //倒序反转接收的数据
    {
    //prodata[i] = RxdData[RxdCnt-i-2];
      prodata[i] = RxdData[i];
    }//--rxd:1234567-->7654321-->RxdCnt-2=7(len) 0xabcd
    
    if(((RxdCnt-1)>8)&&((RxdCnt-1)<17))//大于32位0xFFFFFFFF 分两个int处理
    {
      for(j=0;j<8;j++)
      {
        DataL |= HexToChar(prodata[j]);
     
        if(j<7)
          DataL = DataL<<4;//rt_kprintf("data:%d\n",data);				
      }

      for(j=8;j<(RxdCnt-1);j++)
      {
	DataH |= HexToChar(prodata[j]);

	if(j<(RxdCnt-2))
          DataH = DataH<<4;//rt_kprintf("data:%d\n",data);				
      } 
      return 2;     
    }
    else if((RxdCnt-1)<9)
    {
      for(j=0;j<(RxdCnt-1);j++)
      {
        DataL |= HexToChar(prodata[j]);
     
        if(j<(RxdCnt-2))
          DataL = DataL<<4;//rt_kprintf("data:%d\n",data);				
      }
      return 1;
    }
    else
    {
      //return ((RxdCnt-2)/8+1)
      return 0;//接收数据超长
    }
}
return 0;
  //write_flash(&prodata[0],FlashAdd,RxdCnt-3);
  //write_flash(&RxdData[0],FlashXbee,RxdCnt-2);

}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void At_Msg_Get(unsigned char *cmd ,unsigned int delay,uint32_t *data,unsigned char wr )
{
  //unsigned char RetCode = 0;
   ClearRxd(); 
    At_Write(cmd,delay);//send data
      DelayTime(1000);
  while((TimerCnt0>>1))DelayTime(1000);//wait data ok or timeout
  if(TimerCnt0)//rxd ok
  {
   // RetCode = ansys();
    At_Msg_Proc();
    *data = DataL;
  }   
 /* if(wr)
  {
   write_flash(DataL,FlashXbee,sizeof(write));
  }*/

}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char  At_Msg_Set(unsigned char *cmd ,unsigned char delay,uint32_t data,unsigned char wr )
{
  char WriteData[8];
  
 /* At_Write(cmd,delay);//send data*/
  //DH!=0x13A200)&&(DL!=0x04076E9D1)
  
    while(*cmd)
    {
        UartWriteChar(*cmd++);
         DelayTime(100);
    }
    //41 54 44 4C 34 30 37 36 45 39 43 42 0D 
    //41 54 44 48 31 33 41 32 30 30 0D
    //sprintf(WriteData,"%lx",data);
    //CharToHex();
    WriteData[7] = CharToHex((data&0x0F));
    WriteData[6] = CharToHex(((data>>4)&0x0F));
    WriteData[5] = CharToHex(((data>>8)&0x0F));
    WriteData[4] = CharToHex(((data>>12)&0x0F));
    WriteData[3] = CharToHex(((data>>16)&0x0F));
    WriteData[2] = CharToHex(((data>>20)&0x0F));
    WriteData[1] = CharToHex(((data>>24)&0x0F));
    WriteData[0] = CharToHex(((data>>28)&0x0F));    
    
    DelayTime(500);
    UartWriteStr(WriteData);
    DelayTime(500);
    UartWriteChar('\r');
    //DelayTime(500);
    //UartWriteChar('\n');

    TimerSrv(delay);
    
if((RxdData[0]==0x4F)&&(RxdData[1]==0x4B)&&(RxdData[2]==0x0D))
  return 1;
else
  return 0;    
}





/*--------------------------------------------------------------------------*/
