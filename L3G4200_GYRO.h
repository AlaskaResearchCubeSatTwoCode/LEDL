//Kyle Tam
// Feb. 17, 2017
// LEDL gyroscope header

#ifndef _L3G4200_GYRO_H
#define _L3G4200_GYRO_H

//SAD= 110100xb if SDO is high LSB is 1, if SDO is low LSB is 0
//fall edge triggered

//fixed address bits
#define L3G4200_SAD                     0x69
#define L3G4200_READ_SAD                0xD3
#define L3G4200_Write_SAD               0xD2

///////////////////// 8-bit Register Addresses////////////////// 
#define L3G4200_WHO_AM_I                0x0F  //READ                          //Device ID 
#define L3G4200_REFERENCE/DATACAPTURE   0X25  //READ-WRITE                   //Reference value for interrupt generation. default:0
#define L3G4200_OUT_TEMP                0x26  //READ                         //Temperature data
#define L3G4200_STATUS_REG              0x27  //READ

///Control Registers
#define L3G4200_CTRL_REG1               0x20  //ALL are READ-WRITE
#define L3G4200_CTRL_REG2               0x21
#define L3G4200_CTRL_REG3               0x22
#define L3G4200_CTRL_REG4               0x23
#define L3G4200_CTRL_REG5               0x24

///Axis Registers 
#define L3G4200_OUT_X_LOW               0x28  //ALL are READ ONLY           //_-Axis angular rate expressed as 2's comp
#define L3G4200_OUT_X_HIGH              0x29
#define L3G4200_OUT_Y_LOW               0x2A
#define L3G4200_OUT_Y_HIGH              0x2B
#define L3G4200_OUT_Z_LOW               0x2C
#define L3G4200_OUT_Z_HIGH              0x2D

///FIFO Registers
#define L3G4200_FIFO_CTRL_REG           0x2E  //READ-WRITE
#define L3G4200_FIFO_SRC_REG            0x2F  //READ

///Interrupt Registers
#define L3G4200_INT1_CFG                0x30  // READ-WRITE
#define L3G4200_INT1_SRC                0x31  // READ
#define L3G4200_INT1_TSH_XH             0x32  // READ-WRITE
#define L3G4200_INT1_TSH_XL             0x33  // READ-WRITE
#define L3G4200_INT1_TSH_YH             0x34  // READ-WRITE
#define L3G4200_INT1_TSH_YL             0x35  // READ-WRITE
#define L3G4200_INT1_TSH_ZH             0x36  // READ-WRITE
#define L3G4200_INT1_TSH_ZL             0x37  // READ-WRITE
#define L3G4200_INT1_DURATION           0x38  // READ-WRITE


#endif