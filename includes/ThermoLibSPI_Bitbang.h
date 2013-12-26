/*
 * ThermoLibSPI_Bitbang.h -- bit-banged SPI header,
 * part of MaxThermo Thermocouple-to-digital
 * converter library for AVR and Arduino-compatible devices.
 *
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
 *
 * http://www.flyingcarsandstuff.com/projects/MaxThermo/
 *
 *
 * Please let me know if you use MaxThermo in your projects, and
 * provide a URL if you'd like me to link to it from the library
 * home.
 *
 *
 * This program library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See file LICENSE for further informations on licensing
 * terms.
 */

#ifndef MAXTHERMOLIB_SPI_BITBANG_HEADER_H_
#define MAXTHERMOLIB_SPI_BITBANG_HEADER_H_

#include "ThermoLibIncludes.h"
#include "ThermoLibConfig.h"

#ifdef MAXTHERMOLIB_SPI_BITBANG

#ifndef MAXTHERMO_SUPPORTS_CUSTOM_SPI_PINS
#define MAXTHERMO_SUPPORTS_CUSTOM_SPI_PINS
#endif



#ifndef LSBFIRST
#define LSBFIRST 0
#define MSBFIRST 1
#endif

#ifndef SPI_CLOCK_DIV2
#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06
#endif

#ifndef SPI_MODE0
#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C
#endif

#ifndef F_CPU
#define F_CPU	16000000L
#endif

void SPIBitbang_begin(uint8_t sck_pin, uint8_t miso_pin, uint8_t mosi_pin);
uint8_t SPIBitbang_transfer(uint8_t b);
void SPIBitbang_setup(uint8_t mode, uint8_t div, uint8_t msblsb);


#define SPI_SETUP(mode, clockdiv, msblsb)	SPIBitbang_setup(mode, clockdiv, msblsb)


#define SPI_BEGIN(sck_pin, miso_pin, mosi_pin)			SPIBitbang_begin(sck_pin, miso_pin, mosi_pin)


#define SPI_TRANSFER(b)		SPIBitbang_transfer(b)


#endif




#endif /* MAXTHERMOLIBSPI_BITBANG_H_ */
