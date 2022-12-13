/**
   @file RAK15007_CY15B108QN.cpp
   @author rakwireless.com
   @brief  This is a library for the RAK SPI FRAM config
   @version 0.1
   @date 2022-06-15
   @copyright Copyright (c) 2022
**/
#include "RAK15007_CY15B108QN.h"

// flash devices
const struct
{
  uint16_t deviceID; // device ID
  uint32_t size;     // fram capacity(unit:byte)
} fram_device[] = {
    // infineon
    {0x2C40, 512 * 1024UL}, // CY15B104QN (device = 7F7F7F7F7F7FC22C40)
    {0x2E03, 1024 * 1024UL} // CY15B108QN(device = 7F7F7F7F7F7FC22E03)
};

/**
 * @brief Construct a new RAK_FRAM_CY15B108QN::RAK_FRAM_CY15B108QN object
 * @param cs spi cs
 */
RAK_FRAM_CY15B108QN::RAK_FRAM_CY15B108QN(uint8_t cs)
{
  _spi_cs = cs;
}

/**
 *   @brief  Reads the deviceID and the uniqueID from the IC
 *   @param  deviceID The 9-Byte Device ID
 *   @param  uniqueID The 8-Byte Unique ID
 */
void RAK_FRAM_CY15B108QN::getDeviceID(uint16_t *deviceID, uint8_t *uniqueID)
{
  uint8_t deviceIDcmd = OPCODE_RDID;
  uint8_t uniqueIDcmd = OPCODE_RUID;
  uint8_t fram_deviceID[9] = {0};

  RAK_SPI::readFromSPI(_spi_cs, 0x01, &deviceIDcmd, 0x09, fram_deviceID);
  delay(100);
  RAK_SPI::readFromSPI(_spi_cs, 0x01, &uniqueIDcmd, 0x08, uniqueID);

  uint16_t bufID = 0x00;
  bufID = (fram_deviceID[1] << 8) | fram_deviceID[0];
  if (bufID == fram_device[0].deviceID)
  {
    *deviceID = bufID;
  }
  else if (bufID == fram_device[1].deviceID)
  {
    *deviceID = bufID;
  }
  else
  {
    *deviceID = 0x00;
  }
}

/**
 * @brief Special sector read
 * @param fram_address  address containing the 8-bit sector address
 * @param  values The pointer to an array of 8-bit values to read starting at address
 * @param count  The number of bytes to read
 */
void RAK_FRAM_CY15B108QN::specialSectorRead(uint8_t fram_address, uint8_t *values, size_t count)
{
  uint8_t wBuf[10] = {0x00};
  uint8_t wCount = 0;

  wBuf[wCount++] = OPCODE_SSRD;
  if (_framSizeBytes > 3)
    wBuf[wCount++] = (uint8_t)(0x00 >> 24);
  if (_framSizeBytes > 2)
    wBuf[wCount++] = (uint8_t)(0x00 >> 16);
  wBuf[wCount++] = (uint8_t)(0x00 >> 8);
  wBuf[wCount++] = (uint8_t)(fram_address & 0xFF);

  RAK_SPI::readFromSPI(_spi_cs, wCount, wBuf, count, values);
}

/**
 * @brief Special sector write
 * @param fram_address  address containing the 8-bit sector address
 * @param values The pointer to an array of 8-bit values to write starting at address
 * @param count  The number of bytes to write
 */
void RAK_FRAM_CY15B108QN::specialSectorWrite(uint8_t fram_address, uint8_t *values, size_t count)
{
  uint8_t wBuf[10] = {0x00};
  uint8_t wCount = 0;
  wBuf[wCount++] = OPCODE_SSWR;
  if (_framSizeBytes > 3)
    wBuf[wCount++] = (uint8_t)(0x00 >> 24);
  if (_framSizeBytes > 2)
    wBuf[wCount++] = (uint8_t)(0x00 >> 16);
  wBuf[wCount++] = (uint8_t)(0x00 >> 8);
  wBuf[wCount++] = (uint8_t)(fram_address & 0xFF);
  RAK_SPI::writeToSPI(_spi_cs, wCount, wBuf, count, values);
}

/**
 * @brief Fast read memory data
 * @param fram_address  address containing the 8-bit sector address
 * @param values The pointer to an array of 8-bit values to read starting at address
 * @param count  The number of bytes to read
 */
void RAK_FRAM_CY15B108QN::fastReadOperation(uint8_t fram_address, uint8_t *values, size_t count)
{
  uint8_t wBuf[10] = {0x00};
  uint8_t wCount = 0;

  wBuf[wCount++] = OPCODE_FSTRD;
  if (_framSizeBytes > 3)
    wBuf[wCount++] = (uint8_t)(fram_address >> 24);
  if (_framSizeBytes > 2)
    wBuf[wCount++] = (uint8_t)(fram_address >> 16);
  wBuf[wCount++] = (uint8_t)(fram_address >> 8);
  wBuf[wCount++] = (uint8_t)(fram_address & 0xFF);
  wBuf[wCount++] = DUMMY;

  RAK_SPI::_openSPI(_spi_cs);
  SPI_USER.transfer(wBuf, wCount);

  for (auto i = 0; i < count; i++)
  {
    values[i] = SPI_USER.transfer(0x00); // read values
  }
  RAK_SPI::_closeSPI(_spi_cs);
}

/**
    @brief  Reads the status register
    @return the status register value
*/
uint8_t RAK_FRAM_CY15B108QN::getStatusRegister()
{
  uint8_t cmd, val;

  cmd = OPCODE_RDSR;

  RAK_SPI::readFromSPI(_spi_cs, 0x01, &cmd, 0x01, &val);
  return val;
}

/**
 *   @brief  Sets the status register
 *   @param  value value that will be set
 */
void RAK_FRAM_CY15B108QN::setStatusRegister(uint8_t value)
{
  uint8_t cmd[2];

  cmd[0] = OPCODE_WRSR;
  cmd[1] = value;

  RAK_SPI::writeToSPI(_spi_cs, 0x01, &cmd[0], 0x01, &cmd[1]);
}

/**
 *   @brief  Sets adress size to provided value
 *   @param  nAddressSize address size in bytes
 */
void RAK_FRAM_CY15B108QN::setAddressSize(uint8_t nAddressSize)
{
  _framSizeBytes = nAddressSize;
}

/**
 * @brief Set serial Number
 * @param write_values The pointer to an array of 8-bit values to write starting at address
 * @param count  The number of bytes to write
 */
void RAK_FRAM_CY15B108QN::setSerialNumber(uint8_t *write_values, size_t count)
{
  uint8_t cmd;

  cmd = OPCODE_WRSN;

  RAK_SPI::writeToSPI(_spi_cs, 0x01, &cmd, count, write_values);
}

/**
 * @brief Get serial number
 * @param read_values The pointer to an array of 8-bit values to read starting at address
 * @param count  The number of bytes to read
 */
void RAK_FRAM_CY15B108QN::getSerialNumber(uint8_t *read_values, size_t count)
{
  uint8_t cmd;

  cmd = OPCODE_RDSN;

  RAK_SPI::readFromSPI(_spi_cs, 0x01, &cmd, count, read_values);
}



/**
 *  @brief  Initializes SPI and configures the chip
 *  @return true if successful,false if failed
 */
bool RAK_FRAM_CY15B108QN::begin()
{
  RAK_SPI::SPIinit();
  RAK_SPI::SPIselect(_spi_cs);
  RAK_SPI::setSPIspeed(SPIClock::FAST);

  uint16_t framDeviceID = 0x00;
  uint8_t framUniqueID[8] = {0};
  getDeviceID(&framDeviceID, framUniqueID);

  uint32_t fram_size = 0;

  for (uint8_t typeCount = 0; typeCount < 2; typeCount++)
  {
    if (framDeviceID == fram_device[typeCount].deviceID)
    {
      fram_size = fram_device[typeCount].size;
    }
  }
  if (fram_size > 64UL * 1024)
  {
    setAddressSize(3);
  }
  else
  {
    setAddressSize(2);
  }

  return fram_size != 0;
}

/**
 * @brief Get the capacity of the chip
 *
 * @return the chip capacity(unit:byte)
 */
uint32_t RAK_FRAM_CY15B108QN::getFramSize(void)
{
  uint32_t f_size = 0;
  uint16_t framDeviceID = 0x00;
  uint8_t framUniqueID[8] = {0};
  getDeviceID(&framDeviceID, framUniqueID);
  for (uint8_t fcount = 0; fcount < sizeof(fram_device) / sizeof(fram_device[0]); fcount++)
  {
    if (framDeviceID == fram_device[fcount].deviceID)
      f_size = fram_device[fcount].size;
  }

  if (f_size != 0)
  {
    return f_size;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Enables or disables writing to the SPI flash
 * @param enable True enables writes, false disables writes
 */
void RAK_FRAM_CY15B108QN::writeEnable(bool enable)
{
  uint8_t opcode;
  if (enable)
  {
    opcode = OPCODE_WREN;
  }
  else
  {
    opcode = OPCODE_WRDI;
  }
  RAK_SPI::writeToSPI(_spi_cs, 0x01, &opcode, 0x00, 0x00);
}

/**
 *  @brief  Writes a byte at the specific FRAM address
 *  @param addr  The 32-bit address to write to in FRAM memory
 *  @param value The 8-bit value to write at framAddr
 */
void RAK_FRAM_CY15B108QN::writeByte(uint32_t fram_address, uint8_t value)
{
  uint8_t wBuf[10] = {0x00};
  uint8_t wCount = 0;
  wBuf[wCount++] = OPCODE_WRITE;
  if (_framSizeBytes > 3)
    wBuf[wCount++] = (uint8_t)(fram_address >> 24);
  if (_framSizeBytes > 2)
    wBuf[wCount++] = (uint8_t)(fram_address >> 16);
  wBuf[wCount++] = (uint8_t)(fram_address >> 8);
  wBuf[wCount++] = (uint8_t)(fram_address & 0xFF);

  RAK_SPI::writeToSPI(_spi_cs, wCount, wBuf, 0x01, &value);
}

/*!
 *   @brief  Writes count bytes starting at the specific FRAM address
 *   @param addr   The 32-bit address to write to in FRAM memory
 *   @param values The pointer to an array of 8-bit values to write starting at addr
 *   @param count  The number of bytes to write
 */
void RAK_FRAM_CY15B108QN::write(uint32_t fram_address, uint8_t *values, size_t count)
{
  uint8_t wBuf[10] = {0x00};
  uint8_t wCount = 0;
  wBuf[wCount++] = OPCODE_WRITE;
  if (_framSizeBytes > 3)
    wBuf[wCount++] = (uint8_t)(fram_address >> 24);
  if (_framSizeBytes > 2)
    wBuf[wCount++] = (uint8_t)(fram_address >> 16);
  wBuf[wCount++] = (uint8_t)(fram_address >> 8);
  wBuf[wCount++] = (uint8_t)(fram_address & 0xFF);
  RAK_SPI::writeToSPI(_spi_cs, wCount, wBuf, count, values);
}

/*!
 *   @brief  Reads an 8 bit value from the specified FRAM address
 *   @param  addr The 32-bit address to read from in FRAM memory
 *   @return The 8-bit value retrieved at framAddr
 */
uint8_t RAK_FRAM_CY15B108QN::readByte(uint32_t fram_address)
{
  uint8_t wBuf[10] = {0x00};
  uint8_t val = 0x00;
  uint8_t wCount = 0;

  wBuf[wCount++] = OPCODE_READ;
  if (_framSizeBytes > 3)
    wBuf[wCount++] = (uint8_t)(fram_address >> 24);
  if (_framSizeBytes > 2)
    wBuf[wCount++] = (uint8_t)(fram_address >> 16);
  wBuf[wCount++] = (uint8_t)(fram_address >> 8);
  wBuf[wCount++] = (uint8_t)(fram_address & 0xFF);

  RAK_SPI::readFromSPI(_spi_cs, wCount, wBuf, 0x01, &val);
  return val;
}

/*!
 *   @brief  Read count bytes starting at the specific FRAM address
 *   @param  addr The 32-bit address to write to in FRAM memory
 *   @param  values The pointer to an array of 8-bit values to read starting at addr
 *   @param  count The number of bytes to read
 */
void RAK_FRAM_CY15B108QN::read(uint32_t fram_address, uint8_t *values, size_t count)
{
  uint8_t wBuf[10] = {0x00};
  uint8_t wCount = 0;

  wBuf[wCount++] = OPCODE_READ;
  if (_framSizeBytes > 3)
    wBuf[wCount++] = (uint8_t)(fram_address >> 24);
  if (_framSizeBytes > 2)
    wBuf[wCount++] = (uint8_t)(fram_address >> 16);
  wBuf[wCount++] = (uint8_t)(fram_address >> 8);
  wBuf[wCount++] = (uint8_t)(fram_address & 0xFF);

  RAK_SPI::readFromSPI(_spi_cs, wCount, wBuf, count, values);
}

