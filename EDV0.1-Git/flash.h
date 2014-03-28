#ifndef __flash_h
#define __flash_h
#include "type.h"

unsigned char InitFlash(unsigned char Mode);
void TestFlash(void);
void WaitForEnable(void);
void erase_flash(unsigned int Addr);
unsigned char write_flash( char *value,unsigned int Add,char Len);
unsigned char read_flash(char *value,unsigned int Add,char Len);

#endif /* __flash_h */

/* end of flash.h */
