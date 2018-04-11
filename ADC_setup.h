#ifndef __ADC_setup
  #define __ADC_setup

void adc_int(void);
void adcsetup(void);
void start_timerA3(void);
void stop_timerA3(void);


//extern int handle_adc; //extern will reference the int handle_adc that is written in ADC_setup file
//CTL_EVENT_SET_t handle_adc//use an event to trigger the adc, needs to be used with the advanced ARCbus code 
#endif
