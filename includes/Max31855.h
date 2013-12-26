/*
 * Max31855.h -- interface fom Max31855 device
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

#ifndef THERMO_TO_DIGITALCONV_MAX31855_LIB_H_
#define THERMO_TO_DIGITALCONV_MAX31855_LIB_H_


#include "ThermocoupleDevice.h"
#include "Max31855/ThermocoupleData.h"
#include "Max31855/InternalData.h"
#include "Max31855/SampledValues.h"



namespace MaxThermo
{

	class Max31855 : public ThermocoupleDevice {

	public:
		typedef M31855::SampledValues 	Sample;
		typedef SampleRawDataInit	SampleLog;

		Max31855(PinNumber nSelectPin, bool quickReads=false) :
			ThermocoupleDevice(nSelectPin, quickReads) {}

#ifdef MAXTHERMO_SUPPORTS_CUSTOM_SPI_PINS
		Max31855(PinNumber nSelectPin, PinNumber clockPin, PinNumber misoPin,
				bool quickReads=false, bool autoInitSPI=true) :
					ThermocoupleDevice(nSelectPin, clockPin, misoPin, quickReads, autoInitSPI)
		{}
#endif
		virtual ~Max31855() {}

		Status::Result updateThermoReading();
		Status::Result updateThermoAndInternalReading();
		virtual Status::Result update(); // update thermo if quickReads is on, both otherwise.


		virtual bool thermoTempIsNegative();
		int16_t thermoTempAsInt();
		double  thermoTemp();


		inline bool internalTempIsNegative() { return last_sample.data.internal.isNegative();}
		int8_t internalTempAsInt();
		float internalTemp();


		Sample getSample(SampleLog* sample_archive, uint16_t index);
		Sample * lastSample() { return &last_sample;}

	protected:
		void performRead(bool complete);
		Status::Result doUpdate(bool complete);

		Status::Result determineError();
		SampleSummary getSampleSummary(SampleLog* sample_container, uint16_t index);

	private:

		Sample last_sample;

	};


} /* namespace MaxThermo */


#endif /* THERMO_TO_DIGITALCONV_MAX31855_LIB_H_ */
