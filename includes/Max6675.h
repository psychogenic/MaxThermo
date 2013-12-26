/*
 * Max6675.h -- interface fom Max6675 device
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
#ifndef THERMO_TO_DIGITALCONV_MAX6675_LIB_H_
#define THERMO_TO_DIGITALCONV_MAX6675_LIB_H_


#include "ThermocoupleDevice.h"
#include "Max6675/ThermocoupleData.h"
#include "Max6675/SampledValues.h"


namespace MaxThermo
{

	class Max6675 : public ThermocoupleDevice {

	public:
		typedef M6675::SampledValues 	Sample;
		typedef SampleRawDataInit		SampleLog;

		Max6675(PinNumber nSelectPin, bool quickReads=false) :
			ThermocoupleDevice(nSelectPin, quickReads)
		{}

#ifdef MAXTHERMO_SUPPORTS_CUSTOM_SPI_PINS
		Max6675(PinNumber nSelectPin, PinNumber clockPin, PinNumber misoPin,
				bool quickReads=false, bool autoInitSPI=true) :
					ThermocoupleDevice(nSelectPin, clockPin, misoPin, quickReads, autoInitSPI)
		{}
#endif
		virtual ~Max6675() {}


		virtual Status::Result update() ;


		virtual bool thermoTempIsNegative();
		int16_t thermoTempAsInt();
		double  thermoTemp();


		// internal data not available here
		inline bool internalTempIsNegative() { return false ;}
		int8_t internalTempAsInt() { return 0; }
		float internalTemp() { return 0;}



		Sample getSample(SampleRawDataInit * sample_container, uint16_t index);
		Sample * lastSample() { return &last_sample;}

	protected:
		void performRead();

		virtual SampleSummary getSampleSummary(SampleRawDataInit* sample_container, uint16_t index);

		Status::Result determineError();

	private:

		Sample last_sample;

	};


} /* namespace MaxThermo */

#endif /* THERMO_TO_DIGITALCONV_MAX6675_LIB_H_ */
