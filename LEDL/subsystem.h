//This file will cover all stack up events
#ifndef __SUBSYSTEM_H
#define __SUBSYSTEM_H
#include "arcbus.h"

enum{SYS_EV_1=1,SYS_EV_2,SYS_EV_3,SYS_EV_4};  // These are events in SYS_evt, add more as necessary 
#define SYS_EVT_ALL (SYS_EV_1|SYS_EV_2|SYS_EV_3|SYS_EV_4)

void sub_events(void *p);

void sys_events(void *p);
extern CTL_EVENT_SET_t SYS_evt;// This creates the event struct,change SYS to actual subsystem

extern CMD_PARSE_DAT LEDL_parse;

#include <Error.h>
#define TEMP_ARRAY_SIZE       7 //array is 7 16 byte column 
#define LAUNCH_DATA_SIZE      512
#define CLYDE_ARRAY_SIZE      20//we are only sampling 20 of the possible clyde sensors 
#define MAG_ARRAY_SIZE        6//there are 6 magnetometers
#define GYRO_ARRAY_SIZE       3//there are 3 gyroscopes 
#define LEDL_DATA_ID          0x3311//this is specified by the data block definitions (launch data)
#define LEDL_DETECT_ID        0x2121//this is launch_detect data

#define LEDL_PACKETSIZE_FOR_CDH   15//this is specified by becon each spot is different length,
#define CLYDE_PACKETSIZE_FOR_CDH  15//this is specified by becon each spot is 16 bits 
#define LEDL_PACKETSIZE_FOR_ACDS  9//each packet has 6 mag and 3 gyro measurements

#define SD_EV_WRITE_1         0x01
#define SD_EV_WRITE_2         0x02 
#define SD_EV_DIE             0x04
#define SD_EV_FINISHED        0x08
#define SD_EV_ALL             (SD_EV_WRITE_1|SD_EV_WRITE_2|SD_EV_DIE|SD_EV_FINISHED)

#define LEDL_EV_GET_TEMP_DATA  0x01
#define LEDL_EV_EPS_CMD        0x02
#define LEDL_EV_SEND_DAT       0x04
#define LEDL_EV_BLOW_FUSE      0x08
#define LEDL_EV_STATUS_TIMEOUT 0x10
#define LEDL_EV_ALL            (LEDL_EV_EPS_CMD|LEDL_EV_GET_TEMP_DATA|LEDL_EV_SEND_DAT|LEDL_EV_BLOW_FUSE|LEDL_EV_STATUS_TIMEOUT)

#define CLYDE_EV_GET_DATA     0x01
#define LAUNCHDATA_FLAG       0x01
#define ORBITDATA_FLAG        0x01

//LEDL data starts after error data
#define SD_LAUNCH_DATA_START  (ERR_ADDR_END+936)//this starts at 1000
#define SD_BECON_DATA         (ERR_ADDR_END+1)//start our beacon data at 65
#define launch_didNot_happened 0xAA
#define launch_happened        0x22
#define MODE_DETECT            0
#define MODE_LAUNCH            1
#define MODE_ORBIT             2
#endif
