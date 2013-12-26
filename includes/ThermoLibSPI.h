/*
 * ThermoLibSPI.h -- SPI selection auto-includer,
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

#ifndef MAXTHERMOLIBSPI__MAININCLUDE_H_
#define MAXTHERMOLIBSPI__MAININCLUDE_H_

#include "ThermoLibConfig.h"
#ifdef MAXTHERMOLIB_SPI_ARDUINO
#include "ThermoLibSPI_Arduino.h"
#elif defined(MAXTHERMOLIB_SPI_BITBANG)
#include "ThermoLibSPI_Bitbang.h"
#else
#error "Must select an SPI implementation MAXTHERMOLIB_SPI_*";

#endif


#endif /* THERMOLIBSPI_H_ */
