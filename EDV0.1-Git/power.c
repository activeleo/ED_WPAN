/**
  ******************************************************************************
  * @file    power.c
  * @author  ANCN@Leo
  * @version V1.0.1
  * @date    03/17/2014
  * @brief   source file for power.c module.
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
//#include "power.h"  

/*  P1DIR |= 0x01;                            // Set P1.0 to output direction

  while (1)                                 // Test P1.4
  {
    if ((0x10 & P1IN)) P1OUT |= 0x01;       // if P1.4 set, set P1.0
    else P1OUT &= ~0x01;                    // else reset
  }
  */
  /*初始化电源控制端*/
void InitPowerContrl()
{
//初始化电源控制引脚
  P2DIR |= 0x20;  
//打开模块电源
  PowerSet(PowerOn);
//延时等待1秒保证系统稳定 
  LongDelay(1); 
}

void PowerSet( rt_uint8_t pSet)
{
if(pSet == PowerOn)
{
  XbeeMode_ON;
  XbeePowerStatus = 1;
}
else if(pSet == PowerOff)
{
  XbeeMode_OFF;
  XbeePowerStatus = 0;
}
}

void InitSleepContrl()
{
  //初始化控制引脚
  P3DIR |= 0x20; //P3.5-->output-->SLEEP_REQ
  P3DIR &= ~0x10;//P3.4-->input-->SLEEP_ON/OFF
  SetExitSleep;//low-->active
  //获取当前的状态
  if(SleepGet())
  {
    SleepRunStatus = Active;
  }
  else
  {
    SleepRunStatus = Sleep;
  }
  //设定控制参数
  
}

rt_uint8_t SleepSet(rt_uint8_t pSet)
{
if(pSet == ReqInSleep)//申请进入休眠模式
{
  SetInSleep;
  DelayTime(10000);//40ms--check
  if(SleepGet())//获取休眠状态
  {
    SleepRunStatus = Active;
    return 0;
  }
  else
  {
    SleepRunStatus = Sleep;
    return 1;
  }
  
}
else if(pSet == ReqExitSleep)
{
  SetExitSleep;
  DelayTime(10000);//40ms--check
  if(SleepGet())//获取休眠状态
  {
    SleepRunStatus = Active;
    return 0;
  }
  else
  {
    SleepRunStatus = Sleep;
    return 1;
  }  
}
return 0xFF;
}

rt_uint8_t SleepGet()
{
rt_uint8_t GetStatus = 0;  
 GetStatus = P3IN&0x10;         //P3.4-->input-->status 
return GetStatus;
}
