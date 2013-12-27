/*
 * ThermocoupleDataD7505.cpp -- Max31855 thermo data implementation,
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

#include "includes/DS7505/ThermocoupleData.h"

namespace MaxThermo {
namespace D7505 {

ThermocoupleData::ThermocoupleData() {}

ThermocoupleData::ThermocoupleData(uint16_t raw) {
	init(raw);
}

void ThermocoupleData::init(uint16_t raw)
{



	thermo.is_negative = (raw & 0x8000) >> 15; // sign bit -- 1000 0000
	uint16_t t = (raw & 0x7F00); // 7 bits worth of data -- 0111 1111 0000 0000

	// un-twos-complement (if required) and shift down to real value
	// such that temp has an absolute value
	thermo.abs_temp = thermo.is_negative ? (( ~(t) + 1) >> 4) : (t >> 4) ;

	thermo.fractional_bits = (raw & 0xF0) >> 4; // the 4 bits after the "." -- 1111 0000

	// nothing else in here...


}

double ThermocoupleData::temp() {

	// get the "full" value (int and fraction) and ensure it
	float s = thermo.is_negative ? -1.0000f : 1.0000f;
	return s * (thermo.abs_temp + ((1.0f/16.0f) * thermo.fractional_bits));


}

int16_t ThermocoupleData::tempAsInt() {

	int8_t s = thermo.is_negative ? -1 : 1;
	return thermo.abs_temp * s;

}

} /* namespace D7505 */
} /* namespace MaxThermo */
