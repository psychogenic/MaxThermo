/*
 * TempMonitor.ino -- usage demonstration for the MaxThermo
 * library, specifically its logging facilities.
 *
 * Starts by logging 100 samples, and analyzing the results a bit.
 * Then proceeds to output temperature, similar to
 * the TempMonitor example.
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
// we also want to include the library!
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
 * Leave output_to_serial defined if you want
 * to see anything on the serial monitor.
 * Comment out if you just want to see how much
 * space the lib takes when compiled (functionality
 * used here would add about 3k to your executable).
 *
 */
// 
#define output_to_serial


#define delay_between_samples_output		500


// Our thermocouple-to-digital converter instance, as
// a global -- here we used the defined THERMO_DEVICE
// to be general, normally you'd probably just use
// MaxThermo::Max31855 myDevice(chip_select_pin, clock_pin, serial_in_pin);
// or somesuch.

MaxThermo::THERMO_DEVICE tdc(chip_select_pin, chip_clock_pin, chip_serial_in_pin);



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



void setup()
{
  SERIAL_BEGIN(serial_baud_rate);
  tdc.begin(); // calling begin will set everything up, including SPI
  tdc.setQuickReads(false); // we'll monitor everything we can
  
  SERIAL_PRINTLN("About to start monitoring...");
}

void loop()
{
  // tell the device to update (and log, if we're still doing that)

  if (tdc.update() == MaxThermo::Status::OK)
  {
    // all was well.
    
      // log is done, so now we output temp
      double temperature = tdc.thermoTemp();
      SERIAL_PRINT("Temp: ");
      SERIAL_PRINTN(temperature);
      SERIAL_PRINT(" (which is ");
      SERIAL_PRINTN(MAXTHERMO_C_TO_F(temperature));
      SERIAL_PRINTLN(" F)");
      float intTemp = tdc.internalTemp();
      if (intTemp > 0)
      {
        
        SERIAL_PRINT("Internal temp: ");
        SERIAL_PRINTN(intTemp);
        SERIAL_PRINTLN(" deg C");
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
  delay(delay_between_samples_output );

}