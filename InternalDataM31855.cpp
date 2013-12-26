/*
 * InternalDataM31855.cpp -- Max31855 internal data implementation,
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

#include "includes/Max31855/InternalData.h"


namespace MaxThermo {
namespace M31855 {

InternalData::InternalData()
{

}
InternalData::InternalData(uint16_t raw)
{
	init(raw);

}

void InternalData::init(uint16_t raw)
{

	internal.is_negative = (raw & 0x8000) >> 15; // sign bit 1000 0000 0000 0000
	uint16_t t = (raw & 0x7F00); // 7 bits worth of temp - 0111 1111 0000 0000

	// un-twos-complement (if required) and shift down to real value
	// such that temp has an absolute value
	internal.abs_temp = internal.is_negative ?  ( ~(t) + 1) >> 8 : t >> 8;

	internal.fractional_bits = (raw & 0xF0) >> 4; // 1111 0000 -- 4 bits of precision here
	internal.vcc_short = (raw & 0x04) >> 2; // 0100 --  short to VCC flag
	internal.ground_short = (raw & 0x02) >> 1; // 0010 -- short to ground flag
	internal.open_circuit = (raw & 0x01); // 0001 -- open circuit flag

}
float InternalData::temp()
{

	// get the absolute temp with integer portion and fraction (1/16, cause there are 4 bits worth)
	float t = internal.abs_temp + ((1.0f/16.0f) * internal.fractional_bits);

	// make negative if required
	if (internal.is_negative)
		return -1.0f * t;

	return t;

}
int8_t InternalData::tempAsInt()
{

	// make negative if required
	if (internal.is_negative)
		return -1 * internal.abs_temp;

	return internal.abs_temp;

}

} /* namespace M31855 */
} /* namespace MaxThermo */
