/*
 * ThermocoupleData.h -- header for Max6675 sampled values data
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
#ifndef MAXTHERMOLIB_MX6675_SAMPLEDVALUES_H_
#define MAXTHERMOLIB_MX6675_SAMPLEDVALUES_H_
#include "../SampledValues.h"

namespace MaxThermo {
namespace M6675 {


typedef struct InternalDummyDataStruct {

} InternalData;


typedef struct SampleRawDataStruct {
	MaxThermo::M6675::ThermocoupleData thermo;

	SampleRawDataInit init;

	SampleRawDataStruct() : thermo(0) {}

	SampleRawDataStruct(uint16_t tdata) :
		thermo(tdata), init(tdata)
	{

	}





} SampleRawData;

class SampledValues {
public:
	SampledValues() {}
	virtual ~SampledValues() {}

	SampledValues(SampleRawDataStruct & rawdata) : data(rawdata) {
		full_sample = true;
	}
	SampledValues(uint16_t tdata) : data(tdata), full_sample(true)
	{
		full_sample = true;
	}


	ThermocoupleData * thermo() { return &(data.thermo);}
	InternalData * internal() { return NULL;}

	MaxThermo::M6675::SampleRawData	data;
	bool full_sample;

};

} /* namespace M6675 */
} /* namespace MaxThermo */
#endif /* MAXTHERMOLIB_MX6675_SAMPLEDVALUES_H_ */
