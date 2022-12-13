/**
   @file RAK15007_CY15B108QN.h
   @author rakwireless.com
   @brief  This is a library for the RAK SPI FRAM config
   @version 0.1
   @date 2022-06-15
   @copyright Copyright (c) 2022
**/

#ifndef _RAK15007_CY15B108QN_H_
#define _RAK15007_CY15B108QN_H_

#include <Arduino.h>
#include <stdlib.h>
#include <SPI.h>
#include <math.h>
#include "RAK_SPI.h"

// WREN Set write enable latch 0000 0110b
#define OPCODE_WREN 0x06

// WRDI Reset write enable latch 0000 0100b
#define OPCODE_WRDI 0x04

// RDSR Read Status Register 0000 0101b
#define OPCODE_RDSR 0x05

// WRSR Write Status Register 0000 0001b
#define OPCODE_WRSR 0x01

// WRITE Write memory data 0000 0010b
#define OPCODE_WRITE 0x02

// READ Read memory data 0000 0011b
#define OPCODE_READ 0x03

// FSTRD Fast read memory data 0000 1011b
#define OPCODE_FSTRD 0x0B

// SSWR Special Sector Write  0100 0010b
#define OPCODE_SSWR 0x42

// SSRD Special Sector Read   0100 1011b
#define OPCODE_SSRD 0x4B

// RDID Read device ID 1001 1111b
#define OPCODE_RDID 0x9F

// RUID Read Unique ID 4Ch 0100 1100b
#define OPCODE_RUID 0x4C

// WRSN  Write Serial Number  1100 0010b
#define OPCODE_WRSN 0xC2

// RDSN  Read Serial Number    11000 011b
#define OPCODE_RDSN 0xC3


// DPD Enter Deep Power-Down BAh 1011 1010b
#define OPCODE_DPD 0xBA

// HBN Enter Hibernate mode B9h 1011 1001b
#define OPCODE_HBN 0xB9

// The dummy byte can be any 8-bit value but Axh(1010xxxx),00h is typically used as the dummy byte in most use cases
#define DUMMY 0x00

#define DEVICEID_0_7 0x7F7F7F7F7F7FC2

class RAK_FRAM_CY15B108QN
{
public:
  RAK_FRAM_CY15B108QN(uint8_t cs);

  bool begin();
  void setSerialNumber(uint8_t *write_values, size_t count);
  void getSerialNumber(uint8_t *read_values, size_t count);

  uint32_t getFramSize(void);
  void writeEnable(bool enable);

  void writeByte(uint32_t fram_address, uint8_t value);
  void write(uint32_t fram_address, uint8_t *values, size_t count);

  uint8_t readByte(uint32_t fram_address);
  void read(uint32_t fram_address, uint8_t *values, size_t count);

  void getDeviceID(uint16_t *deviceID, uint8_t *uniqueID);
  uint8_t getStatusRegister(void);
  void setStatusRegister(uint8_t value);
  void setAddressSize(uint8_t nAddressSize);

  void specialSectorWrite(uint8_t fram_address, uint8_t *values, size_t count);
  void specialSectorRead(uint8_t fram_address, uint8_t *values, size_t count);

  void fastReadOperation(uint8_t fram_address, uint8_t *values, size_t count);

private:
  uint8_t _framSizeBytes = 2;
  uint8_t _spi_cs;
};

#endif
