#include "system.h"
extern struct ED_Device ED;

char Flash[8]={0x01,0x02,0x03,0x04,0x11,0x12,0x13,0x14};
unsigned char read_flash(char *value,unsigned int Add,char Len);
char ReadFlash[8];


/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char InitFlash(unsigned char Mode)
{
read_flash(ReadFlash,0x1000,8);
ED.eIP = ReadFlash[0];
ED.cIP = ReadFlash[1];
ED.xPAN = (ReadFlash[2]<<8)|ReadFlash[3];
ED.xSC = ReadFlash[4];
ED.xSD = ReadFlash[5];
ED.xCE = ReadFlash[6];
ED.xFlag = ReadFlash[7];

read_flash(ReadFlash,0x1008,8);
ED.xSP = ReadFlash[0];
ED.xSN = ReadFlash[1];
ED.xSM = ReadFlash[2];
ED.xST = ReadFlash[3];
ED.xSO = ReadFlash[4];
ED.xPL = ReadFlash[5];
ED.xPM = ReadFlash[6];
ED.xRSSI = ReadFlash[7];

read_flash(ReadFlash,0x1010,8);
ED.eMACH = ReadFlash[0];
ED.eMACH = (ED.eMACH<<8) | ReadFlash[1];
ED.eMACH = (ED.eMACH<<8) | ReadFlash[2];
ED.eMACH = (ED.eMACH<<8) | ReadFlash[3];
ED.eMACL = ReadFlash[4];
ED.eMACL = (ED.eMACL<<8) | ReadFlash[5];
ED.eMACL = (ED.eMACL<<8) | ReadFlash[6];
ED.eMACL = (ED.eMACL<<8) | ReadFlash[7];

read_flash(ReadFlash,0x1018,8);
ED.cMACH = ReadFlash[0];
ED.cMACH = (ED.cMACH<<8) | ReadFlash[1];
ED.cMACH = (ED.cMACH<<8) | ReadFlash[2];
ED.cMACH = (ED.cMACH<<8) | ReadFlash[3];
ED.cMACL = ReadFlash[4];
ED.cMACL = (ED.cMACL<<8) | ReadFlash[5];
ED.cMACL = (ED.cMACL<<8) | ReadFlash[6];
ED.cMACL = (ED.cMACL<<8) | ReadFlash[7];

    /*if((ED.ED_IP!=0xFFFF)&&(ED.RT_IP!=0xFFFF)
	&&(ED.ED_MACH!=0xFFFFFFFF)&&(ED.ED_MACL!=0xFFFFFFFF)
	&&(ED.RT_MACH!=0xFFFFFFFF)&&(ED.RT_MACL!=0xFFFFFFFF))
    {
          return 1;
    }*/
  return 0;

}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char ResetFlash(unsigned char Mode)
{
if(Mode == 0xFF )
{
DelayTime(500);
erase_flash(0x1000);
DelayTime(500);
}

ED.eIP = 0xFF;
ED.cIP = 0xFF;
ED.xFlag = 0xFF;
ED.xRSSI = 0xFF;

ReadFlash[0] = 0xFF;
ReadFlash[1] = 0xFF;          
ReadFlash[2] = ED.xPAN>>8;        
ReadFlash[3] = ED.xPAN;  
ReadFlash[4] = ED.xSC;
ReadFlash[5] = ED.xSD;   
ReadFlash[6] = ED.xCE;
ReadFlash[7] = ED.xFlag;     
write_flash(ReadFlash,0x1000,8);

ReadFlash[0] = ED.xSP;
ReadFlash[1] = ED.xSN;          
ReadFlash[2] = ED.xSM;        
ReadFlash[3] = ED.xST;  
ReadFlash[4] = ED.xSO;
ReadFlash[5] = ED.xPL;   
ReadFlash[6] = ED.xPM;
ReadFlash[7] = ED.xRSSI; 
write_flash(ReadFlash,0x1008,8);

ReadFlash[0] = ED.eMACH>>24;
ReadFlash[1] = ED.eMACH>>16;          
ReadFlash[2] = ED.eMACH>>8;        
ReadFlash[3] = ED.eMACH;  
ReadFlash[4] = ED.eMACL>>24;
ReadFlash[5] = ED.eMACL>>16;   
ReadFlash[6] = ED.eMACL>>8;
ReadFlash[7] = ED.eMACL; 
write_flash(ReadFlash,0x1010,8);
ReadFlash[0] = ED.cMACH>>24;
ReadFlash[1] = ED.cMACH>>16;          
ReadFlash[2] = ED.cMACH>>8;        
ReadFlash[3] = ED.cMACH;  
ReadFlash[4] = ED.cMACL>>24;
ReadFlash[5] = ED.cMACL>>16;   
ReadFlash[6] = ED.cMACL>>8;
ReadFlash[7] = ED.cMACL; 
write_flash(ReadFlash,0x1018,8);


return 1;
}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/

void TestFlash()
{
 FCTL2 = FWKEY + FSSEL_2 + FN1;          // д╛хо SMCLK/3 =333KHz 
read_flash(ReadFlash,0x1000,8);
_NOP();
write_flash(Flash,0x1000,8);
_NOP();
read_flash(ReadFlash,0x1000,8);
_NOP();

}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void WaitForEnable()
{
    while((FCTL3 & BUSY) == BUSY);      //Busy
}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
void erase_flash(unsigned int Addr)  
{ 
    char *FlashPtr;
    FlashPtr = (char *)Addr;
    FCTL1 = FWKEY + ERASE;                      // Set Erase bit
    FCTL3 = FWKEY;                              // Clear Lock bit
    //DINT;
    *FlashPtr = 0;                              // Dummy write to erase Flash segment B
    WaitForEnable();                            //Busy
    //EINT;
    FCTL1 = FWKEY;                              // Lock
    FCTL3 = FWKEY + LOCK;                       // Set Lock bit  
}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char write_flash( char *value,unsigned int Add,char Len)
{
  char *Flash_ptr;                          // Flash pointer
  unsigned int i,j;

  Flash_ptr = (char *) Add;              // Initialize Flash pointer
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  //*Flash_ptr = 0;                           // Dummy write to erase Flash segment

  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i=0; i<Len; i++)
  {
    *Flash_ptr++ = *value;                   // Write value to flash
    *value++;
    for(j=0;j<100;j++)_NOP();
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
  return 1;
}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char read_flash(char *value,unsigned int Add,char Len)
{
  char *Flash_ptr;                          // Flash pointer
  unsigned int i,j;

  Flash_ptr = (char *) Add;              // Initialize Flash pointer

  for (i=0; i<Len; i++)
  {
     *value = *Flash_ptr++;                   // Write value to flash
    *value++;
    for(j=0;j<100;j++)_NOP();
  }

  return 1;
}
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char CheckFlash(  )
{
read_flash(ReadFlash,0x1007,1);
if(ReadFlash[0]!=0xFF)
{
	return ReadFlash[0];
}
  return 0;
}

/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
rt_uint8_t WriteConfigToFlash(rt_uint8_t pOptr,rt_uint8_t pSD,rt_uint8_t pLN,unsigned char *pBuf)
{
//rt_uint8_t pBuf[8] = {0,0,0,0,0,0,0,0};
rt_uint8_t i = 0,j = 0,k = 0;;

if(((pSD+pLN)<=0xFF)&&(pLN<=8))
{
read_flash(ReadFlash,0x1000+pSD,pLN);
for(i=0;i<pLN;i++)
{
	if(ReadFlash[i]==0xFF)
		j=j+1;
}

}

 if(pOptr==0xF0)
{
        DelayTime(500);
        erase_flash(0x1000);
        DelayTime(500);
	return 0;
}
else
{
	for(k=0;k<pLN;k++)
	{
	ReadFlash[k] = *pBuf;
	*pBuf++;
	}
write_flash(ReadFlash,0x1000+pSD,pLN);
return 1;

}

}



  //char ReadFlash[8];
    	/*read_flash(ReadFlash,0x1000,8);
	ED.ED_IP = (ReadFlash[0]<<8)|ReadFlash[1];
	ED.RT_IP = (ReadFlash[2]<<8)|ReadFlash[3];
	ED.PAN_ID = ReadFlash[4];
    	ED.ED_CE = ReadFlash[5]; 
	ED.ED_SD = ReadFlash[6]; 
	ED.ED_TY = ReadFlash[7]; */
    
    	/*read_flash(ReadFlash,0x1010,8);
    	ED.ED_MACH = ReadFlash[0];
    	ED.ED_MACH = (ED.ED_MACH<<8) | ReadFlash[1];
    	ED.ED_MACH = (ED.ED_MACH<<8) | ReadFlash[2];
    	ED.ED_MACH = (ED.ED_MACH<<8) | ReadFlash[3];
    	ED.ED_MACL = ReadFlash[4];
    	ED.ED_MACL = (ED.ED_MACL<<8) | ReadFlash[5];
    	ED.ED_MACL = (ED.ED_MACL<<8) | ReadFlash[6];
    	ED.ED_MACL = (ED.ED_MACL<<8) | ReadFlash[7];
    
    	read_flash(ReadFlash,0x1018,8);*/
    	/*ED.RT_MACH= ReadFlash[0];
    	ED.RT_MACH = (ED.RT_MACH<<8) | ReadFlash[1];
    	ED.RT_MACH = (ED.RT_MACH<<8) | ReadFlash[2];
    	ED.RT_MACH = (ED.RT_MACH<<8) | ReadFlash[3];
    	ED.RT_MACL= ReadFlash[4];
    	ED.RT_MACL = (ED.RT_MACL<<8) | ReadFlash[5];
    	ED.RT_MACL = (ED.RT_MACL<<8) | ReadFlash[6];
    	ED.RT_MACL = (ED.RT_MACL<<8) | ReadFlash[7];

        DelayTime(500);
       if((ED.RT_MACH!=0x0013A200)&&(ED.RT_MACL!=0x4076E9CB))
       {
       return 1;
       }
       else
       {
        erase_flash(0x1000);
        DelayTime(500);*/
        /*ReadFlash[0] = ED.ED_IP>>8;        
	ReadFlash[1] = ED.ED_IP;         
	ReadFlash[2] = ED.RT_IP>>8;        
	ReadFlash[3] = ED.RT_IP; 
	ReadFlash[4] = ED.PAN_ID; 
	ReadFlash[5] = ED.ED_CE; 
	ReadFlash[6] = ED.ED_SD; 
	ReadFlash[7] = ED.ED_TY; 
	write_flash(ReadFlash,0x1000,8);*/
        //ED.RT_MACH = 0x0013A200;
        //ED.RT_MACL = 0x4076E9CB;
        /*ReadFlash[0] = ED.ED_MACH>>24;
	ReadFlash[1] = ED.ED_MACH>>16;          
	ReadFlash[2] = ED.ED_MACH>>8;        
	ReadFlash[3] = ED.ED_MACH;  
	ReadFlash[4] = ED.ED_MACL>>24;
	ReadFlash[5] = ED.ED_MACL>>16;   
	ReadFlash[6] = ED.ED_MACL>>8;
	ReadFlash[7] = ED.ED_MACL;     
	write_flash(ReadFlash,0x1010,8);*/
        /*ReadFlash[0] = ED.RT_MACH>>24;
        ReadFlash[1] = ED.RT_MACH>>16;          
	ReadFlash[2] = ED.RT_MACH>>8;        
	ReadFlash[3] = ED.RT_MACH;  
	ReadFlash[4] = ED.RT_MACL>>24;
	ReadFlash[5] = ED.RT_MACL>>16;   
	ReadFlash[6] = ED.RT_MACL>>8;
	ReadFlash[7] = ED.RT_MACL;     
	write_flash(ReadFlash,0x1018,8);
       DelayTime(500); 
       } 
        */



