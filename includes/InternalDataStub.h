/*
 * InternalDataStub.h -- header for dummy internal data struct,
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
#ifndef MAXTHERMO_INTERNALDATA_DUMMY_H_
#define MAXTHERMO_INTERNALDATA_DUMMY_H_

#include "ThermoLibIncludes.h"


namespace MaxThermo
{


	typedef struct InternalDummyDataStruct {


		// temp() -- get the real temperature measurement
		 float temp() { return 0;}

		// tempAsInt() -- get the integer portion of temp measurement
		 int8_t tempAsInt() { return 0;}

		// various flag checks
		 bool isNegative() { return false;}
		 bool isOK() { return true;}
		 bool isFault() { return false;}
		 bool isShorted() { return false;}
		 bool isGroundShort() { return false; }
		 bool isVccShort() { return false;}
		 bool isOpenCircuit() { return false;}


	} InternalDataStub; // for use by classes that don't support internal data.






} /* namespace MaxThermo */

#endif /* MAXTHERMO_INTERNALDATA_DUMMY_H_ */
