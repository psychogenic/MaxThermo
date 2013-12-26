/*
 * ThermocoupleDevice.h -- header for device base class,
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
#ifndef THERMOCOUPLEDEVICE_BASECLASS_H_
#define THERMOCOUPLEDEVICE_BASECLASS_H_

#include "ThermoLibIncludes.h"
#include "SampledValues.h"
#include "ThermoLibSPI.h"

namespace MaxThermo
{

	typedef uint8_t PinNumber;

	typedef struct logSummaryStruct {

		uint16_t entries_total;
		uint16_t entries_valid;
		uint16_t faults;
		double maximumTemperature;
		double minimumTemperature;
		double averageTemperature;

	} LogSummary;

	typedef struct sampleSummaryStruct {
		double temperature;
		bool is_fault;
	} SampleSummary;

	namespace Status {

		typedef enum StatusResEnum {
			NoReading=0,
			OK,
			NoResponse,
			InconsistentResponses,
			ShortToVcc,
			ShortToGround,
			OpenCircuit,
			LogFull,
			UnknownError
		} Result ;

	} /* namespace status */

	class ThermocoupleDevice {

	public:
		// init using standard SCK/MISO/MOSI pins
		ThermocoupleDevice(PinNumber nSelectPin, bool quickReads=false);
#ifdef MAXTHERMO_SUPPORTS_CUSTOM_SPI_PINS
		// init with custom SCK/MISO
		ThermocoupleDevice(PinNumber nSelectPin, PinNumber clockPin, PinNumber misoPin,
				bool quickReads=false, bool autoInitSPI=true);
#endif
		virtual ~ThermocoupleDevice() {}

		virtual void begin(int8_t selectPin=-1);

		bool autoInitSPI() { return auto_init_spi;}
		void setAutoInitSPI(bool setTo) { auto_init_spi = setTo;}

		bool quickReads() { return quick_reads;}
		void setQuickReads(bool setTo) { quick_reads = setTo;}


		virtual Status::Result update() = 0;

		bool isLogging() { return sample_log_array != NULL;  }
		Status::Result startLoggingTo(SampleRawDataInit * sample_container, uint16_t container_size);
		void stopLogging();
		LogSummary logSummary(SampleRawDataInit * sample_container, uint16_t container_size);

		Status::Result status() { return last_status;}
		bool isOK() { return last_status == Status::OK;}
		bool isFault() { return (last_status != Status::OK) && (last_status != Status::NoReading);}
		bool circuitOpen() { return last_status == Status::OpenCircuit;}
		bool circuitShort() { return (last_status == Status::ShortToGround) || (last_status == Status::ShortToVcc);}



		inline bool tempIsNegative() { return thermoTempIsNegative();}
		inline double temp() { return thermoTemp();}
		inline int16_t tempAsInt() { return thermoTempAsInt();}

		virtual bool thermoTempIsNegative() = 0;
		virtual int16_t thermoTempAsInt() = 0;
		virtual double  thermoTemp() = 0;



		virtual void initSPI();
	protected:

		virtual Status::Result determineError() = 0;

		virtual SampleSummary getSampleSummary(SampleRawDataInit* sample_container, uint16_t index) = 0;




		PinNumber nSelect_pin;
		PinNumber clock_pin;
		PinNumber miso_pin;

		bool auto_init_spi;
		bool quick_reads;
		Status::Result last_status;

		uint16_t sample_log_idx;
		uint16_t sample_log_size;
		SampleRawDataInit * sample_log_array;

	};


} /* namespace MaxThermo */

#endif /* THERMOCOUPLEDEVICE_H_ */
