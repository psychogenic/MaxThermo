/*
 * Max6675.cpp -- implementation of main interface,
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

#include "includes/Max6675.h"

#include "includes/ThermoLibIncludes.h"


namespace MaxThermo {



void Max6675::performRead() {

	PIN_SET_LOW(nSelect_pin);
	// DELAY_MS(1);
	DELAY_uS(50);

	uint16_t readVal = ((uint8_t) SPI_TRANSFER(0x0)) << 8;
	readVal |= ((uint8_t) SPI_TRANSFER(0x0));

	PIN_SET_HIGH(nSelect_pin);

	MaxThermo::M6675::SampledValues new_vals(readVal);
	last_sample = new_vals;


}


SampleSummary Max6675::getSampleSummary(SampleRawDataInit* sample_container, uint16_t index)
{
	SampleSummary sum;
	Max6675::Sample s = getSample(sample_container, index);
	if (s.data.thermo.isFault())
		sum.is_fault = true;
	else {
		sum.is_fault = false;
		sum.temperature = s.data.thermo.temp();
	}
	return sum;
}
Max6675::Sample Max6675::getSample(SampleRawDataInit * sample_container, uint16_t index)
{
	Max6675::SampleLog * s = (Max6675::SampleLog *)(sample_container);
	M6675::SampledValues tmp(s[index]);

	return tmp;

}



Status::Result Max6675::update() {

	performRead();

	if (sample_log_array) {
		// logging
		if (sample_log_idx < sample_log_size) {


			Max6675::SampleLog * s = (Max6675::SampleLog *)(sample_log_array);
			(s[sample_log_idx++]) = last_sample.data.init;

		}

		// don't use an 'else' here, but
		// split this in two, so we return logfull
		// as soon as it is true.
		if (sample_log_idx >= sample_log_size) {
			last_status = Status::LogFull;
			return last_status;
		}

	}
	last_status =
			last_sample.data.thermo.isFault() ? this->determineError() : Status::OK;

	return last_status;

}

bool  Max6675::thermoTempIsNegative() { return last_sample.data.thermo.isNegative();}

int16_t  Max6675::thermoTempAsInt() {
	return last_sample.data.thermo.tempAsInt();

}

double Max6675::thermoTemp() {
	return last_sample.data.thermo.temp();
}




Status::Result  Max6675::determineError() {

	if (last_sample.data.thermo.isFault())
		return Status::OpenCircuit; // only error I know about

	return Status::InconsistentResponses;
}

}
