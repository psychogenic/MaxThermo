/*
 * ThermoLibSPI_Bitbang.cpp -- bit-banged SPI implementation,
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

#include "includes/ThermoLibConfig.h"

#ifdef MAXTHERMOLIB_SPI_BITBANG

#include "includes/ThermoLibSPI_Bitbang.h"
#include "includes/ThermoLibPlatform.h"

uint8_t _mode = 0;
unsigned int clock_delay_us = (F_CPU/(4000000L));
uint8_t use_msb = 1;
uint8_t _clock_pin = 0;
uint8_t _mosi_pin = 0;
uint8_t _miso_pin = 0;


void SPIBitbang_begin(uint8_t sck_pin, uint8_t miso_pin, uint8_t mosi_pin)
{
	_clock_pin = sck_pin;
	_miso_pin = miso_pin;
	_mosi_pin = mosi_pin;
	/*Serial.print("Setting clock pin as output: ");
	Serial.println(_clock_pin, DEC);
	*/
	PIN_SETUP_AS_OUTPUT(_clock_pin);
	/*
	Serial.print("Setting miso pin as input: ");
	Serial.println(_miso_pin, DEC);
	*/
	PIN_SETUP_AS_INPUT(_miso_pin);

	if (_mosi_pin)
		PIN_SETUP_AS_OUTPUT(_mosi_pin);
}

uint8_t SPIBitbang_transfer(uint8_t b)
{
	uint8_t retVal = 0;

	PIN_SET_LOW(_clock_pin);
	uint8_t curVal = 0;
#ifdef SPI_REQUIRES_MSBLSB_SELECTION
	if (use_msb)
	{
#endif
		for (int8_t i=7; i>=0; i--)
		{

			if (_mosi_pin)
				PIN_SET(_mosi_pin, ((b & (1 << i)) ? HIGH : LOW));
			PIN_SET_HIGH(_clock_pin);
			DELAY_uS(clock_delay_us);
			curVal = PIN_READ(_miso_pin) ? 1 : 0;
			retVal |= ( curVal << i );
			PIN_SET_LOW(_clock_pin);
			DELAY_uS(clock_delay_us);
		}

#ifdef SPI_REQUIRES_MSBLSB_SELECTION
	} else {
		for (uint8_t i=0; i<=7; i++)
		{

			if (_mosi_pin)
				PIN_SET(_mosi_pin, (b & (1 << i)) ? HIGH : LOW);
			PIN_SET_HIGH(_clock_pin);
			DELAY_uS(clock_delay_us);
			retVal |= ( (PIN_READ(_miso_pin) ? 1 : 0) << i );
			PIN_SET_LOW(_clock_pin);
			DELAY_uS(clock_delay_us);
		}
	}
#endif
	return retVal;

}


void SPIBitbang_setup(uint8_t mode, uint8_t div, uint8_t msblsb)
{
	_mode = mode; // ignoring this for now.
	uint32_t divisor = 4;
	switch (div)
	{
	case SPI_CLOCK_DIV2:
		divisor = 2;
		break;

	case SPI_CLOCK_DIV4:
		divisor = 4;
		break;

	case SPI_CLOCK_DIV8:
		divisor = 8;
		break;

	case SPI_CLOCK_DIV16:
		divisor = 16;
		break;

	case SPI_CLOCK_DIV32:
		divisor = 32;
		break;

	case SPI_CLOCK_DIV64:
		divisor = 64;
		break;

	case SPI_CLOCK_DIV128:
		divisor = 128;
		break;
	default:
		divisor = 4;
		break;
	}

	clock_delay_us = (F_CPU/(1000000L*divisor));


}


#endif /* MAXTHERMOLIB_SPI_BITBANG */
