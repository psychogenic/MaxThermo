/*
 * ThermocoupleDevice.h -- header for device base class,
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

		/*
		 * begin([selectPin])
		 *
		 * Call begin() prior to using the device, so we can set everything up.
		 */
		virtual void begin(int8_t selectPin=-1);

		/*
		 * autoInitSPI()/setAutoInitSPI()
		 * Will initialize SPI on begin(), if true (default).
		 */
		bool autoInitSPI() { return auto_init_spi;}
		void setAutoInitSPI(bool setTo) { auto_init_spi = setTo;}

		/*
		 * quickReads()/setQuickReads()
		 * For devices that support it (like the Max31855) quickreads will
		 * only fetch the thermocouple-related data, and stop short of collecting
		 * extra bytes.
		 */
		bool quickReads() { return quick_reads;}
		void setQuickReads(bool setTo) { quick_reads = setTo;}


		/*
		 * update() performs an actual read of the thermocouple and caches the result.
		 */
		virtual Status::Result update() = 0;

		/*
		 * Logging related.
		 * When you startLoggingTo(), all update()s will be stored in the sample_container array.
		 *
		 * Call stopLogging() when you are done, or wait for update() to return a
		 * 	MaxThermo::Status::LogFull
		 * result (and then call stopLogging()/do whatever you wish with the sample list).
		 *
		 * See the TempLogger example.
		 */
		bool isLogging() { return sample_log_array != NULL;  }
		Status::Result startLoggingTo(SampleRawDataInit * sample_container, uint16_t container_size);
		/*
		 * pause the logging of update()s to the sample_container, leaving current state intact.
		 */
		bool pauseLogging();

		/*
		 * 	resumeLogging()
		 * 	Resume logging after calls to startLoggingTo() + pauseLogging()
		 */
		bool resumeLogging();

		/*
		 * stopLogging()
		 * Cease all logging activities, may be called anytime when logging but MUST be
		 * called once update()/status() starts returning LogFull status.
		 */
		uint16_t stopLogging();
		LogSummary logSummary(SampleRawDataInit * sample_container, uint16_t container_size);


		/*
		 * status() returns the cached MaxThermo::Status::Result from the last update().
		 */
		Status::Result status() { return last_status;}

		/*
		 * isOk()/isFault()/circuitOpen()/ciruitShort() -- convenience functions for checking the
		 * status().
		 */
		bool isOK() { return last_status == Status::OK;}
		bool isFault() { return (last_status != Status::OK) && (last_status != Status::NoReading);}
		bool circuitOpen() { return last_status == Status::OpenCircuit;}
		bool circuitShort() { return (last_status == Status::ShortToGround) || (last_status == Status::ShortToVcc);}


		/*
		 * tempXXX
		 * Shorthand methods for the thermocouple temperature data from the last update().
		 */

		inline bool tempIsNegative() { return thermoTempIsNegative();}
		inline double temp() { return thermoTemp();}
		inline int16_t tempAsInt() { return thermoTempAsInt();}

		/*
		 * thermoTempIsNegative()
		 * Return true if the temperature is subzero (celcius)
		 */
		virtual bool thermoTempIsNegative() = 0;

		/*
		 * thermoTempAsInt()
		 * return the thermocouple temperature from the last update(), but ignores the
		 * digits and is more efficient (if you don't care about the fractional value).
		 */
		virtual int16_t thermoTempAsInt() = 0;

		/*
		 * thermoTemp()
		 * Returns the (real) thermocouple temperature from the last update().
		 */
		virtual double  thermoTemp() = 0;

		/*
		 * internalTempXXX versions are also available in the implementation, but don't have
		 * the overhead of virtual methods.
		 */

		/*
		 * initSPI()
		 * Used internally to initialize the SPI, but available publically if you wanit.
		 */
		virtual void initSPI();
	protected:

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
		SampleRawDataInit * sample_log_swap;

	};



} /* namespace MaxThermo */

#endif /* THERMOCOUPLEDEVICE_H_ */
