/*
 * ThermoLibConfig.h, global compilation configuration
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

#ifndef MAXTHERMOLIB_CONFIG_H_
#define MAXTHERMOLIB_CONFIG_H_

#define MAXTHERMO_LIB_VERSION		1
#define MAXTHERMO_LIB_SUBVERSION	0

#define MAXTHERMO_VERSION_AT_LEAST(v, sub)			( (MAXTHERMO_LIB_SUBVERSION >= sub) or (MAXTHERMO_LIB_VERSION > v))

/*
 * MAXTHERMOLIB_PLATFORM
 * Select the platform to use for accessing the microcontroller.
 *
 * Currently supported:
 * 	MAXTHERMOLIB_PLATFORM_ARDUINO
 *
 * Other platforms, such as raw AVR, should be easy to support as
 * everything is setup to make the code independent of these details.
 */
#define MAXTHERMOLIB_PLATFORM_ARDUINO




/*
 * MAXTHERMOLIB_SPI
 *
 * SPI support method.  By default, MaxThermo "bit-bangs" the SPI
 * exchanges between the uC and thermocouple.  The advantage is that
 * you can use *any* pins for the clock and serial data, at the cost
 * of adding extra weight to the library footprint.
 *
 * If you are using Arduino, and want to reduce the library's size,
 * you can define
 * 	MAXTHERMOLIB_SPI_ARDUINO
 * instead of the default MAXTHERMOLIB_SPI_BITBANG.
 *
 * Available options:
 * 		MAXTHERMOLIB_SPI_BITBANG (default)
 * 		MAXTHERMOLIB_SPI_ARDUINO (Arduino SPI.* methods)
 *
 */
// #define MAXTHERMOLIB_SPI_ARDUINO
#define MAXTHERMOLIB_SPI_BITBANG




//#define DEBUG


#if defined(MAXTHERMOLIB_PLATFORM_ARDUINO)
#define CUSTOM_ATEXIT_CXX_METHOD
#endif

// sanity checks...

#ifdef MAXTHERMOLIB_SPI_ARDUINO
	#ifndef MAXTHERMOLIB_PLATFORM_ARDUINO
		#error "Can't use Arduino SPI unless using platform ARDUINO (MAXTHERMOLIB_PLATFORM_ARDUINO)"
	#endif
#endif


#if defined(MAXTHERMOLIB_SPI_BITBANG) and defined(MAXTHERMOLIB_SPI_ARDUINO)
#error "Can't use multiple MAXTHERMOLIB_SPI_* methods--choose one!"
#endif

#endif /* MAXTHERMOLIB_CONFIG_H_ */
