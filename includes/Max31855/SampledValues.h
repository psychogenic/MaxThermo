/*
 * SampledValues.h -- header for Max31855 sampled values
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
#ifndef MAXTHERMOLIB_MX31855_SAMPLEDVALUES_H_
#define MAXTHERMOLIB_MX31855_SAMPLEDVALUES_H_
#include "../SampledValues.h"

namespace MaxThermo {
namespace M31855 {

typedef struct SampleRawDataStruct {


	SampleRawDataStruct() : thermo(0) {}

	SampleRawDataStruct(uint16_t tdata) :
		thermo(tdata), full_sample(false), init(tdata << 16)
	{

	}

	SampleRawDataStruct(SampleRawDataInit combinedData) :
		thermo((combinedData & 0xFFFF0000) >> 16),
		internal( (combinedData & 0x0000FFFFul)),
		full_sample(true),
		init(combinedData)
	{

	}


	MaxThermo::M31855::ThermocoupleData thermo;
	MaxThermo::M31855::InternalData internal;

	bool full_sample;

	SampleRawDataInit init;

} SampleRawData;

class SampledValues { //  : public MaxThermo::SampledValues {
public:
	SampledValues() : // : MaxThermo::SampledValues(),
			data(0u) {}
	virtual ~SampledValues() {}

	SampledValues(uint16_t tdata) : // MaxThermo::SampledValues(),
			data(tdata) {}

	SampledValues(uint32_t combinedData) : // : MaxThermo::SampledValues(),
			data(combinedData) {

	}

	SampledValues(SampleRawData rd) : data(rd)
	{

	}
	ThermocoupleData * thermo() { return &(data.thermo);}
	InternalData * internal() { return data.full_sample ? &(data.internal): NULL;}


	SampleRawData data;
};

} /* namespace M31855 */
} /* namespace MaxThermo */
#endif /* MAXTHERMOLIB_MX31855_SAMPLEDVALUES_H_ */
