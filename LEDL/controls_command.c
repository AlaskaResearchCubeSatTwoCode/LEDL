//This file will cover all stack up events
#include <msp430.h>
#include <ctl.h>
#include <ARCbus.h>
#include <Error.h>
#include <SDlib.h>
#include "pins.h"



void ledl_pin_setup(void){
//SET ALL POWER PMOS IN OFF POSITION 
SENSORS_DIR|=0xFF;//set all ledl vcc control pins to outputs
SENSORS_OUT|=0xFF;//MAKE ALL PMOSF OFF , OUT IS PULLED UP. 
SENSORS_DS|= 0xFF;//make the sensor drive strength at 3.3 V   

//SET ALL LEDs off
//define LED pins 
LED_PORT_DIR|=0XFF;
LED_PORT_DS|=0XFF;
LED_PORT_OUT&=~0XFF;
LED_CONTROL_X_DIR|=LED_CONTROL_X;  
LED_CONTROL_X_OUT&=~LED_CONTROL_X;
LED_CONTROL_X_DS|=LED_CONTROL_X; 
LED_CONTROL_Y_DIR|=LED_CONTROL_Y;
LED_CONTROL_Y_OUT&=~LED_CONTROL_Y;
LED_CONTROL_Y_DS|=LED_CONTROL_X; 

//TURN OFF LUMEN SENSORS 
LUX14_DIR|=LUX1_VCC_CONT4|LUX4_VCC_CONT7;
LUX_2_DIR|=LUX2_VCC_CONT5;
LUX35_DIR|=LUX3_VCC_CONT6|LUX5_VCC_CONT8; 

LUX14_DS|=LUX1_VCC_CONT4|LUX4_VCC_CONT7;
LUX_2_DS|=LUX2_VCC_CONT5;
LUX35_DS|=LUX3_VCC_CONT6|LUX5_VCC_CONT8; 

LUX14_OUT&=~(LUX1_VCC_CONT4|LUX4_VCC_CONT7);
LUX_2_OUT&=~LUX2_VCC_CONT5;  
LUX35_OUT&=~(LUX3_VCC_CONT6|LUX5_VCC_CONT8);   
     
//Turn off Regulator and turn off NMOS for blowing fuse power pins                
LEDL_REGULATOR_DIR|=LEDL_REGULATOR_ON_OFF|LEDL_BLOW_FUSE;
LEDL_REGULATOR_OUT&=~(LEDL_REGULATOR_ON_OFF|LEDL_BLOW_FUSE);                                    
LEDL_REGULATOR_DS|=LEDL_REGULATOR_ON_OFF|LEDL_BLOW_FUSE;              
}


void LED_ON_OFF(char LED_NAME, char status){
  if (status==ON){ 
  LED_PORT_OUT|=LED_NAME; 
  } 
  else if(status==OFF){
  LED_PORT_OUT&=~LED_NAME;
  }
}

void VCC_ON_OFF(char VCC_NAME, char status){
  if (status==ON){ 
  SENSORS_OUT&=~VCC_NAME;
  } 
  else if(status==OFF){
  SENSORS_OUT|=VCC_NAME;
  }
}


 
 void turn_on_all_sensors(void){
 P6DIR|=0xFF; 
 P6DS|=0XFF; 
 P6OUT &=0XC5;
 //P6OUT&= 0XDF;
  
 
 } 
