/*
 * TempLogger.ino -- usage demonstration for the MaxThermo
 * library, specifically its logging facilities.
 *
 * Starts by logging 100 samples, and analyzing the results a bit.
 * Then proceeds to output temperature, similar to
 * the TempMonitor example.
 *
 * Copyright (C) 2013 Pat Deegan.  All rights reserved.
 *
 * http://www.flyingcarsandstuff.com/projects/MaxThermo/
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
// we want to include the library!
#include <MaxThermo.h>


/*
 * serial_baud_rate
 * Set this to the same baud rate used in the serial
 * monitor.
 */
#define serial_baud_rate	9600


/*
 * chip_select_pin
 * Set this to the pin which is connected to the
 * chip select (CS) pin on the device.
 *
 */
#define chip_select_pin		5

/*
 * chip_clock_pin
 * Set this to the pin which is connected to the
 * spi clocking pin on the device.
 *
 */
#define chip_clock_pin          10


/*
 * chip_serial_in_pin
 * Set this to the pin which is connected to the
 * data (SO) pin on the device
 *
 */
#define chip_serial_in_pin      9

/*
 * thermo_device
 * Set this to the type of thermocouple-to-digital
 * converter you have.
 *
 * Currently supported:
 * 	Max31855
 * 	Max6675
 *
 * Note: no internal temperature available for Max6675
 */
#define THERMO_DEVICE		Max31855


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
#define num_samples_to_log		180


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


// Our thermocouple-to-digital converter instance, as
// a global -- here we used the defined THERMO_DEVICE
// to be general, normally you'd probably just use
// MaxThermo::Max31855 myDevice(chip_select_pin, clock_pin, serial_in_pin);
// or somesuch.

MaxThermo::THERMO_DEVICE tdc(chip_select_pin, chip_clock_pin, chip_serial_in_pin);


// We'll be keeping a bunch of samples in a list, while logging
// so here's the array:

MaxThermo::THERMO_DEVICE::SampleLog the_samples[num_samples_to_log];



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
  MaxThermo::THERMO_DEVICE::Sample aSample = tdc.getSample(the_samples, 0);
  
  // output some info
  SERIAL_PRINT("The first sample ");
  if (aSample.thermo()->isFault())
  {
  	// was an error.  If the device supports internal data reads, 
  	// you could check aSample.internal() for specifics but this 
  	// requires device-specific code, e.g. aSample.internal()->isGroundShort()
  	// only works for Max31855
    SERIAL_PRINTLN("was a faulty reading.");
  } else {
  	// all is well, output temperature.
    SERIAL_PRINT("has temp:");
    SERIAL_PRINTNLN(aSample.thermo()->temp());
  }
  
  // show the internal temp, if we can
  if (aSample.internal())
  {
    // the device supports readings of internal data
    SERIAL_PRINT(" with internal temp of ");
    SERIAL_PRINTNLN(aSample.internal()->temp());
  }
  
  // we could go through each of the samples in the array
  // like this, but for now we'll just get a summary using 
  // the built-in method:
  
  MaxThermo::LogSummary summary = tdc.logSummary(the_samples, num_samples_to_log);

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
  tdc.begin(); // calling begin will set everything up, including SPI
  tdc.setQuickReads(false); // we'll log everything we can, including internal data if available
  
  // we tell the lib we want to send the sample from each 
  // update() to this array of logging samples
  tdc.startLoggingTo(the_samples, num_samples_to_log);
  
  SERIAL_PRINTLN("About to start logging...");
}

unsigned int dotCounter = 0;
void loop()
{
  // tell the device to update (and log, if we're still doing that)

  if (tdc.update() == MaxThermo::Status::LogFull)
  {
    // we were logging, now we're done!
    tdc.stopLogging(); // must acknowledge that we know its done
    logSummary();

  } 
  else if (tdc.status() == MaxThermo::Status::OK)
  {
    // all was well.
    if (tdc.isLogging())
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
      float temperature = tdc.thermoTemp();
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
    switch(tdc.status())
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
  delay(tdc.isLogging() ? delay_between_samples_logging : delay_between_samples_output );

}