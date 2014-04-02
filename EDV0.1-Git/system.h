/* EndDevice-V1.0.0  ���Ͱ�
2014-3 �汾�������
2014-3-27 ȫ�°汾������
˵�� :���ڸ���Ӳ��ƽ̨������ȫ������ģ�黯�ͱ�׼����������

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
#include "flash.h"

#define rTY		0	//�豸����
#define rNS		1	//�ź�����
#define rPW		2	//��Դ��ѹ
#define rSN		3	//����ʱ��
#define rST		4	//����ʱ��
#define rPs		5	//��ʼ��ַ
#define rPL		6	//���ʳ���
#define rPG		7	//ϵͳҳ
#define rDB0		8	//����0
#define rDB1		9	//����1
#define rDB2		10	//����2
#define rDB3		11	//����3
#define rDB4		12	//����4
#define rDB5		13	//����5
#define rDB6		14	//����6
#define rDB7		15	//����7

#define PG0		1	//ϵͳҳ
#define PG1		2	//ϵͳҳ
#define PG2		3	//ϵͳҳ
#define PG3		4	//ϵͳҳ
#define PG4		5	//ϵͳҳ
#define PG5		6	//ϵͳҳ
#define PG6		7	//ϵͳҳ
#define PG7		8	//ϵͳҳ


//P2OUT ^= 0x08;
#define LED1_ON        P2OUT |= 0x08
#define LED1_OFF       P2OUT &= ~0x08
#define LED2_ON        P2OUT |= 0x10
#define LED2_OFF       P2OUT &= ~0x10

#define XBEE_ON        P2OUT |= 0x20
#define XBEE_OFF       P2OUT &= ~0x20

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
	rt_uint8_t     	eIP;        	// �豸IP��ַ
  	rt_uint8_t    	cIP;        	// WRTU-IP��ַ 
    	rt_uint16_t     	xPAN;      	// PANID-����ID
  	rt_uint8_t      	xSC;        	//
  	rt_uint8_t      	xSD;        	// 
   	rt_uint8_t      	xCE;        	// 
   	rt_uint8_t      	xFlag;        	// 
   	
   	rt_uint8_t      	xSP;        	//
   	rt_uint8_t      	xSN;        	//
   	rt_uint8_t      	xSM;        	//
   	rt_uint16_t      xST;        	//
    	rt_uint8_t      	xSO;        	//  	   	   	   	
   	rt_uint8_t      	xPL;        	// 
   	rt_uint8_t      	xPM;        	//   
   	rt_uint8_t      	xRSSI;        	//

	
  	rt_uint32_t     	eMACH;      // �豸MAC��ַ
  	rt_uint32_t     	eMACL;      // �豸MAC��ַ
  	rt_uint32_t     	cMACH;      // WRTU-MAC��ַ
  	rt_uint32_t     	cMACL;      // WRTU-MAC��ַ  
  	rt_uint32_t     	dMACH;      // Ŀ��MAC��ַ
  	rt_uint32_t     	dMACL;      // Ŀ��MAC��ַ
  
  	unsigned char 	*pReg;//�Ĵ���ָ��
  	unsigned char 	*pRxd;//���ջ�����ָ��
  	unsigned char 	*pTxd;//���ͻ�����ָ��

};

 struct Net_Farme
{	
rt_uint8_t     	DA;     //Ŀ���ַ
rt_uint8_t      	FC;     //������
rt_uint16_t      SD;     //��ʼ��ַ
rt_uint16_t      LN;     //���ݳ���
rt_uint8_t		 DATA[Data_Length];
rt_uint16_t     CRC;
}; 

//struct Net_Farme DevMsg;


//#include "xbeeapp.h"
/*#define SET_BIT(x,y) ((x)|=(0x0001<<(y)))  //��x�ĵڣ�λΪ1
#define CLR_BIT(x,y) ((x)&=~(0x0001<<(y)))  //��x�ĵڣ�λΪ0
#define CPL_BIT(x,y) ((x)^=(0x0001<<(y)))  //ȡ��x�ĵڣ�λ
#define GET_BIT(x,y) ((x)&(0x0001<<(y)))   //��ȡx�ĵڣ�λ*/

extern struct Net_Farme MsgTxd;
extern struct Net_Farme MsgRxd;
 
extern unsigned char RxdData[32];
// extern ED_Device_t ED;
extern unsigned char InitFlash(unsigned char Mode);
extern unsigned int WRTU_LinkCnt; 
extern unsigned char ED_RunSta;
extern unsigned int WRTU_LinkCnt; 
extern unsigned char FlashChk;/* ���Flash�����Ƿ���ȷ */
extern unsigned char TxFlag;
extern unsigned char RxFlag;
extern unsigned int XbeeBusy;
extern unsigned char RxdOk;/* ��������OK��� */
extern unsigned char RxdCnt;/* �������ݵļ��� ÿһ�ν��ջ���� */
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
extern unsigned char RxdState;/* ��������OK��� */

extern struct ED_Device ED;
extern unsigned char HoldReg[64];
//extern unsigned char EdReg[16];
//extern unsigned char PgReg[8][8];

extern rt_uint8_t XbeePowerStatus;         //��Դ״̬��1=Active 0=Sleep   
extern rt_uint8_t SleepRunStatus;          //��ǰ״̬��1=Active 0=Sleep 
extern rt_uint8_t SleepReqStatus;          //����״̬��1=Active 0=Sleep   
extern rt_uint8_t SleepTimeSet;            //���ߵ�ʱ�����


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

void AppProcess(void);