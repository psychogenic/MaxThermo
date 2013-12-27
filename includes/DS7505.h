/*
 * DS7505.h -- interface fom DS7505 device
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
#ifndef THERMO_TO_DIGITALCONV_DS7505_LIB_H_
#define THERMO_TO_DIGITALCONV_DS7505_LIB_H_


#include "ThermoLibConfig.h"

#ifdef MAXTHERMOLIB_ENABLE_I2C_SUPPORT

#include "ThermocoupleDevice.h"
#include "DS7505/ThermocoupleData.h"
#include "DS7505/SampledValues.h"

// 3-bit max address (0111 == 7)
#define DS7505_MAX_ADDRESS		7

#define DS7507_CONFIGREG_SHIFT_NVB			7
#define DS7507_CONFIGREG_SHIFT_RES			5
#define DS7507_CONFIGREG_SHIFT_FAULTTOL		3
#define DS7507_CONFIGREG_SHIFT_POLARITY		2
#define DS7507_CONFIGREG_SHIFT_OPMODE		1
#define DS7507_CONFIGREG_SHIFT_SHUTDOWN		0

#define DS7507_CONFIGREG_MASK_NVB		( 0x1 << DS7507_CONFIGREG_SHIFT_NVB)
#define DS7507_CONFIGREG_MASK_RES		( 0x3 << DS7507_CONFIGREG_SHIFT_RES)
#define DS7507_CONFIGREG_MASK_FAULTTOL	( 0x3 << DS7507_CONFIGREG_SHIFT_FAULTTOL)
#define DS7507_CONFIGREG_MASK_POLARITY	( 0x1 << DS7507_CONFIGREG_SHIFT_POLARITY)
#define DS7507_CONFIGREG_MASK_OPMODE	( 0x1 << DS7507_CONFIGREG_SHIFT_OPMODE)
#define DS7507_CONFIGREG_MASK_SHUTDOWN	( 0x1 << DS7507_CONFIGREG_SHIFT_SHUTDOWN)

#define DS7505_REGISTER_TEMP			0x0
#define DS7505_REGISTER_CONFIG			0x1
#define DS7505_REGISTER_HYSTERESIS		0x2
#define DS7505_REGISTER_TOS				0x3


namespace MaxThermo
{

	class DS7505 : public ThermocoupleDevice {

	public:
		typedef D7505::SampledValues 	Sample;
		typedef SampleRawDataInit		SampleLog;

		typedef enum resolutionEnum {
				Resolution_9bits  = 0x00,	// 00
				Resolution_10bits = 0x01,	// 01
				Resolution_11bits = 0x02, 	// 10
				Resolution_12bits = 0x03	// 11
		} Resolution;


		typedef enum faultTolEnum {
			TOS_1_Fault = 0x00, // 00
			TOS_2_Faults= 0x01, // 01
			TOS_4_Faults= 0x02, // 10
			TOS_6_Faults= 0x03, // 11
		} FaultTolerance;

		DS7505(uint8_t address2, uint8_t address1, uint8_t address0, Resolution res=Resolution_10bits);
		DS7505(uint8_t address_bits, Resolution res=Resolution_10bits);


		virtual ~DS7505() {}

		/* thermostat-specific */
		void setTOSTrigger(FaultTolerance tolerance);
		void setThermostat(float temperature, float hysteresis);


		/*
		 * begin([selectPin])
		 *
		 * Call begin() prior to using the device, so we can set everything up.
		 */
		virtual void begin(int8_t address = -1);

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

		virtual SampleSummary getSampleSummary(SampleRawDataInit* sample_container, uint16_t index);



	private:

		void refreshI2CAddress();
		void updateConfigRegister();
		uint16_t prepareTemperatureBits(float temp);

		Sample last_sample;
		uint8_t configRegister;
		uint8_t address_bits;
		uint8_t i2caddress;
		uint8_t queried_register_pointer;
		Resolution current_resolution;

	};


} /* namespace MaxThermo */


#endif /* MAXTHERMOLIB_ENABLE_I2C_SUPPORT */

#endif /* THERMO_TO_DIGITALCONV_DS7505_LIB_H_ */
