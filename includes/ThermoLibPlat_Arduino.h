/*
 * ThermoLibPlat_Arduino.h -- Arduino-specifics,
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

#ifndef MAXLIBPLAT_SELECTED_ARDUINO_H_
#define MAXLIBPLAT_SELECTED_ARDUINO_H_

#include "ThermoLibConfig.h"

#ifdef MAXTHERMOLIB_PLATFORM_ARDUINO
#include <Arduino.h>


#define DELAY_MS(ms)		delay(ms)
#define DELAY_uS(us)		delayMicroseconds(us)


#define PIN_SETUP_AS_OUTPUT(pin)		pinMode(pin, OUTPUT)
#define PIN_SETUP_AS_INPUT(pin)			pinMode(pin, INPUT)


#define PIN_SET(pin, val)		digitalWrite((pin), (val))
#define PIN_SET_HIGH(pin)		digitalWrite(pin, HIGH)
#define PIN_SET_LOW(pin)		digitalWrite(pin, LOW)

#define PIN_READ(pin)			digitalRead((pin))


#define DEBUG_OUTPUT(msg)			Serial.print(msg)
#define DEBUG_OUTPUTN(msg, b)		Serial.print(msg, b)
#define DEBUG_OUTPUTLN(msg)			Serial.println(msg)
#define DEBUG_OUTPUTNLN(msg, b)		Serial.println(msg, b)



#endif /* MAXTHERMOLIB_PLATFORM_ARDUINO */

#endif /* MAXLIBPLAT_ARDUINO_H_ */
