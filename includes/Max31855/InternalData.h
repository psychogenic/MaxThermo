/*
 * InternalData.h -- header for Max31855 internal data
 * recipient, part of MaxThermo Thermocouple-to-digital
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

#ifndef MAXTHERMOLIB_M31855_INTERNALDATA_H_
#define MAXTHERMOLIB_M31855_INTERNALDATA_H_

#include "../ThermoLibIncludes.h"

namespace MaxThermo {
namespace M31855 {


// using a bitfields means we have access to all our fields
// but only need 2 bytes (16 bits), rather than using a
// regular struct to do so, which would require 7 bytes.
// This way, you can make large arrays of internal data without
// eating all your RAM.
typedef  struct {
		unsigned is_negative :1;
		unsigned abs_temp : 7;
		unsigned fractional_bits :4;
		unsigned reserved :1;
		unsigned vcc_short :1;
		unsigned ground_short :1;
		unsigned open_circuit :1;

} InternalRawData;

class InternalData {
public:
	InternalData();
	InternalData(uint16_t raw); // construct with raw data from device
	 ~InternalData() {}

	// temp() -- get the real temperature measurement
	 float temp();

	// tempAsInt() -- get the integer portion of temp measurement
	 int8_t tempAsInt();

	// various flag checks
	 bool isNegative() { return internal.is_negative;}
	 bool isOK() { return ! isFault();}
	 bool isFault() { return internal.open_circuit || internal.ground_short || internal.vcc_short;}
	 bool isShorted() { return internal.ground_short || internal.vcc_short;}
	 bool isGroundShort() { return internal.ground_short;}
	 bool isVccShort() { return internal.vcc_short;}
	 bool isOpenCircuit() { return internal.open_circuit;}


	// assign with raw data from device
	 InternalData & operator=(uint16_t val) { init(val); return *this;}

private:


	void init(uint16_t raw);

	InternalRawData internal;
};

} /* namespace M31855 */
} /* namespace MaxThermo */
#endif /* INTERNALDATA_H_ */
