/* Copyright (C) 
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 *  2024 - fjrg76 at hotmail dot com
 */

#include <Arduino.h>

#include <SPI.h>
// must be declared here!

#define SYSTEM_TICK (5)
#define MILLIS_TO_TICKS( x ) ( (x) / (SYSTEM_TICK) )

enum eDisplay_HC595
{
   CATHODES  = 4,
   LATCH_PIN = 2,
   OE_PIN    = 3,
};

constexpr uint8_t cathodes_array[eDisplay_HC595::CATHODES] = {53, 49, 51, 47};
uint8_t memory_frame[eDisplay_HC595::CATHODES];

#include "Display_HC595.hpp"

int main(void)
{
   init();
   // Arduino stuff: don't modify it!

   SPI.begin();

   Display_HC595 display( cathodes_array, eDisplay_HC595::CATHODES, eDisplay_HC595::LATCH_PIN, memory_frame );

   // my diy HC595 display includes it, but its use is uncommon, so it's not part of the driver:
   pinMode( eDisplay_HC595::OE_PIN, OUTPUT );
   digitalWrite( eDisplay_HC595::OE_PIN, LOW );

   display.clear();

   display.print( "DCBA" );
   // we're printing a string
   
   display.showDP( 1 );
   // show the decimal point in digit 1

   uint16_t time_delay = MILLIS_TO_TICKS( 1000 );
   uint16_t cont = 15;

   bool first_time = true;

   while( 1 )
   {
      delay( SYSTEM_TICK );

      display.update();
      // update the display every 5 ms

      if( --time_delay == 0 )
      {
         time_delay = MILLIS_TO_TICKS( 1000 );

         if( not first_time )
         {
            display.clear();

            display.print( "0000" );
            // we're printing a string (for zero-padding)

            if( cont < 10 )        display.setCursor( 3 );
            else if( cont < 100 )  display.setCursor( 2 );
            else if( cont < 1000 ) display.setCursor( 1 );

            display.print( cont );
            // we're printing an integer

            display.showDP( 3 );
            // insert a decimal point in digit 3 just for fun

            --cont;
            if( cont == 0 )
            {
               cont = 15;
               first_time = true;
            }
         }
         else
         {
            display.clear();

            display.print( "A.BC.D" );
            // decimal point is part of a string, twice!

            first_time = false;
         }
      }
   }
}
