#ifndef __netapp_h
#define __netapp_h

//#include "system.h"
#include "type.h"


extern uint8_t	DB;///< 
extern uint8_t	PL;///< 

extern unsigned char HexToChar(unsigned char bChar);
extern unsigned char CharToHex(unsigned char bHex);
extern void ClearRxd(void);

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

extern void UartWriteStr(char *s);
extern void UartWriteChar(char c);
extern void UartWriteStrNum(char *s,char num);
extern void DelayTime(unsigned int delay);
extern unsigned char At_WR(void);
extern unsigned char  At_Da_Set(uint32_t DaH,uint32_t DaL,unsigned char wr );
extern void erase_flash(unsigned int Addr);
extern unsigned char write_flash( char *value,unsigned int Add,char Len);
extern unsigned char read_flash(char *value,unsigned int Add,char Len);


unsigned char  Net_Init(void);
void Chk_Link(void);
void Chk_Act(void);




void MsgProcess(void);
rt_uint8_t MsgRecive(rt_uint8_t ED_ID);
rt_uint8_t MsgCopyToRxd(rt_uint8_t pLen );
rt_uint8_t MsgAction(rt_uint8_t pFC);
rt_uint8_t ReadReg(rt_uint8_t pSD,rt_uint8_t pLN,rt_uint8_t *pBuf);
rt_uint8_t WriteReg(rt_uint8_t pSD,rt_uint8_t pLN,rt_uint8_t *pBuf);
rt_uint8_t MsgSendReg(rt_uint8_t pTY,rt_uint8_t pSD,rt_uint8_t pSL);
void TestReg(void);
rt_uint8_t MsgResponse(rt_uint8_t pFC,rt_uint8_t pLN);
rt_uint8_t UpLoadMsg(rt_uint8_t pFC,rt_uint8_t pSD,rt_uint8_t pLN);
void SyncToFlash(rt_uint8_t pOptr, rt_uint8_t pPAGE );



#endif /* __netapp_h */

/* end of netapp.h */





