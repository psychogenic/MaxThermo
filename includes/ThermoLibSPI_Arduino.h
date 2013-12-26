/*
 * ThermoLibSPI_Arduino.h -- native SPI include header,
 * part of MaxThermo Thermocouple-to-digital
 * converter library for AVR and Arduino-compatible devices.
 *
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
 *
 * http://flyingcarsandstuff.com/projects/MaxThermo/
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
#ifndef MAXTHERMOLIB_SPI_ARDUINO_H_
#define MAXTHERMOLIB_SPI_ARDUINO_H_

#include "ThermoLibConfig.h"

#ifdef MAXTHERMOLIB_SPI_ARDUINO
#include <SPI.h>
#define SPI_SETUP(mode, clockdiv, msblsb)	\
	SPI.setDataMode(mode);	\
	SPI.setBitOrder(msblsb); \
	SPI.setClockDivider(clockdiv)

#define  SPI_BEGIN(sck_pin, miso_pin, mosi_pin)			SPI.begin()

#define SPI_TRANSFER(b)		SPI.transfer(b)

#endif



#endif /* MAXTHERMOLIBSPI_ARDUINO_H_ */
