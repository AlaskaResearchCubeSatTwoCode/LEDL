/**********************************************************************************************************************************************
The commands.c file is for commands that will be displayed through the serial terminal. 
In order to add a command you must create a function as seen below.
Then function must be added to the "const CMD_SPEC cmd_tbl[]={{"help"," [command]",helpCmd}" table at the end of the file.
**********************************************************************************************************************************************/
#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <terminal.h>
#include <commandLib.h>
#include <stdlib.h>
#include <ARCbus.h>
#include <SDlib.h>
#include <i2c.h>
#include <UCA2_uart.h>        // UART setup
#include "pins.h"
#include "LIS3DH_Accelerometer.h"
#include "L3G4200_GYRO.h"
#include "temp.h"

//Declaring globals
  unsigned int port=4;
  unsigned int sda=1;
  unsigned int scl=0;
  unsigned short ret, stat, pwrup; //return status, pwrup variables for sending commands and error checking
  signed char acc[4]={0,0,0,0};
//*********************************************************** passing arguments over the terminal *********************************************
int example_command(char **argv,unsigned short argc){
  int i,j;
  //TODO replace printf with puts ? 
  printf("This is an example command that shows how arguments are passed to commands.\r\n""The values in the argv array are as follows : \r\n");
  for(i=0;i<=argc;i++){
    printf("argv[%i] = 0x%p\r\n\t""string = \"%s\"\r\n",i,argv[i],argv[i]);
    //print out the string as an array of hex values
    j=0;
    printf("\t""hex = {");
    do{
      //check if this is the first element
      if(j!='\0'){
        //print a space and a comma
        printf(", ");
      }
      //print out value
      printf("0x%02hhX",argv[i][j]);
    }while(argv[i][j++]!='\0');
    //print a closing bracket and couple of newlines
    printf("}\r\n\r\n");
  }
  return 0;
}


/*********************************************************** Using the Timer_A1 ***************************************************************
* DONT USE TIMER0_Ax_VECTOR !!! this interrupt is use in library code and will cause a collision 
* Use TIMERx_Ay_VECTOR x=2,3 & y=0,1
* TIMER0_Ax_VECTOR used in ARClib ? 
* TIMER1_Ax_VECTOR used in ????
**********************************************************************************************************************************************/
int example_timer_IR(char **argv,unsigned short argc){
  int timer_check;
  WDTCTL = WDTPW+WDTHOLD;                                   // Stop WDT
  P7DIR |= 0xFF;                                            // Setting port 7 to drive LED's (0xFF ==1111 1111)
  P7OUT = 0x00;                                             // Set all LED's on port 7 to start all off
//************************************ Set up clock [0] 
  TA2CTL |= TASSEL__ACLK | MC_2;                            // Setting Timer_A to ACLK(TASSEL_1) to continuous mode(MC_2)

//*********************************** Set timer interrupt enable [1] 
  TA2CCTL0 |= CCIE;                                          // Capture/compare interrupt enable #0
  TA2CCTL1 |= CCIE;                                          // Capture/compare interrupt enable #1

//*********************************** Set the timer count IR value [2] 
  TA2CCR0 = 10000;                                           // Timer0_A3 Capture/Compare @ 10000 counts
  TA2CCR1 = 1000;                                            // TA0IV_1 Capture/Compare @ 1000 counts

   while (1)                                                // poll in while loop until a key press
   {
      if ((timer_check=getchar()) != EOF)
     {
      break;                                                 // break out of loop if a key is pressed
     }
    ctl_events_wait(CTL_EVENT_WAIT_ANY_EVENTS,0, 1<<15, CTL_TIMEOUT_DELAY, 1000); // wait in loop 
   }
  reset(0,ERR_SRC_CMD,CMD_ERR_RESET,0);                     // reset clock registers 
  return 0;
}

// ********************************* Timer_A0 interrupt code ***********************************************************************************
//NOTE: to use interrupts you usually need need to enable global interrupts using _ENIR();
//      This is not necessary when using ARClib code as the enable is called in ARClib
//NOTE: For P1IV you still need to enable P1IE registers 
//**********************************************************************************************************************************************
void Timer_A2_A0(void)__interrupt[TIMER2_A0_VECTOR]{     // Timer A0 interrupt service routine TA0IV_TA0IFG. 
  P7OUT^=BIT0; // toggle LEDs when IR is called
}

void Timer_A2_A1(void)__interrupt[TIMER2_A1_VECTOR]{     // Timer A0 interrupt service routine for capture comp 1 and 2
        P7OUT^=BIT1; // light LEDs
}

//******************************************* Using the SD card *******************************************************************************

int SD_write(char **argv,unsigned short argc){
char buff[512];
int mmcReturnValue, result , i;  
  VCC_ON_OFF(LEDL_SDCARD_VCC_PIN,ON);
  mmcInit_msp();  // Sets up the interface for the card
  mmc_pins_on();  //Sets up MSP to talk to SD card
  mmcReturnValue=mmcInit_card();

  if (mmcReturnValue==MMC_SUCCESS){ // check good initialization 
    printf("\rCard initalized Sucessfully\r\n");
  }
  else{
    printf("Check SD card.\r\nInitalized failed.\r\n Error %i\r\n",mmcReturnValue);
  }

//populate buffer block
  for(i=1;i<=argc;i++) {// ignore 0 *argv input (argv[0]--> "SD_write" )
    strcat(buff,argv[i]); // appends chars from one string to another
    strcat(buff,"|");     // separates strings with | as strcat eats NULL
  }

//write to SD card
  result= mmcWriteBlock(100,buff); //(unsigned char*) casting my pointer(array) as a char 
 
  if (result>=0){ // check SD write 
  printf("SD card write success.\r\n");
  }
  else{
    printf("SD card write failed.\r\nError %i\r\n",result);
  }
  return 0;
}

int SD_read(char **argv,unsigned short argc){
  #define ASCIIOUT_STR  "%c "
 char buffer[512];
 int resp , i;
  
  //read from SD card
 resp=mmcReadBlock(100,buffer);
  //print response from SD card
  printf("%s\r\n",SD_error_str(resp));

        for(i=0;i<9;i++){//changed the 512 to 256 which is a result of changing CHAR TO INT

        if(i<8){
          printf(ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR "\r\n",
          buffer[i*28+3],buffer[i*28+4],buffer[i*28+5],buffer[i*28+6],buffer[i*28+7],buffer[i*28+8],buffer[i*28+9],buffer[i*28+10],buffer[i*28+11],buffer[i*28+12],buffer[i*28+13],
          buffer[i*28+14],buffer[i*28+15],buffer[i*28+16],buffer[i*28+17],buffer[i*28+18],buffer[i*28+19],buffer[i*28+20],buffer[i*28+21],buffer[i*28+22],buffer[i*28+23],
          buffer[i*28+24],buffer[i*28+25],buffer[i*28+26],buffer[i*28+27],buffer[i*28+28],buffer[i*28+29],buffer[i*28+30]);
          }

        else{
          printf(ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR ASCIIOUT_STR "\r\n",
          buffer[i*28+3],buffer[i*28+4],buffer[i*28+5],buffer[i*28+6],buffer[i*28+7],buffer[i*28+8],buffer[i*28+9],buffer[i*28+10],buffer[i*28+11],buffer[i*28+12],buffer[i*28+13],
          buffer[i*28+14],buffer[i*28+15],buffer[i*28+16],buffer[i*28+17],buffer[i*28+18],buffer[i*28+19],buffer[i*28+20],buffer[i*28+21],buffer[i*28+22],buffer[i*28+23],
          buffer[i*28+24],buffer[i*28+25],buffer[i*28+26],buffer[i*28+27],buffer[i*28+28],buffer[i*28+29],buffer[i*28+30]);
          }
        }
        return 0;
}

//******************************************************** Sending an I2C packet
int send_I2C(char **argv,unsigned short argc){
  unsigned short addr, cmd_id, resp, payload_count=argc-2, i;
  unsigned char packet[4+BUS_I2C_HDR_LEN+BUS_I2C_CRC_LEN], *payload;

  if (argc < 2) {
    printf("Not enough arguments.\n");
    return 0;
  }

  // Get integer value of address from passed string
  addr=strtoul(argv[1], NULL, 0);
  // Get integer value of I2C command from passed string
  cmd_id=strtoul(argv[2], NULL, 0);

  // Initialize packet with source I2C address, and I2C command
  payload=BUS_cmd_init(packet, cmd_id);

  for (i=payload_count; i > 0; --i) {
    payload[payload_count-i]=strtoul(argv[payload_count-i+2], NULL, 0); 
  }

  // Transmit packet to addr. Payload is zero length. 
  resp=BUS_cmd_tx(addr, packet, payload_count, 0);
  if (resp == 0) {
    printf("Valid response\n");
  }
  else {
    printf("Error: %d\n", resp);
  }
  /*initI2C(4,1,0);
  // I2C_tx expects a char* for transmitted data, not an integer
  resp=i2c_tx(addr, (unsigned char*)&cmd_id, 1);
  if (resp < 0){
    printf("I2C transaction error.\n");
  }*/

  return 0;
}

///////////////////////////Sensor Data Commands
int getX_Rotation(char **argv,unsigned short argc){ 
  signed int xmeas;
  unsigned char statreg[1]={L3G4200_STATUS_REG};
  unsigned char pwr[2]={L3G4200_CTRL_REG1,0x1F};
  unsigned char dat[1];
  unsigned short sendSize=1;
  unsigned short receiveSize=1;
  unsigned char addr[2]={L3G4200_OUT_X_LOW, L3G4200_OUT_X_HIGH };
     initI2C(port,sda,scl);
     pwrup=i2c_tx(L3G4200_SAD,pwr,2);
     stat=i2c_tx(L3G4200_SAD,statreg,1); 
     i2c_rx(L3G4200_SAD,dat,1);
     ret=i2c_tx(L3G4200_SAD,addr+1,sendSize);
      if(ret>0){//if tx is able to send 1 byte( the address of the slave) 
           ret=i2c_rx(L3G4200_SAD,dat,receiveSize);
           xmeas=dat[0];
           xmeas=xmeas<<8;
      }else{
           printf("ret failed, ret returned %i \r\n\t",ret);
           return 0;
      }
    ret=i2c_tx(L3G4200_SAD,addr,sendSize);
      if(ret>0){//if tx is able to send 1 byte( the address of the slave) 
           ret=i2c_rx(L3G4200_SAD,dat,receiveSize);
           xmeas |=dat[0];
      }else{
           printf("ret failed, ret returned %i \r\n\t",ret);
           return 0;
      }
    xmeas*=(8.75/1000);
    return xmeas;
     // printf(" \r\n\t X axis rotational rate %i deg/sec",xmeas);
}

int getY_Rotation(char **argv,unsigned short argc){
  signed int ymeas;
  unsigned char statreg[1]={L3G4200_STATUS_REG};
  unsigned char pwr[2]={L3G4200_CTRL_REG1,0x1F};
  unsigned char dat[1];
  unsigned short sendSize=1;
  unsigned short receiveSize=1;
  unsigned char addr[2]={L3G4200_OUT_Y_LOW, L3G4200_OUT_Y_HIGH};
    initI2C(port,sda,scl);
    pwrup=i2c_tx(L3G4200_SAD,pwr,2);
    stat=i2c_tx(L3G4200_SAD,statreg,1);
    i2c_rx(L3G4200_SAD,dat,1);
    ret=i2c_tx(L3G4200_SAD,addr+1,sendSize);
      if(ret>0){//if tx is able to send 1 byte( the address of the slave) 
           ret=i2c_rx(L3G4200_SAD,dat,receiveSize);
           ymeas=dat[0];
           ymeas=ymeas<<8;
      }else{
           printf("ret failed, ret returned %i \r\n\t",ret);
           return 0;
      }
    ret=i2c_tx(L3G4200_SAD,addr,sendSize);
      if(ret>0){//if tx is able to send 1 byte( the address of the slave) 
           ret=i2c_rx(L3G4200_SAD,dat,receiveSize);
           ymeas |=dat[0];
      }else{
           printf("ret failed, ret returned %i \r\n\t",ret);
           return 0;
      }
    ymeas*=(8.75/1000);
    return ymeas;
      //printf(" \r\n\t Y axis rotational rate %i deg/sec",ymeas);
}

int getZ_Rotation(char **argv,unsigned short argc){
  signed int zmeas;
  unsigned char statreg[1]={L3G4200_STATUS_REG};
  unsigned char pwr[2]={L3G4200_CTRL_REG1,0x1F};
  unsigned char dat[1];
  unsigned short sendSize=1;
  unsigned short receiveSize=1;
  unsigned char addr[2]={L3G4200_OUT_Z_LOW, L3G4200_OUT_Z_HIGH };
    initI2C(port,sda,scl);
    pwrup=i2c_tx(LIS3DH_Write_SAD,pwr,2);
    stat=i2c_tx(L3G4200_SAD,statreg,1); 
    i2c_rx(L3G4200_SAD,dat,1);
    ret=i2c_tx(L3G4200_SAD,addr+1,sendSize);
      if(ret>0){//if tx is able to send 1 byte( the address of the slave) 
           ret=i2c_rx(L3G4200_SAD,dat,receiveSize);
           zmeas=dat[0];
           zmeas=zmeas<<8;
      }else{
           printf("ret failed, ret returned %i \r\n\t",ret);
           return 0;
      }
    ret=i2c_tx(L3G4200_SAD,addr,sendSize);
      if(ret>0){//if tx is able to send 1 byte( the address of the slave) 
           ret=i2c_rx(L3G4200_SAD,dat,receiveSize);
           zmeas |=dat[0];
      }else{
           printf("ret failed, ret returned %i \r\n\t",ret);
           return 0;
      }
    zmeas*=(8.75/1000);
    return zmeas;
    //  printf(" \r\n\t Z axis rotational rate %i deg/sec",zmeas);
}

void rotationVector(char **argv, unsigned short argc){
  while(UCA2_CheckKey()==EOF){
      short xValue=getX_Rotation(0,0), yValue=getY_Rotation(0,0), zValue=getZ_Rotation(0,0);
      printf("(%i,%i,%i) %c/sec \r\n\t", xValue,yValue,zValue,248);
      ctl_timeout_wait(ctl_get_current_time()+750);
  }
}


int getRotation(char **argv, unsigned short argc){
   unsigned short busSize=1;
  unsigned char dat[1];
  unsigned char addr[1]= {L3G4200_WHO_AM_I };
  initI2C( port,sda,scl); 
  ret= i2c_tx(0x69,addr,busSize);
    printf("tx return %i \r\n\t",ret);
    printf("dat stored %s \r\n\t",*dat);
  ret=i2c_rx(0x69,dat,1);

       printf("rx return %i \r\n\t",ret);
    printf("dat stored %xh \r\n\t", dat[0]);

  //ret = i2c_txrx(0x69,addr,busSize,dat,busSize);
 // ret = i2c_rx(addr, dat , busSize);
 }


int temp(char **argv,unsigned short argc)
{
  unsigned char reg[1] = {TEMP_VAL};
  unsigned char addr = 0x4F; // all low (0x48); (0x4A A2 low, A1 High, A0 low); (0x49 A2 low, A1 low, A0 high); (0x4C A2 High, A1 low, A0 low); (0x4E A2 High, A1 High, A0 low); (0x4D A2 High, A1 Low, A0 High); (0x4F A2 High, A1 High, A0 High);
  unsigned char aptr, *temp;
  int ret;
  ret = i2c_tx(addr, reg ,1);
  printf("I2C TX return %i \r\n\t", ret);
  if(ret==1){
  ret = i2c_rx(addr,temp, 2); 
  printf("I2C RX return %i \r\n\t", ret);
  printf("%i.%02u \r\n\t",(short)((char)temp[0]),25*(temp[1]>>6));
  }
   
}


// Below is my LIS3DH code
//initalize accelerometer to measure 16 G 
int initalize_acc(char **argv,unsigned short argc){
unsigned char  pwr[2]= {LIS3DH_CTRL_REG1,0x9F};         //for sending the command to enable all axes and putting the turning the acc on
unsigned char  dat[1];         //buffer for data received
unsigned char  ctrl_reg4[2]={LIS3DH_CTRL_REG4,0x30};////set accelerometer to be 16 G responsive CTRL_REG4 (23h) 
int pwrup; 
  //block data update - continious update (0), BLE data selection (0) LSB at lower address 
 //FS set to 16 G, (11), High resoultion mode disableded (0), self test disabled (00), SPI interface mode (x) write 00110000 0x30 
unsigned char stataddr[1]= {LIS3DH_STATUS_REG};
  initI2C(port,sda,scl);            //Initalize I2c
 pwrup = i2c_tx(LIS3DH_SAD,pwr,2); //send powerup command to acc
  if(pwrup != 2){
   /// printf("Power on failed, pwrup returned: \r\n\t"); //Print debugging statement
    return 0;
  }
   pwrup = i2c_tx(LIS3DH_SAD,ctrl_reg4,2); //send set up acc command to take 16 G acceleration 
  if(pwrup != 2){
    //printf("Command Ctl register failed, i2c returned: %i \r\n\t",pwrup); //Print debugging statement
    return 0;
    }
  //take status data
  stat=i2c_tx(LIS3DH_SAD,stataddr,1);                  //poll the status register
  i2c_rx(LIS3DH_SAD,dat,1);                //get the stuff from the stat reg
   printf("Status register returned: %i \r\n\t ",dat[0]);
return 0;
}

//take all accelerometer data

signed char *get_Acceleration(char **argv,unsigned short argc){
//Declare variables up here so Xworks doesn't complain.
unsigned short sendSize    =1; //Size of data sent
unsigned short receiveSize =1; //Size of data received
unsigned char  dat[1]={0};         //buffer for data received
unsigned char addr[3]={LIS3DH_OUT_X_H,LIS3DH_OUT_Y_H,LIS3DH_OUT_Z_H}; //addresses of the low and high bits of the X acc
unsigned char stataddr[1]= {LIS3DH_STATUS_REG};
  //take acceleration data
  stat=i2c_tx(LIS3DH_SAD,stataddr,1);                  //poll the status register
  i2c_rx(LIS3DH_SAD,acc,1);                //get the stuff from the stat reg
  // printf("Status register returned: %i \r\n\t ",dat[0]);

  ret=i2c_tx(LIS3DH_SAD, addr,sendSize); //Poll the register for the X_acc high bits
  if(ret>0){  
      ret=i2c_rx(LIS3DH_SAD,acc+1,receiveSize);
  }else{
        printf("ret H on x-axis failed, ret returned: %i \r\n\t ",ret);
        return 0;
  }
  ret=i2c_tx(LIS3DH_SAD, addr+1,sendSize); //Poll the register for the X_acc high bits
  if(ret>0){  
      ret=i2c_rx(LIS3DH_SAD,acc+2,receiveSize);
  }else{
        printf("ret H on y-axis failed, ret returned: %i \r\n\t ",ret);
        return 0;
  }
  ret=i2c_tx(LIS3DH_SAD, addr+2,sendSize); //Poll the register for the X_acc high bits
  if(ret>0){  
      ret=i2c_rx(LIS3DH_SAD,acc+3,receiveSize);
  }else{
        printf("ret H on z-axis failed, ret returned: %i \r\n\t ",ret);
        return 0;
  }
  //printf("%c %c %c", x_Acc[1], x_Acc[2], x_Acc[3]);
  return acc;   
 }

/*
void accVector(char **argv, unsigned short argc){
 signed char accelx[10], accely[10],accelz[10];
 signed char data[10]; 
 int samples = 10; 
 long time[40];
 int i=0; 
  for(i=0;i<samples;i++){
  get_Acceleration(0,0);
      data[i]=acc[0];//, yValue=getY_Rotation(), zValue=getZ_Rotation();
      accelx[i]=acc[1];//, yValue=getY_Rotation(), zValue=getZ_Rotation();
      accely[i]=acc[2];//, yValue=getY_Rotation(), zValue=getZ_Rotation();
      accelz[i]=acc[3];//, yValue=getY_Rotation(), zValue=getZ_Rotation(); 
      //ctl_timeout_wait(ctl_get_current_time()+5);
      time[i]=ctl_get_current_time();
}

  for(i=0;i<samples;i++){
   printf("(data=%i, x=%i, y=%i, z=%i time= %lu \r\n", data[i],accelx[i],accely[i],accelz[i],time[i]);//
   } 
 //printf("STUPID TEST");//
}
*/
/*
void acceleration_Vector(char **argv,unsigned short argc){
 while(UCA2_CheckKey()==EOF){
  signed char* xValue=*getX_Acceleration();
    printf("(%i m/s/s \r\n\t",xValue;
    ctl_timeout_wait(ctl_get_current_time()+750);
 }
*
}*/
//table of commands with help
const CMD_SPEC cmd_tbl[]={{"help"," [command]",helpCmd},
                   {"ex","[arg1] [arg2] ...\r\n\t""Example command to show how arguments are passed",example_command},
                   {"timer_IR","[time]...\r\n\tExample command to show how the timer can be used as an interupt",example_timer_IR},
                   {"SD_write","Writes given args to the SD card",SD_write},
                   {"SD_read","",SD_read},
                   {"send_I2C","Sends I2C command to subsystem",send_I2C},
                   {"x_acc","display x acceleration",get_Acceleration},
                   {"temp","Get Temp data",temp},
                   {"gyro","Get Rotaion data",rotationVector},
                   //{"acc","get timed acceleration data",accVector},
                   {"init_acc","initalize accelerometer to take 16 G acceleration", initalize_acc},
                   ARC_COMMANDS,CTL_COMMANDS,ERROR_COMMANDS,MMC_COMMANDS,
                   //end of list
                   {NULL,NULL,NULL}};

