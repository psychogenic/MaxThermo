MaxThermo
=========

MaxThermo: Thermocouple/thermostat/thermometer to digital converter library for microcontrollers.

	Copyright (C) 2013 Pat Deegan, psychogenic.com
	http://flyingcarsandstuff.com/project/MaxThermo/


This library allows you easily play with various thermocouple-to-digital converter and other temperature-related chips.
Currently supported: Max31855 and Max6675 (thermocouple-to-digital) and DS7505 (thermometer/thermostat).

Usage
=====

	// in the simplest case, you simply create the device, ask it to fetch 
	// current data (through SPI, using update()) and access the temp().
	
	#include <MaxThermo.h>
	MaxThermo::Max31855 thermocouple(chip_select_pin, chip_clock_pin, chip_serial_in_pin);
	
	// at some point (in setup() with Arduino) you need to start it up
	tdc.begin();

	// from here on in, you can call update() and access the collected data.
	// See the included examples or the project home for info
	if (tdc.update() == MaxThermo::Status::OK)
	{
	
		OUTPUT("The temperature is currently:");
		OUTPUT(tdc.temp());
	}
	

See the project home for more details on API and use: http://flyingcarsandstuff.com/project/MaxThermo/ as well as the included examples.

Features
========
	* Support for multiple device types using the same API (Max31855, Max6675, DS7505)
	* Support for thermocouple/temperature data
	* Support internal data (where available, like on the Max31855), to access device temperature, specific fault descriptions (open circuit, short circuits, etc.)
	* Support for memory-efficient logging (see the TempLogger example)
	* Unified API is extended as required to support device-specific features (e.g. setThermostat() for the DS7505)

License
=======

Released unde the terms of the LGPL (v3)--see the LICENSE file for details.


