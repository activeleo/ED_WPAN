#ifndef __timer_h
#define __timer_h
#include "type.h"

void TimeraInit( void );
void TimerSrv(unsigned int Delay);
void TimerSrvBusy( void );
void TimerSrvRun( void );
void LongDelay(unsigned int CNT);
void timera_start(unsigned int TimCh,unsigned int Delay);
void timera_stop(unsigned int TimCh);
void LongDelay(unsigned int CNT);
void DelayTime(unsigned int delay);

//void Timer_A0 (void);

#endif /* __timer_h */

/* end of timer.h */





