#ifndef __power_h
#define __power_h



#define SetInSleep              P3OUT |= 0x20
#define SetExitSleep            P3OUT &= ~0x20

#define XbeeMode_ON             P2OUT |= 0x20
#define XbeeMode_OFF            P2OUT &= ~0x20

    


void InitPowerContrl(void);
void InitSleepContrl(void);
void PowerSet( rt_uint8_t pSet);
rt_uint8_t SleepGet(void);
rt_uint8_t SleepSet(rt_uint8_t pSet);



#endif /* __power_h */

/* end of power.h */

