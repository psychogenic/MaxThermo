/*
 * ThermocoupleDataM6675.cpp -- Max31855 thermo data implementation,
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

#include "includes/Max6675/ThermocoupleData.h"

namespace MaxThermo {
namespace M6675 {

ThermocoupleData::ThermocoupleData() {}

ThermocoupleData::ThermocoupleData(uint16_t raw) {
	init(raw);
}

void ThermocoupleData::init(uint16_t raw)
{

	uint16_t t = (raw & 0x7FE0); // 10 bits worth of data -- 0111 1111 1110 0000

	// un-twos-complement (if required) and shift down to real value
	// such that temp has an absolute value
	thermo.temp =  (t >> 4) ;

	thermo.fractional_bits = (raw & 0x18) >> 2; // the 2 bits after the "." -- 1 1000
	thermo.fault = (raw & 0x04); // the thermo fault flag. -- 100


}

double ThermocoupleData::temp() {

	// get the "full" value (int and fraction) and ensure it
	return thermo.temp + ((1.0f/4.0f) * thermo.fractional_bits);

}

int16_t ThermocoupleData::tempAsInt() {

	return thermo.temp;

}

} /* namespace M6675 */
} /* namespace MaxThermo */
