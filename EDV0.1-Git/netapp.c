/**
  ******************************************************************************
  * @file    netapp.c
  * @author  ANCN@Leo
  * @version V1.0.1
  * @date    03/17/2014
  * @brief   source file for netapp.c module.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 ANCN</center></h2>
  */ 

#include "system.h"
 
extern unsigned char ED_Reg[32];
extern struct ED_Device ED;
extern struct Net_Farme MsgTxd;
extern struct Net_Farme MsgRxd;
const unsigned char auchCRCHi[] = 
{
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40
};

const unsigned char auchCRCLo[] = 
{
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
	0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
	0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
	0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
	0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
	0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
	0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
	0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
	0x40
};

unsigned int CRC16(unsigned char *pushMsg, unsigned int usDataLen)
{
	unsigned char uchCRCHi = 0xFF;	// 高CRC字节初始化
	unsigned char uchCRCLo = 0xFF;	// 低CRC 字节初始化
	unsigned int uIndex =0;      		// CRC循环中的索引
	
	while(usDataLen--)		// 传输消息缓冲区
	{
    uIndex = uchCRCHi ^ *pushMsg++;	// 计算CRC
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex]; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	}
	
	return (uchCRCHi << 8 | uchCRCLo);
}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
********************************************************************************************************
void ClearTxd()
{
  unsigned char i = 0;
  for(i=0;i<Data_Length;i++)
  {
  DevMsg.DATA[i]='\0';
  }
  DevMsg.CRC = 0;
}*/

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void MsgClear(unsigned char MsgType)
{
  unsigned char i = 0;
  if(MsgType==Txd)
  {
  	MsgTxd.DA = 0x00;
  	MsgTxd.FC = 0x00;
  	MsgTxd.SD = 0x0000;
  	MsgTxd.LN = 0x0000;
 	for(i=0;i<Data_Length;i++)
  	{
  	MsgTxd.DATA[i]='\0';
  	}
  	MsgTxd.CRC = 0x0000;
  }
  else if(MsgType==Rxd)
  {
  	MsgRxd.DA = 0x00;
  	MsgRxd.FC = 0x00;
  	MsgRxd.SD = 0x0000;
  	MsgRxd.LN = 0x0000;
 	for(i=0;i<Data_Length;i++)
  	{
  	MsgRxd.DATA[i]='\0';
  	}
  	MsgRxd.CRC = 0x0000;
  }
}

/*extern unsigned int XbeeBusy;
void Set_DA()
{
  At_Init();
    if((DH!=0x13A200)&&(DL!=0x4076E9DF))
    {
    At_Da_Set(0x13A200,0x4076E9DF,1);
    At_WR();
    }
      DelayTime(5000);
   At_Exit();
}*/

/*********************************************************************************************************
** Function name:       
** Descriptions:       
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
/* 设计说明
1：初始化网络相关的流程：无线模块的初始化、网络注册、网络配置服务等。
*/
unsigned char  Net_Init()
{
  rt_uint32_t TEMP = 0;

 At_Busy();//设定超时计数
 if(At_Init())//进入at模式
 {
    	At_Busy();
    	//At_Msg_Get("atid",100,(uint32_t *)&(ED.PAN_ID),1);       // PANID
    	At_Msg_Get("atid",100,&TEMP,1);
    	ED.xPAN = TEMP;
    	At_Msg_Get("atsc",100,&TEMP,1);
    	ED.xSC = TEMP;
		
	At_Busy();DelayTime(50);
  	At_Msg_Get("atsd",100,&(TEMP),1);
  	ED.xSD= TEMP;
  	At_Msg_Get("atce",100,&(TEMP),1);
  	ED.xCE= TEMP;
	
	At_Busy();DelayTime(50);
  	At_Msg_Get("atpl",100,&(TEMP),1);
  	ED.xPL= TEMP;
  	At_Msg_Get("atpm",100,&(TEMP),1);
  	ED.xPM= TEMP;

	At_Busy();DelayTime(50);
  	At_Msg_Get("atsp",100,&(TEMP),1);
  	ED.xSP= TEMP;
  	At_Msg_Get("atsn",100,&(TEMP),1);
  	ED.xSN= TEMP;
	At_Busy();DelayTime(50);
  	At_Msg_Get("atsm",100,&(TEMP),1);
  	ED.xSM= TEMP;
  	At_Msg_Get("atst",100,&(TEMP),1);
  	ED.xST= TEMP;
  	At_Msg_Get("atsn",100,&(TEMP),1);
  	ED.xSN= TEMP;

  	At_Busy();  
  	DelayTime(50);
  	At_Msg_Get("atsh",100,&(ED.eMACH),1);       // 获取无线模块的地址 
  	DelayTime(50);
  	At_Msg_Get("atsl",100,&(ED.eMACL),1); 
  	DelayTime(50);
  	At_Busy();
  	At_Msg_Get("atdh",100,&(ED.dMACH),1);       // 获取目标地址
  	DelayTime(50);
  	At_Msg_Get("atdl",100,&(ED.dMACL),1);    
  	At_Busy();   

	
    	//At_Msg_Get("atdb",100,(uint32_t *)&DB,1);       // RSSI  
    	//At_Msg_Get("atpl",100,(uint32_t *)&PL,1);       // 电源级别 
    	//ED.NST = PL;		/*网络状态*/
    	//ED.NRS = DB;		/*信号质量*/

    	At_Busy();//debug--->CE=0x13A200,0x04076E9DF   
    	DelayTime(5000);
    	At_Exit();
		
	
	 	
	return 0;
  }
 else
 {
 	return 1;
 }
  //检查模块的模式是否合法：注意不是网关模式
  /*if(CE!=0x00)
  {
    At_Msg_Set("atce",100,0,1);
    ClearRxd();
    At_Msg_Get("atce",100,(uint32_t *)&CE,1);
    ClearRxd();
  }*///固定设置模块为路由模式--2013-12-06 注意没有加入电源优化
 
}
/*********************************************************************************************************
** Function name:       	MsgProcess
** Descriptions:		主要的接收流程  
** input parameters:	None    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
void MsgProcess()
{

if(MsgRecive(RxdData[0]))//接收第一个字节为地址的数据信息
{
  MsgAction(MsgRxd.FC);//pFC
}
}
/*********************************************************************************************************
** Function name:       	MsgRecive
** Descriptions:		None   
** input parameters:	None    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
rt_uint8_t MsgRecive(rt_uint8_t ED_ID)
{

if((ED.eIP == ED_ID)&&(CRC16(RxdData,(RxdCnt+1))==0))
{
	if(MsgCopyToRxd(RxdCnt+1))//复制接收缓冲区的数据到消息区
	{

          UartClearRxd();//释放接收缓冲区
          return 1;
	}
        else
        {
        UartClearRxd();
        return 0;
        }

}
return 0;

}

/*********************************************************************************************************
** Function name:       	MsgRecive
** Descriptions:		None   
** input parameters:	None    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
rt_uint8_t MsgCopyToRxd(rt_uint8_t pLen )
{
rt_uint8_t i = 0;

  switch (RxdData[1])
  {
  case 0x03://读取保持寄存器
    if(pLen<=0x20)
    {
      MsgRxd.DA = RxdData[0];
      MsgRxd.FC = RxdData[1];
      MsgRxd.SD = (RxdData[2]<<8)|RxdData[3];	
      MsgRxd.LN = (RxdData[4]<<8)|RxdData[5];

      for(i=0;i<(pLen - 8);i++)
      {
        MsgRxd.DATA[i] = RxdData[6+i];
      }
    }
    MsgRxd.CRC = (RxdData[pLen-1]<<8)|RxdData[pLen];
    break;
  case 0x06://读取保持寄存器
    if(pLen<=0x20)
    {
      MsgRxd.DA = RxdData[0];
      MsgRxd.FC = RxdData[1];
      MsgRxd.SD = (RxdData[2]<<8)|RxdData[3];	
      //MsgRxd.LN = (RxdData[4]<<8)|RxdData[5];

      for(i=0;i<(pLen - 6);i++)
      {
        MsgRxd.DATA[i] = RxdData[4+i];
      }
    }
    MsgRxd.CRC = (RxdData[pLen-1]<<8)|RxdData[pLen];
    break;
   case 0x10://读取保持寄存器
    if(pLen<=0x20)
    {
      MsgRxd.DA = RxdData[0];
      MsgRxd.FC = RxdData[1];
      MsgRxd.SD = (RxdData[2]<<8)|RxdData[3];	
      MsgRxd.LN = (RxdData[4]<<8)|RxdData[5];

      for(i=0;i<(pLen - 8);i++)
      {
        MsgRxd.DATA[i] = RxdData[6+i];
      }
    }
    MsgRxd.CRC = (RxdData[pLen-1]<<8)|RxdData[pLen];
    break;
    case 0x01://设定多个保持寄存器
   
      MsgRxd.DA = RxdData[0];
      MsgRxd.FC = RxdData[1];
      MsgRxd.SD = (RxdData[2]<<8)|RxdData[3];	
      MsgRxd.LN = (RxdData[4]<<8)|RxdData[5];

      for(i=0;i<(pLen - 8);i++)
      {
        MsgRxd.DATA[i] = RxdData[6+i];
      }
    
    	MsgRxd.CRC = (RxdData[pLen-1]<<8)|RxdData[pLen];
		
	if(MsgRxd.SD==0x0000)
	  {
	   TestReg();
	  }	
       else if(MsgRxd.SD==0x0001)
      	  {
      		UpLoadMsg(0x03,0x00,0x10);
         }
	 else if(MsgRxd.SD==0x0002)
	  {
		UpLoadMsg(0x03,0x08,0x8);
	  }

      return 0;
    break;
  default:
    if(pLen<=0x20)
    {
      MsgRxd.DA = RxdData[0];
      MsgRxd.FC = RxdData[1];
      MsgRxd.SD = (RxdData[2]<<8)|RxdData[3];	
      MsgRxd.LN = (RxdData[4]<<8)|RxdData[5];
      for(i=0;i<(pLen - 8);i++)
      {
        MsgRxd.DATA[i] = RxdData[6+i];
      }
    }
    MsgRxd.CRC = (RxdData[pLen-1]<<8)|RxdData[pLen];
    break;
}

return 1;

}


/*********************************************************************************************************
** Function name:       	MsgAction
** Descriptions:		None   
** input parameters:	None    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
rt_uint8_t MsgAction(rt_uint8_t pFC)
{
  switch (pFC)
  {
  case 0x03://读取保持寄存器
    if(ReadReg(MsgRxd.SD,MsgRxd.LN,&(MsgTxd.DATA[1])))//
    {
      MsgResponse(0x03,MsgRxd.LN);// Get:LN=Num
    }
    else
    {
     return 0;
    }
    break;
  case 0x06://设定单个保持寄存器
    if(WriteReg(MsgRxd.SD,0x01,MsgRxd.DATA))
    {
      MsgResponse(0x06,0x01);// Get:LN=Num
    }
    else
    {
     return 0;
    }
    break;
  case 0x10://设定多个保持寄存器
    if(WriteReg(MsgRxd.SD,MsgRxd.LN,&(MsgRxd.DATA[1])))
    {
      MsgResponse(0x10,MsgRxd.LN);// Get:LN=Num
    }
    else
    {
     return 0;
    }
    break;

  default:
    MsgResponse(pFC,8);
    break;
}

  return 0;
}
/*********************************************************************************************************
** Function name:       	CheckReg
** Descriptions:		检查寄存器的访问地址是否正确  
** input parameters:	None    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
rt_uint8_t CheckReg(rt_uint8_t pSD,rt_uint8_t pLN)
{
  if((pLN<=0x10)&&(pSD<=EDRegSize))//一次最多读取的为32个寄存器
    return 1;
  else
    return 0;
}
/*********************************************************************************************************
** Function name:       	CheckReg
** Descriptions:		检查寄存器的访问地址是否正确  
** input parameters:	None    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
void TestReg( )
{
rt_uint8_t i = 0; 
  for(i=0;i<64;i++)
  {
  HoldReg[i] = i;
  }
}
/*********************************************************************************************************
** Function name:       	ReadReg
** Descriptions:		获取寄存器的数据   
** input parameters:	None    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
rt_uint8_t ReadReg(rt_uint8_t pSD,rt_uint8_t pLN,rt_uint8_t *pBuf)
{
rt_uint8_t i = 0;
  if(CheckReg(pSD,pLN))//地址和范围合法
  {
    for(i=0;i<pLN;i++)
    {
      *pBuf =HoldReg[pSD+i];
      *pBuf++;
    }
    return 1;
  }
  
  return 0;
}
/*********************************************************************************************************
** Function name:       	WriteReg
** Descriptions:		设定保持寄存器的数据   
** input parameters:	None    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
rt_uint8_t WriteReg(rt_uint8_t pSD,rt_uint8_t pLN,rt_uint8_t *pBuf)
{
  rt_uint8_t i = 0;
  rt_uint8_t pPage_H = 0,pPage_L = 0;
  if(CheckReg(pSD,pLN))//地址和范围合法
  {
  	if((pSD!=0x07) )
		{
    		for(i=0;i<pLN;i++)
    			{
     			 	//*pBuf = HoldReg[pSD+i];
     			 	//*pBuf++;
      				HoldReg[pSD+i]=*pBuf;
      				*pBuf++;
    			}
    		return 1;
  		}//不设定页寄存器
  		else if((pSD==0x07)&&(MsgRxd.DATA[0]!=0x00))//页寄存器的写入操作
  		{
  		pPage_L = (MsgRxd.DATA[0])&0x0F;
                pPage_H = (MsgRxd.DATA[0])&0xF0;
    		for(i=0;i<pLN;i++)
    			{
      				HoldReg[i+16+((pPage_L-1)*8)]=*pBuf;
      				*pBuf++;
    			}
		SyncToFlash(pPage_H,pPage_L);
    		return 1;		
		}
  }
  return 0;
}

void SyncToFlash(rt_uint8_t pOptr, rt_uint8_t pPAGE )
{
rt_uint8_t PAGE = 0,ADD = 0,LEN = 0;

PAGE = (pPAGE-1);//pPAGE = 1.2.....
ADD = 16+((pPAGE-1)*8);
LEN = 8;
WriteConfigToFlash(pOptr,PAGE,LEN,&HoldReg[ADD]);

}

/*********************************************************************************************************
** Function name:       	MsgSend
** Descriptions:		发送信息
** input parameters:	None    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
rt_uint8_t MsgSend(rt_uint8_t pFC,rt_uint8_t pLN,rt_uint8_t *pBuf)
{
 unsigned char TxdBuf[24];
 rt_uint8_t i = 0;
   switch (pFC)
  {
  case 0x03://读取保持寄存器
    TxdBuf[0] = MsgTxd.DA;
    TxdBuf[1] = MsgTxd.FC;
    for(i=0;i<pLN;i++)
      {
      TxdBuf[2+i] = *pBuf;//TxdBuf[6+i] = *pBuf;
      *pBuf++;
      }
    MsgTxd.CRC = CRC16(TxdBuf,pLN+2);//8+1+pLN
    TxdBuf[2+pLN+0] = MsgTxd.CRC>>8;
    TxdBuf[2+pLN+1] = MsgTxd.CRC;
    MsgClear(Txd);
    UartWriteStrNum((char *)TxdBuf,pLN+4);//强制类型转换
    break;
  case 0x06://设定单个保持寄存器
    TxdBuf[0] = MsgTxd.DA;
    TxdBuf[1] = MsgTxd.FC;
    TxdBuf[2] = MsgTxd.SD>>8;
    TxdBuf[3] = MsgTxd.SD;
    for(i=0;i<pLN;i++)
      {
      TxdBuf[4+i] = *pBuf;//TxdBuf[6+i] = *pBuf;
      *pBuf++;
      }
    MsgTxd.CRC = CRC16(TxdBuf,pLN+4);//8+1+pLN
    TxdBuf[4+pLN+0] = MsgTxd.CRC>>8;
    TxdBuf[4+pLN+1] = MsgTxd.CRC;
    MsgClear(Txd);
    UartWriteStrNum((char *)TxdBuf,pLN+4+2);//强制类型转换
    break;
  case 0x10://设定多个保持寄存器
    TxdBuf[0] = MsgTxd.DA;
    TxdBuf[1] = MsgTxd.FC;
    TxdBuf[2] = MsgTxd.SD>>8;
    TxdBuf[3] = MsgTxd.SD;
    TxdBuf[4] = MsgTxd.LN>>8;
    TxdBuf[5] = MsgTxd.LN;
   
    MsgTxd.CRC = CRC16(TxdBuf,pLN+4);//8+1+pLN
    TxdBuf[4+pLN+0] = MsgTxd.CRC>>8;
    TxdBuf[4+pLN+1] = MsgTxd.CRC;

    MsgClear(Txd);
    UartWriteStrNum((char *)TxdBuf,pLN+4+2);//强制类型转换  
    break;    
   default:
    TxdBuf[0] = MsgTxd.DA;
    TxdBuf[1] = MsgTxd.FC;
    TxdBuf[2] = MsgTxd.SD>>8;
    TxdBuf[3] = MsgTxd.SD;
    TxdBuf[4] = MsgTxd.LN>>8;
    TxdBuf[5] = MsgTxd.LN;
    MsgTxd.CRC = CRC16(TxdBuf,pLN-2);//8+1+pLN
    TxdBuf[pLN-2+0] = MsgTxd.CRC>>8;
    TxdBuf[pLN-2+1] = MsgTxd.CRC;
    MsgClear(Txd);
    UartWriteStrNum((char *)TxdBuf,pLN);//强制类型转换    
    break;
  } 
  return 0;
}

/*********************************************************************************************************
** Function name:       	MsgResponse
** Descriptions:		响应函数  
** input parameters:	pLN:数据区域的长度    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
rt_uint8_t MsgResponse(rt_uint8_t pFC,rt_uint8_t pLN)
{
  switch (pFC)
  {
  case 0x03://读取保持寄存器
      MsgTxd.DA = MsgRxd.DA;
      MsgTxd.FC = MsgRxd.FC;
      MsgTxd.SD = MsgRxd.SD;
      MsgTxd.LN = MsgRxd.LN;
      MsgTxd.DATA[0] = pLN;
      MsgSend(pFC,pLN+1,&(MsgTxd.DATA[0]));//PLN=RXD.LN+1
    break;
  case 0x06://设定单个保持寄存器
      MsgTxd.DA = MsgRxd.DA;
      MsgTxd.FC = MsgRxd.FC;
      MsgTxd.SD = MsgRxd.SD;
      MsgTxd.LN = 0x01;//固定为单个的保持寄存器
       
      ReadReg((MsgTxd.SD),(MsgTxd.LN),&(MsgTxd.DATA[0]));
      MsgSend(pFC,pLN,&(MsgTxd.DATA[0]));//PLN=RXD.LN+0
    break;
  case 0x10://设定多个保持寄存器
      MsgTxd.DA = MsgRxd.DA;
      MsgTxd.FC = MsgRxd.FC;
      MsgTxd.SD = MsgRxd.SD;
      MsgTxd.LN = MsgRxd.LN;//固定为单个的保持寄存器
      pLN = 2;
      //ReadReg((MsgTxd.SD),(MsgTxd.LN),&(MsgTxd.DATA[0]));
      MsgSend(pFC,pLN,&(MsgTxd.DATA[0]));//PLN=RXD.LN+0
    break;
  default:
      MsgTxd.DA = MsgRxd.DA;
      MsgTxd.FC = MsgRxd.FC;
      MsgTxd.SD = MsgRxd.SD;
      MsgTxd.LN = MsgRxd.LN;
      MsgSend(pFC,pLN,&(MsgTxd.DATA[0]));//PLN=RXD.LN+1
    break;
}
MsgClear(Rxd);
  return 0;
}

/*********************************************************************************************************
** Function name:       	UpLoadMsg
** Descriptions:		主动上传寄存的信息   
** input parameters:	pLN:数据区域的长度    
** output parameters:	None  
** Returned value:  	None    
*********************************************************************************************************/
rt_uint8_t UpLoadMsg(rt_uint8_t pFC,rt_uint8_t pSD,rt_uint8_t pLN)
{
MsgClear(Rxd);
if((pFC==0x03)&&((pSD+pLN)<=64)&&(pLN<=16))
{
      	MsgRxd.DA = ED.eIP;
      	MsgRxd.FC = pFC;
      	MsgRxd.SD = pSD;
	MsgRxd.LN = pLN;
	MsgAction(pFC);
	MsgClear(Rxd);
	return 1;
}
return 0;
}

