/*
 * MAX31855.cpp -- implementation of main interface,
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
#include "includes/Max31855.h"
#include "includes/ThermoLibIncludes.h"




#ifdef DEBUG
#define DUMP_BYTE(b)	dumpByte(b)
#else
#define DUMP_BYTE(b)
#endif

#ifndef NULL
#define NULL	0x0
#endif

namespace MaxThermo {


#ifdef DEBUG
void dumpByte(uint8_t * b)
{
	for (int8_t i=7; i>=0; i--)
	{
		if ((*b) & (1 << i))
		{
			DEBUG_OUTPUTN(1, DEC);
		} else {
			DEBUG_OUTPUTN(0, DEC);
		}
		DEBUG_OUTPUT(' ');
	}
}
#endif

void Max31855::performRead(bool complete) {

	PIN_SET_LOW(nSelect_pin);
	DELAY_uS(50);
	// DELAY_MS(1);

	uint32_t readVal = 0;
	uint8_t numBytes = complete ? 4 : 2;
	uint8_t aByte;
	for (uint8_t i = 0; i < numBytes; i++) {
		aByte = (uint8_t) SPI_TRANSFER(0x0);

#ifdef DEBUG
		DEBUG_OUTPUT("<= ");
		DEBUG_OUTPUTNLN(aByte, HEX);
		DUMP_BYTE(&aByte);
		DEBUG_OUTPUTLN(" ");
#endif
		readVal <<= 8;
		readVal |= aByte;
	}



	PIN_SET_HIGH(nSelect_pin);

	if (complete) {
		MaxThermo::M31855::SampledValues new_vals(readVal);
		last_sample = new_vals;
	} else {
		// was a quick read (only the first 16 bits)
		MaxThermo::M31855::SampledValues new_vals((uint16_t) readVal);
		last_sample = new_vals;
	}

}

SampleSummary Max31855::getSampleSummary(SampleLog* sample_container, uint16_t index)
{
	SampleSummary sum;
	Max31855::Sample s = getSample(sample_container, index);
	if (s.data.thermo.isFault())
		sum.is_fault = true;
	else {
		sum.is_fault = false;
		sum.temperature = s.data.thermo.temp();
	}

	return sum;
}
Max31855::Sample Max31855::getSample(SampleLog* sample_container, uint16_t index)
{

	Max31855::SampleLog * s = (Max31855::SampleLog *)(sample_container);

	M31855::SampledValues tmp(s[index]);
	return tmp;
}
Status::Result Max31855::doUpdate(bool complete) {
	performRead(complete);

	if (sample_log_array) {
		// logging
		if (sample_log_idx < sample_log_size) {

			Max31855::SampleLog * s = (Max31855::SampleLog *)(sample_log_array);
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

Status::Result Max31855::updateThermoReading() {

	// do a partial update
	return doUpdate(false);
}
Status::Result Max31855::updateThermoAndInternalReading() {
	// do a full update to get both thermo and internal data
	return doUpdate(true);

}

Status::Result Max31855::update() {

	// do an update according to quick_reads setting (quick == only thermo)
	return quick_reads ?
			updateThermoReading() : updateThermoAndInternalReading();
}

bool Max31855::thermoTempIsNegative() { return last_sample.data.thermo.isNegative();}

int16_t Max31855::thermoTempAsInt() {
	return last_sample.data.thermo.tempAsInt();

}

double Max31855::thermoTemp() {
	return last_sample.data.thermo.temp();
}

int8_t Max31855::internalTempAsInt() {
	if (!last_sample.data.full_sample)
		performRead(true);

	return last_sample.data.internal.tempAsInt();
}

float Max31855::internalTemp() {

	if (!last_sample.data.full_sample)
		performRead(true);

	return last_sample.data.internal.temp();

}

Status::Result Max31855::determineError() {
	if (!last_sample.data.full_sample)
		performRead(true);

	if (last_sample.data.internal.isOpenCircuit())
		return Status::OpenCircuit;

	if (last_sample.data.internal.isGroundShort())
		return Status::ShortToGround;

	if (last_sample.data.internal.isVccShort())
		return Status::ShortToVcc;

	if (last_sample.data.thermo.isFault()) {
		// fault is set but we see nothing?
		// DEBUG_OUTPUTLN("Fault set but dunno why...");
		return Status::UnknownError;
	}

	return Status::InconsistentResponses;
}

}
