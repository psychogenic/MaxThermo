/*
 * MaxThermo.h -- main include header,
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

#ifndef MAXTHERMO_MAINHEADER_H_
#define MAXTHERMO_MAINHEADER_H_

#define MAXTHERMO_C_TO_F(v)		( ((v) * 9.000/5.000) + 32.000)

#include "includes/ThermoLibConfig.h"
#include "includes/Max31855.h"
#include "includes/Max6675.h"

#if defined(DEBUG) and defined(MAXTHERMOLIB_PLATFORM_ARDUINO)
int freeRAM();
#endif

#if defined(CUSTOM_ATEXIT_CXX_METHOD) and ! defined(NO_ATEXIT)
extern "C" void atexit (void);
#endif

#endif /* MAXTHERMO_H_ */
