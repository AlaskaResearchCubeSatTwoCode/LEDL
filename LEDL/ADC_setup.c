#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include "ADC_setup.h"
#include <ctl.h>
#include "subsystem.h"

int adctemp;
int adctemparray1[6];
int readvalue;
extern int adctemp;
//CTL_EVENT_SET_t handle_adc;
int adccount = 0;
int check_adc_read;
//int handle_adc;


void adcsetup(void) {
//setting up A0-A5 for ADC
  //ctl_events_init(&handle_adc, 0);
  P1SEL0 = 0X3F; // DISABLES THE DITIGAL FUNCTION OF PORT 0. MAKES THEM ALL ANALOG CAPABLE
  P1SEL1 = 0X3F; // DISABLES THE DITIGAL FUNCTION OF PORT 1. sets up for ADC10_A function
  //BITS SET TO DEFINE ADC12 FOR SINGLE CHANNEL MODE
  ADC10CTL0 |= ADC10SHT_3 | ADC10ON  | ADC10MSC; // SAMPELING FOR 32 CLOCK CYCLES | ADC12 IS ON | CONVERT AUTOMATICALLY MULTISAMPMODE
// ADC10CTL1 |= ADC10SHS_0 | ADC10SHP | ADC10DIV_0 | ADC10SSEL_0 | ADC10CONSEQ_1; // triggers off of SHS | SAMPCON signal is sorced from the sampeling timer
// divide by 1 | clock select ACLK | sequence of channels
  ADC10CTL1 |= ADC10SHS_0 | ADC10SHP | ADC10DIV_1 | ADC10SSEL_3 | ADC10CONSEQ_1; // triggers off of SHS | SAMPCON signal is sorced from the sampeling timer
  // divide by 1 | clock select SMCLK | sequence of channels
  ADC10CTL2 |= ADC10PDIV0 | ADC10RES | ADC10SR; // PREDEVIDE BY 1 | 10 BIT RES | LIMITS SAMPELING TO 50 KSPS (REDUCE CURRENT)
  //enable interrupt OF DATA READY
  ADC10IE |= ADC10IE0;
  // SET ALL THE CORESPONDING MEMEORY REGISTERS WITH CORESPONDING INPUT CHANNEL
  ADC10MCTL0 = ADC10SREF_0 | ADC10INCH_5; //USED SOURCE VOLTAGE TO COMPARE RAILS | INPUT CHANNEL start IS A5

  ADC10IFG = 0; // SET ALL FLAGS TO 0

}



//added the interrupt if statements for each memory location
void adc_int(void) __interrupt[ADC10_VECTOR] { //this is an interrupt function the _interrupt tells its a function interrupt
  switch (ADC10IV) {

  case ADC10IV_ADC10IFG://: conversion ready Interrupt
    // adctemp=ADC10MEM0;
    check_adc_read = ADC10MCTL0;
    adctemparray1[adccount] = ADC10MEM0 | (check_adc_read << 13);
    adccount++;
    if (adccount == 5) {
       //ctl_events_set_clear(&SYS_evt,SYS_EV_1,0);
      //ctl_events_set_clear(&handle_adc, 1 << 0, 0);
      adccount = 0;
      P7OUT ^= BIT6;
    }
    break ;

  case ADC10IV_ADC10INIFG: //window comparator Interrupt flags
    break ;

  case ADC10IV_ADC10LOIFG: ///window comparator Interrupt flags
    break ;

  case ADC10IV_ADC10HIIFG: //window comparator Interrupt flags
    break ;

  case ADC10IV_ADC10TOVIFG: //ADC10MEM0 overflow
    break ;

  case ADC10IV_NONE:
    break ;

  }
}



void start_ADC_sampeling(void) {
  //ENABLE CONVERSION BIT FOR ADC, FOR THIS TO WORK WE NEED RISING CLOCK EDGE TO START THE SAMPLING
  ADC10CTL0 |= ADC10ENC | ADC10SC; // SET ENABLE BIT TO START CONVERSION |START OF CONVERSION
}

void stop_ADC_sampeling(void) {
//ENABLE CONVERSION BIT FOR ADC, FOR THIS TO WORK WE NEED RISING CLOCK EDGE TO START THE SAMPLING
  ADC10CTL0 &= ~(ADC10ENC | ADC10SC); // SET ENABLE BIT TO START CONVERSION |START OF CONVERSION
}

void init_timerA3(void) {
  //setup timer A
  TA3CTL = TASSEL_1 | ID_0 | TACLR; //TASSEL_2 SMClK TASSEL_1 ACLK
  //init CCR0 for tick interrupt
  //TACCR0=32;
// TACCTL0=CCIE;
  TA3CCR0 = 8;
  TA3CCTL0 = OUTMOD_4 | CCIE;
}

//start timer A in continuous mode
void start_timerA3(void) {
//start timer A
  TA3CTL |= MC_2;

}

void stop_timerA3(void) {
//turn off timer
  TA3CTL &= ~MC_3;
  adccount = 0;
}

void task_tick_forADC(void) __ctl_interrupt[TIMER3_A0_VECTOR] {
  //set rate to 8000Hz to change this later on, change the TACCR0+=4 for final value to take accelerometer data at 4kHz
  TA3CCR0 += 8;
  //increment timer
  //start adc
  start_ADC_sampeling();
  P7OUT ^= BIT7;

}
