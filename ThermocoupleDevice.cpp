/*
 * ThermocoupleDevice.cpp -- Device base class implementation,
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

#include "includes/ThermocoupleDevice.h"
#include "includes/ThermoLibPlatform.h"
#include "includes/ThermoLibSPI.h"

namespace MaxThermo {

ThermocoupleDevice::ThermocoupleDevice(
		PinNumber nSelectPin, bool quickReads) :
		nSelect_pin(nSelectPin),
		clock_pin(SCK),
		miso_pin(MISO),
		auto_init_spi(true),
		quick_reads(
				quickReads), last_status(
				Status::NoReading), sample_log_idx(0), sample_log_size(0),
				sample_log_array(
				NULL) {
}

#ifdef MAXTHERMO_SUPPORTS_CUSTOM_SPI_PINS
ThermocoupleDevice::ThermocoupleDevice(PinNumber nSelectPin, PinNumber clockPin, PinNumber misoPin,
				bool quickReads, bool autoInitSPI):
				nSelect_pin(nSelectPin),
				clock_pin(clockPin),
				miso_pin(misoPin),
				auto_init_spi(autoInitSPI),
				quick_reads(quickReads), last_status(
						Status::NoReading), sample_log_idx(0), sample_log_size(0),
						sample_log_array(
						NULL)
{

}
#endif

void ThermocoupleDevice::begin(int8_t selectPin) {
	static bool beginHasRun = false;

	if (beginHasRun)
	{
		Serial.println("WTF? begin again???");
		return;
	}

	beginHasRun = true;
	if (selectPin >= 0)
		nSelect_pin = selectPin;

	PIN_SETUP_AS_OUTPUT(nSelect_pin);
	PIN_SET_HIGH(nSelect_pin);

	if (auto_init_spi) {
		initSPI();
		auto_init_spi = false;
	}

}


void ThermocoupleDevice::initSPI() {

	PIN_SETUP_AS_OUTPUT(clock_pin);
	PIN_SETUP_AS_INPUT(miso_pin);

	SPI_SETUP(SPI_MODE0, SPI_CLOCK_DIV2, MSBFIRST);

	SPI_BEGIN(clock_pin, miso_pin, 0);


	DEBUG_OUTPUT("Calling spi begin with clock ");
	DEBUG_OUTPUTN(clock_pin, DEC);
	DEBUG_OUTPUT(" and miso ");
	DEBUG_OUTPUTNLN(miso_pin, DEC);

}



Status::Result ThermocoupleDevice::startLoggingTo(
		SampleRawDataInit * container, uint16_t container_size) {
	if (!container)
		return Status::LogFull;

	if (!container_size)
		return Status::LogFull;

	sample_log_array = container;
	sample_log_idx = 0;
	sample_log_size = container_size;

	return Status::OK;

}

void ThermocoupleDevice::stopLogging() {
	sample_log_array = NULL;
	sample_log_idx = 0;
	sample_log_size = 0;
}



LogSummary ThermocoupleDevice::logSummary(SampleRawDataInit * sample_container, uint16_t container_size)
{
	LogSummary sum = {0};

	sum.entries_total = container_size;
	double tempTotal = 0;
	double tempMin = 99000; // anything insanely high
	double tempMax = 0;
	double tmp;
	for (uint16_t i=0; i<container_size; i++)
	{
		MaxThermo::SampleSummary  s = this->getSampleSummary(sample_container, i);
		if (s.is_fault)
		{
			sum.faults++;
		} else {
			sum.entries_valid++;
			tmp = s.temperature;

			if (tmp < tempMin)
				tempMin = tmp;
			else if (tmp > tempMax)
				tempMax = tmp;

			tempTotal += tmp;
		}
	}

	if (sum.entries_valid)
	{
		sum.averageTemperature = (tempTotal/(1.0*sum.entries_valid));
		sum.maximumTemperature = tempMax;
		sum.minimumTemperature = tempMin;
	}

	return sum;
}

/* DEADBEEF
int16_t ThermocoupleDevice::thermoTempAsInt() {
	return this->lastSample()->thermo()->tempAsInt();

}

double ThermocoupleDevice::thermoTemp() {
	return this->lastSample()->thermo()->temp();
}
*/


} /* namespace MaxThermo */
