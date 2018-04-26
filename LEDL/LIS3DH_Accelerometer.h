//Kyle Tam
// JAN 24, 2018
// LEDL I2C Accelerometer header

#ifndef _LIS3DH_ACCELEROMETER_H
#define _LIS3DH_ACCELEROMETER_H

//SAD if SDO is tied to ground
//#define LIS3DH_READ       0x31 
//#define LIS3DH_Write      0x30
// if SDO is tied high
#define LIS3DH_SAD               0x19//based on SD0 being floating, the pin is pulled up interanlly in device 0b0011001 is address
#define LIS3DH_READ_SAD          0x33
#define LIS3DH_Write_SAD         0x32

////MISC REGISTERS//
#define LIS3DH_STATUS_REG_AUX    0x07 //Read
#define LIS3DH_WHO_AM_I          0x0F //Read
#define LIS3DH_TEMP_CFG_REG      0x1F //Read-write
#define LIS3DH_REFERENCE         0x26 //Read-write
#define LIS3DH_STATUS_REG        0x27 //Read
#define LIS3DH_TIME_LIMIT        0x3B //Read-Write
#define LIS3DH_TIME_LATENCY      0x3C //Read-Write
#define LIS3DH_TIME_WINDOW       0x3D //Read-write
#define LIS3DH_ACT_THS           0x3E //Read-Write
#define LIS3DH_ACT_DUR           0x3F //Read-Write

//10-bit ADC output REGISTERS
#define LIS3DH_OUT_ADC1_L        0x08 //All are read
#define LIS3DH_OUT_ADC1_H        0x09
#define LIS3DH_OUT_ADC2_L        0x0A
#define LIS3DH_OUT_ADC2_H        0x0B
#define LIS3DH_OUT_ADC3_L        0x0C
#define LIS3DH_OUT_ADC3_H        0x0D

//CTRL REGISTERS
#define LIS3DH_CTRL_REG0         0x1E //All are read-write /*Pull up Enable*/
#define LIS3DH_PU_EN             0x10
#define LIS3DH_PU_DIS            0x90

#define LIS3DH_CTRL_REG1         0x20 /*data rate, LMP and Z, Y, X, enable*/
#define LIS3DH_CTRL_REG2         0x21 /*Filter Selection*/
#define LIS3DH_CTRL_REG3         0x22 /*Interrupt 1 Enable*/
#define LIS3DH_CTRL_REG4         0x23
#define LIS3DH_CTRL_REG5         0x24
#define LIS3DH_CTRL_REG6         0x25 /*Interrupt 2 Enable*/

//Axis output REGISTERS
#define LIS3DH_OUT_X_L           0x28 //ALL are read
#define LIS3DH_OUT_X_H           0x29
#define LIS3DH_OUT_Y_L           0x2A
#define LIS3DH_OUT_Y_H           0x2B
#define LIS3DH_OUT_Z_L           0x2C
#define LIS3DH_OUT_Z_H           0x2D

//FIFO REGISTERS
#define LIS3DH_FIFO_CTRL_REG     0x2E //Read-Write
#define LIS3DH_FIFO_SRC_REG      0x2F //Read

//Interrupt Registers

//Interrupt 1
#define LIS3DH_INT1_CFG          0x30 //Read-Write
#define LIS3DH_INT1_SRC          0x31 //Read
#define LIS3DH_INT1_THS          0x32 //Read-write
#define LIS3DH_INT1_DURATION     0x33 //Read-Write

//Interrupt 2
#define LIS3DH_INT2_CFG          0x34 //Read-Write
#define LIS3DH_INT2_SRC          0x35 //Read
#define LIS3DH_INT2_THS          0x36 //Read-write
#define LIS3DH_INT2_DURATION     0x37 //Read-Write

//Click interrupts
#define LIS3DH_CLICK_CFG         0x38 //Read-Write
#define LIS3DH_CLICK_SRC         0x39 //Read
#define LIS3DH_CLICK_THS         0x3A //Read-write






#endif