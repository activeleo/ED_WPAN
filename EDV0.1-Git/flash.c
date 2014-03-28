#include "system.h"
extern struct ED_Device ED;
/*********************************************************************************************************
** Function name:       
** Descriptions:   
** input parameters:    
** output parameters:  
** Returned value:      
*********************************************************************************************************/
unsigned char InitFlash(unsigned char Mode)
{
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
char Flash[8]={0x01,0x02,0x03,0x04,0x11,0x12,0x13,0x14};
unsigned char read_flash(char *value,unsigned int Add,char Len);
char ReadFlash[8];
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





