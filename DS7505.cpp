/*
 * DS7505.cpp
 *
 *  Created on: 2013-12-27
 *      Author: malcalypse
 */


#include "includes/ThermoLibConfig.h"

#ifdef MAXTHERMOLIB_ENABLE_I2C_SUPPORT
#include "includes/ThermoLibPlatform.h"

#include "includes/DS7505.h"


#define SET_CONFIG_REG(opt, val)		\
	(configRegister &= ( (~(DS7507_CONFIGREG_MASK_##opt) ) | ( (val) << DS7507_CONFIGREG_SHIFT_##opt)))


namespace MaxThermo {


DS7505::DS7505(uint8_t address_bits2, uint8_t address_bits1, uint8_t address_bits0,
		Resolution res) :
		ThermocoupleDevice(0, 0, 0, false, false),
		configRegister(0), queried_register_pointer(DS7505_REGISTER_TEMP), current_resolution(res)
{

	address_bits = ( (address_bits2 ? 1 << 2 : 0) | (address_bits1 ? 1 << 1 : 0) | (address_bits0 ? 1 : 0));
	refreshI2CAddress();
}

DS7505::DS7505(uint8_t full_address_bits, Resolution res) :
		ThermocoupleDevice(0, 0, 0, false, false),
		configRegister(0),
		address_bits(full_address_bits),  queried_register_pointer(DS7505_REGISTER_TEMP),  current_resolution(res)
{

	if (full_address_bits > DS7505_MAX_ADDRESS)
		address_bits = 0; // should warn, todo

	refreshI2CAddress();

}

void DS7505::begin(int8_t new_addr)
{
	if (new_addr >= 0 && new_addr <= DS7505_MAX_ADDRESS)
		address_bits = new_addr;

	refreshI2CAddress();

	// set the specified resolution
	SET_CONFIG_REG(RES, (uint8_t)current_resolution);
	updateConfigRegister();

}

void DS7505::updateConfigRegister()
{
	TRANSMIT_2BYTES_TO(i2caddress, DS7505_REGISTER_CONFIG, configRegister);

}
void DS7505::refreshI2CAddress()
{
	 // 1001 | bits
	i2caddress = 0x48 | address_bits;
}


Status::Result DS7505::update() {
	TRANSMIT_BYTE_TO(i2caddress, queried_register_pointer);

	REQUEST_BYTES_FROM(i2caddress, 2u);

	uint16_t raw_data = RECEIVE_BYTE() << 8;
	MaxThermo::D7505::SampledValues new_vals(raw_data);

	last_sample = new_vals;

	if (sample_log_array) {
		// logging
		if (sample_log_idx < sample_log_size) {

			DS7505::SampleLog * s = (DS7505::SampleLog *) (sample_log_array);
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

	return Status::OK;

}

bool DS7505::thermoTempIsNegative() {
	return last_sample.data.thermo.isNegative();
}

int16_t DS7505::thermoTempAsInt() {
	return last_sample.data.thermo.tempAsInt();
}

double DS7505::thermoTemp() {
	return last_sample.data.thermo.temp();
}

DS7505::Sample DS7505::getSample(SampleRawDataInit* sample_container,
		uint16_t index) {


	DS7505::SampleLog * s = (DS7505::SampleLog *)(sample_container);

	DS7505::Sample tmp(s[index]);
	return tmp;
}


SampleSummary DS7505::getSampleSummary(
		SampleRawDataInit* sample_container, uint16_t index) {

	SampleSummary sum;
	DS7505::Sample s = getSample(sample_container, index);
	sum.is_fault = false; // never any faults reported.
	sum.temperature = s.data.thermo.temp();

	return sum;
}


} /* namespace MaxThermo */


#endif /* MAXTHERMOLIB_ENABLE_I2C_SUPPORT */
