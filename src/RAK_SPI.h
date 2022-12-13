
/**
   @file RAK_SPI.h
   @author rakwireless.com
   @brief  This code is designed to config SPI device
   @version 0.1
   @date 2022-06-15
   @copyright Copyright (c) 2022
**/
#ifndef _RAK_SPI_H_
#define _RAK_SPI_H_

#include <Arduino.h>
#include <SPI.h>

#if defined(_VARIANT_RAK4630_)
extern SPIClass SPI_USER;
#elif defined(_VARIANT_RAK11300_)
extern MbedSPI SPI_USER;
#else
extern SPIClass SPI_USER;
#endif

enum class SPIClock
{
    SLOW,
    FAST
};

namespace RAK_SPI
{
    /**
    Initializes the SPI bus.
    */
    void SPIinit();

    /**
    This basically just frees SPI and the previously used pins.
    */
    void SPIend();

    /**
     Used in case you switched SPI to another device.
    */
    void SPIselect(uint8_t slaveSelectPIN);

    /**
    Arduino function to write to the SPI.
    Takes two separate byte buffers for write header and write data

    @param [in] Header lenght
    @param [in] Header array built before
    @param [in] Data lenght
    @param [in] Data array
    */
    void writeToSPI(uint8_t slaveSelectPIN, uint8_t headerLen, byte header[], uint16_t dataLen, byte data[]);

    /**
    Arduino function to read from the SPI.
    Takes two separate byte buffers for write header and write data

    @param [in] Header lenght
    @param [in] Header array built before
    @param [in] Data lenght
    @param [in] Data array
    */
    void readFromSPI(uint8_t slaveSelectPIN, uint8_t headerLen, byte header[], uint16_t dataLen, byte data[]);

    /**
    Sets speed of SPI clock, fast or slow(20MHz or 2MHz)

    @param [in] SPIClock FAST or SLOW
    */
    void setSPIspeed(SPIClock speed);

    void _openSPI(uint8_t slaveSelectPIN);

    void _closeSPI(uint8_t slaveSelectPIN);
}

#endif
