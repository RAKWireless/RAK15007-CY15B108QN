| <center><img src="./assets/rakstar.jpg" alt="RAKstar" width=25%></center>  | ![RAKWireless](./assets/RAK-Whirls.png) | [![Build Status](https://github.com/RAKWireless/RAK13005-TLE7259-Library/workflows/RAK%20Library%20Build%20CI/badge.svg)](https://github.com/RAKWireless/RAK13005-TLE7259-Library/actions) |
| -- | -- | -- |

# <RAK15007>

RAK15007-CY15B108QN is written by RAK for Infineon CY15B108QN. It provides functions such as FRAM reading and writing.

[*RAKWireless RAK15007*](https://store.rakwireless.com/collections/wisblock-sensor)

# Documentation

* **[Product Repository](https://github.com/RAKWireless/RAK15007-CY15B108QN)** - Product repository for the RAKWireless RAK15007 FRAM module.
* **[Documentation](https://docs.RAKWireless.com/Product-Categories/WisBlock/RAK15007/Overview/)** - Documentation and Quick Start Guide for the RAK15007 FRAM  module.

# Installation

In Arduino IDE open Sketch->Include Library->Manage Libraries then search for RAK15007 .

In PlatformIO open PlatformIO Home, switch to libraries and search for RAK15007 .
Or install the library project dependencies by adding

```log
lib_deps =
  RAKWireless/RAKWireless RAK15007-CY15B108QN library
```

into **`platformio.ini`**

For manual installation download the archive, unzip it and place the RAK15007-CY15B108QN  folder into the library directory.
In Arduino IDE this is usually <arduinosketchfolder>/libraries/
In PlatformIO this is usually <user/.platformio/lib>

# Usage

The library provides RAK_FRAM_CY15B108QN class, which allows communication with CY15B108QN via SPI. These examples show how to use RAK15007.

- [RAK15007_FRAM_Read_Write_CY15B108QN](./examples/RAK15007_FRAM_Read_Write_CY15B108QN)    Read and Write CY15B108QN sensor.
- [RAK15007_FRAM_Write_Protect_CY15B108QN](./examples/RAK15007_FRAM_Write_Protect_CY15B108QN) Test write protection.

## This class provides the following methods:

**bool begin()**

Initializes SPI and configures the chip.

#### Parameters:

| Direction | Name | Function                           |
| --------- | ---- | ---------------------------------- |
| return    |      | true if successful,false if failed |

**void getDeviceID(uint16_t *deviceID, uint8_t *uniqueID) **

Reads the deviceID and the uniqueID from the IC.

#### Parameters:

| Direction | Name     | Function             |
| --------- | -------- | -------------------- |
| in        | deviceID | The 9-Byte Device ID |
| in        | uniqueID | The 8-Byte Unique ID |
| return    |          | none                 |

**void specialSectorRead(uint8_t fram_address, uint8_t *values, size_t count) **

Special sector read.

#### Parameters:

| Direction | Name         | Function                                                     |
| --------- | ------------ | ------------------------------------------------------------ |
| in        | fram_address | address containing the 8-bit sector address                  |
| out       | values       | The pointer to an array of 8-bit values to read starting at address |
| in        | count        | The number of bytes to read                                  |
| return    |              | none                                                         |

**void specialSectorWrite(uint8_t fram_address, uint8_t *values, size_t count) **

Special sector write.

#### Parameters:

| Direction | Name         | Function                                                     |
| --------- | ------------ | ------------------------------------------------------------ |
| in        | fram_address | address containing the 8-bit sector address                  |
| in        | values       | The pointer to an array of 8-bit values to write starting at address |
| in        | count        | The number of bytes to write                                 |
| return    |              | none                                                         |

**void fastReadOperation(uint8_t fram_address, uint8_t *values, size_t count)**

Fast read memory data.

#### Parameters:

| Direction | Name         | Function                                                     |
| --------- | ------------ | ------------------------------------------------------------ |
| in        | fram_address | address containing the 8-bit sector address                  |
| out       | values       | The pointer to an array of 8-bit values to read starting at address |
| in        | count        | The number of bytes to read                                  |
| return    |              | none                                                         |

**uint8_t getStatusRegister()**

Reads the status register.

#### Parameters:

| Direction | Name | Function                  |
| --------- | ---- | ------------------------- |
| return    |      | the status register value |

**void setStatusRegister(uint8_t value)**

Sets the status register.

#### Parameters:

| Direction | Name  | Function               |
| --------- | ----- | ---------------------- |
| in        | value | value that will be set |
| return    |       | none                   |

**void setSerialNumber(uint8_t *write_values, size_t count)**

Set serial Number.

#### Parameters:

| Direction | Name         | Function                                                     |
| --------- | ------------ | ------------------------------------------------------------ |
| in        | write_values | The pointer to an array of 8-bit values to write starting at address |
| in        | count        | The number of bytes to write                                 |
| return    |              | none                                                         |

**void getSerialNumber(uint8_t *read_values, size_t count) **

Get serial number.

#### Parameters:

| Direction | Name        | Function                                                     |
| --------- | ----------- | ------------------------------------------------------------ |
| out       | read_values | The pointer to an array of 8-bit values to read starting at address |
| in        | count       | The number of bytes to read                                  |
| return    |             | none                                                         |

**uint32_t getFramSize(void)  **

Get the capacity of the chip.

#### Parameters:

| Direction | Name | Function                     |
| --------- | ---- | ---------------------------- |
| return    |      | the chip capacity(unit:byte) |

**void writeEnable(bool enable)**

Enables or disables writing to the SPI flash.

#### Parameters:

| Direction | Name   | Function                                   |
| --------- | ------ | ------------------------------------------ |
| in        | enable | True enables writes, false disables writes |
| return    |        | none                                       |
