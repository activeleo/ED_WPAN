/**
  ******************************************************************************
  * @file    adc.c
  * @author  ANCN@Leo
  * @version V1.0.1
  * @date    03/17/2014
  * @brief   source file for adc.c module.
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
//******************************************************************************
//  MSP430G2x33/G2x53 Demo - ADC10, DTC Sample A2-0, AVcc, Single Sequence, DCO
//
//  Description: Sample A3/A2/A1 as single sequence with reference to AVcc.
//  Software sets ADC10SC to trigger sample sequence. In Mainloop MSP430 waits
//  in LPM0 to save power until ADC10 conversion complete, ADC10_ISR(DTC) will
//  force exit from any LPMx in Mainloop on reti. ADC10_ISR will force any LPMx
//  exit. ADC10 internal oscillator times sample period (16x) and conversion
//  (13x). DTC transfers conversion code to RAM 200h - 206h. P1.0 set at start
//  of conversion burst, reset on completion.
//
//               MSP430G2x33/G2x53
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//       >---|P1.3/A3      P1.0|-->LED
//       >---|P1.2/A2          |
//       >---|P1.1/A1          |
//
//  D. Dang
//  Texas Instruments Inc.
//  December 2010
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
#include "system.h"
 
long temp;
extern int long IntDegF;
extern int long IntDegC; 

void InitAdc10()
{
 // ADC10CTL1 = INCH_3 + CONSEQ_1;            // A3/A2/A1, single sequence
 // ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
 // ADC10DTC1 = 0x03;                         // 3 conversions
 // ADC10AE0 |= 0x0E;                         // P1.3,2,1 ADC10 option select
 
  ADC10CTL0  = SREF_1 + ADC10SHT_1 + ADC10SR + REF2_5V  ;//Vref-Vss+8Clk+50k+Ref2.5V  
  //ADC10CTL1 = INCH_0 + INCH_3 + INCH_10 + ADC10SSEL_1 + CONSEQ_0;//Ch0/3/temp+Aclk+Singel 
  ADC10CTL1 = ADC10SSEL_2 + CONSEQ_0;
  ADC10CTL0 |= ADC10ON + ADC10IE;
}

void Adc10RefSet( rt_int8_t pSet,rt_int8_t pRef)
{
  if(pSet)
  {
  ADC10CTL0 |= REFON;
  }
  else
  {
  ADC10CTL0 &= ~REFON;
  }
}

void Adc10Get(rt_int8_t  pCH )
{
    ADC10CTL0 &=~ENC;
    ADC10CTL1 |= pCH<<12;
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
   // DelayTime(5000);      
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
   // ADC10SA = 0x200;                        // Data buffer start
  temp = ADC10MEM;
  Adc10Temo();
}

void Adc10Temo()
{
    // oF = ((A10/1024)*1500mV)-923mV)*1/1.97mV = A10*761/1024 - 468
    //temp = ADC10MEM;
    //IntDegF = ((temp - 630) * 761) / 1024;

    // oC = ((A10/1024)*1500mV)-986mV)*1/3.55mV = A10*423/1024 - 278
     
    //IntDegC =temp*423/1024 - 278;// ((temp - 673) * 423) / 1024;
//IntDegC = (temp-746)/(0.000335*678)+286;
IntDegC = ((temp*2.5)/1023-0.986)/0.00355;
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  //__bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
   temp = ADC10MEM;
}