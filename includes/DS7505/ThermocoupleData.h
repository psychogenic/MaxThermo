/*
 * ThermocoupleData.h -- header for Max6675 thermo data
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
#ifndef MAXTHERMOLIB_D7505_THERMOCOUPLEDATA_H_
#define MAXTHERMOLIB_D7505_THERMOCOUPLEDATA_H_


#include "../ThermoLibIncludes.h"

namespace MaxThermo {
namespace D7505 {


// using a union means we have access to all our fields
// but only need 2 bytes (16 bits), rather than using a struct
// to do so, which would require (at least) 3 bytes.
// This way, you can make large arrays of TDC data without
// eating all your RAM.
typedef struct {
		unsigned is_negative :1;
		unsigned abs_temp: 7;
		unsigned fractional_bits: 4;
} ThermocoupleRawData;


class ThermocoupleData
{
public:
	ThermocoupleData();
	ThermocoupleData(uint16_t raw); // construct with raw data from device
	 ~ThermocoupleData() {}

	// temp() - get real measured temp
	double temp();
	// tempAsInt() - get measured temp as integer
	int16_t tempAsInt();



	// a few flag checks
	 bool isNegative() { return thermo.is_negative; }
	 bool isOK() { return true;} // no faults reported
	 bool isFault() { return false;} // no faults reported




	// assign with raw data from device
	  ThermocoupleData & operator=(uint16_t val) { init(val); return *this;}

private:

	void init(uint16_t raw);

	MaxThermo::D7505::ThermocoupleRawData thermo;
};

} /* namespace M6675 */
} /* namespace MaxThermo */
#endif /* MAXTHERMOLIB_D7505_THERMOCOUPLEDATA_H_ */
