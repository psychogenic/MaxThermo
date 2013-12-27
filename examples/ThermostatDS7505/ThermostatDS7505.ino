/*
 * DS7505.ino -- usage demonstration for the MaxThermo
 * library, specifically DS7505 thermostat support.
 *
 * !!! NOTE !!! Because the DS7505 uses I2C, you MUST go into
 *              the library folder and edit
 *                     includes/ThermoLibConfig.h
 *              to uncomment the MAXTHERMOLIB_ENABLE_I2C define
 *              and set the flag to get access to DS7505 functions.
 *
 *
 *
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
 *
 * http://flyingcarsandstuff.com/projects/MaxThermo/
 *
 *
 * Please let me know if you use Thermocouple in your projects, and
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

// we must include Wire support here
#include <Wire.h>

// we want to include the library!
#include <MaxThermo.h>

#ifndef MAXTHERMOLIB_ENABLE_I2C_SUPPORT
// make sure you get a decent error message if you forgot...
#error	"You MUST edit includes/ThermoLibConfig.h and set the MAXTHERMOLIB_ENABLE_I2C_SUPPORT flag for DS7505 support"
#endif

/*
 * serial_baud_rate
 * Set this to the same baud rate used in the serial
 * monitor.
 */
#define serial_baud_rate	9600


/*
 * The DS7505 has a 3-bit address, configurable using the
 * A2, A1 and A0 pins (tied to Vcc or ground).  We assume
 * they're all tied to ground here, change values to 1 where
 * appropriate.
 */
#define address_bit_2			0
#define address_bit_1			0
#define address_bit_0			0


/*
 * We'll be logging a bunch of samples, decide how many
 * with num_samples_to_log.
 *
 * Note: these are in RAM, so don't overdo it--you'll
 * get weird results or access memory you shouldn't and
 * freeze the AVR.
 * At ~250, you're getting really close to the limit,
 * in this program...
 */
#define num_samples_to_log		100


/*
 * Leave output_to_serial defined if you want
 * to see anything on the serial monitor.
 * Comment out if you just want to see how much
 * space the lib takes when compiled (functionality
 * here would add about 4.5k to your executable).
 *
 */
#define output_to_serial


#define delay_between_samples_logging		30
#define delay_between_samples_output		800


// Our thermostat instance, configured with the address
// bits defined above, and set to 12 bit resolution (can select 9-12 bits)
MaxThermo::DS7505 thermostat(address_bit_2, address_bit_1, address_bit_0, MaxThermo::DS7505::Resolution_12bits);


// We'll be keeping a bunch of samples in a list, while logging
// so here's the array:

MaxThermo::DS7505::SampleLog the_samples[num_samples_to_log];



// defines to keep serial stuff switchable on/off
#ifdef output_to_serial
#define SERIAL_BEGIN(b)				Serial.begin(b)
#define SERIAL_PRINT(msg)			Serial.print(msg)
#define SERIAL_PRINTN(msg)			Serial.print(msg, DEC)
#define SERIAL_PRINTLN(msg)			Serial.println(msg)
#define SERIAL_PRINTNLN(msg)		Serial.println(msg, DEC)
#else
#define SERIAL_BEGIN(b)
#define SERIAL_PRINT(msg)
#define SERIAL_PRINTN(msg)
#define SERIAL_PRINTLN(msg)
#define SERIAL_PRINTNLN(msg)
#endif

/*
 * Once we're done logging, we'll output some info about the
 * results.
*/
void logSummary() {


  SERIAL_PRINTLN("\nDone!");

  // Just to demonstrate, we'll get one sample from the log array
  // and take a peak inside.

  // get the sample
  MaxThermo::DS7505::Sample aSample = thermostat.getSample(the_samples, 0);

  // output some info
  SERIAL_PRINT("The first sample ");
  if (aSample.thermo()->isFault())
  {
  	// was an error.  This never happens for the DS7505, but
	// demonstrates that the API is the same for all
	// MaxThermo devices
    SERIAL_PRINTLN("was a faulty reading.");
  } else {
  	// all is well, output temperature.
    SERIAL_PRINT("has temp:");
    SERIAL_PRINTNLN(aSample.thermo()->temp());
  }

  // show the internal temp, if we can
  if (aSample.internal())
  {
	// again, this is never true for DS7505 but the API
	// is the same in all instances, but for the construtor.
    SERIAL_PRINT(" with internal temp of ");
    SERIAL_PRINTNLN(aSample.internal()->temp());
  }

  // we could go through each of the samples in the array
  // like this, but for now we'll just get a summary using
  // the built-in method:

  MaxThermo::LogSummary summary = thermostat.logSummary(the_samples, num_samples_to_log);

  // output some info.
  SERIAL_PRINTLN("**********   Log Summary **********");
  SERIAL_PRINT("Out of "); SERIAL_PRINTN(summary.entries_total); SERIAL_PRINT(" entries ( ");
  SERIAL_PRINTN(summary.faults);
  SERIAL_PRINT((summary.faults == 1 ? " fault" : " faults")); // please the grammar police
  SERIAL_PRINTLN(" encountered, ");
  SERIAL_PRINTN(summary.entries_valid); SERIAL_PRINTLN(" valid):");
    SERIAL_PRINT("\tAverage temp:\t");
      SERIAL_PRINTNLN(summary.averageTemperature);
    SERIAL_PRINT("\tMinimum:\t");
      SERIAL_PRINTNLN(summary.minimumTemperature);
    SERIAL_PRINT("\tMaximum:\t");
      SERIAL_PRINTNLN(summary.maximumTemperature);

  SERIAL_PRINTLN("\nResuming regular probes...");

  // wait a bit so you can see it.
  delay(1000);





}

void setup()
{
  SERIAL_BEGIN(serial_baud_rate);
  thermostat.begin(); // calling begin will set everything up, including SPI

  // we tell the lib we want to send the sample from each
  // update() to this array of logging samples
  thermostat.startLoggingTo(the_samples, num_samples_to_log);

  SERIAL_PRINTLN("About to start logging...");
}

unsigned int dotCounter = 0;
void loop()
{
  // tell the device to update (and log, if we're still doing that)

  if (thermostat.update() == MaxThermo::Status::LogFull)
  {
    // we were logging, now we're done!
    thermostat.stopLogging(); // must acknowledge that we know its done
    logSummary();

  }
  else if (thermostat.status() == MaxThermo::Status::OK)
  {
    // all was well.
    if (thermostat.isLogging())
    {
      // we're still logging, don't clutter the Serial output,
      // just show we're alive

      SERIAL_PRINT(".");
      if (dotCounter++ > 39)
      {
        dotCounter = 0;
        SERIAL_PRINTLN(" ");
      }
    }
    else {
      // log is done, so now we output temp
      float temperature = thermostat.thermoTemp();
      SERIAL_PRINT("Temp: ");
      SERIAL_PRINTN(temperature);
      SERIAL_PRINT(" (which is ");
      SERIAL_PRINTN(MAXTHERMO_C_TO_F(temperature));
      SERIAL_PRINTLN(" F)");
    }

  }
  else {
    // update didn't say "OK" and it's not that the log
    // is full... lets see why.
    const char * err_type; // will hold appropiate error

    // get the last status
    switch(thermostat.status())
    {
      case MaxThermo::Status::ShortToVcc:
      err_type = "Short to VCC";
      break;

      case MaxThermo::Status::ShortToGround:
      err_type = "Short to GND";
      break;

      case MaxThermo::Status::OpenCircuit:
      err_type = "Open Circuit";
      break;

      case MaxThermo::Status::InconsistentResponses:
      err_type = "inconsistent thermo/internal data";
      break;
    default:
      err_type = "Unknown error?!";
      break;

    }
    // output reason for failure.
    SERIAL_PRINTLN(err_type);
  }

  // take a break, longer if we're finished logging:
  delay(thermostat.isLogging() ? delay_between_samples_logging : delay_between_samples_output );

}
