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

#pragma once

#include <Arduino.h>

class Display_HC595 : public Print
{
private:
   const uint8_t* cathodes{nullptr}; // array for the cathodes, [d0, d1, d2, d3]
   uint8_t* memory_frame{nullptr};   // display abstraction

   uint8_t len{0};                   // number of digits in the display
   uint8_t current_digit{0};         // used when updating the display from the memory_frame
   uint8_t latch_pin{0};
   uint8_t cursor{0};                // used when writing into the memory_frame

   uint8_t encode( uint8_t car );

   using Print::write;
   virtual size_t write( uint8_t car ) override;

public:
   Display_HC595( const uint8_t* _cathodes, const uint8_t _num_digits, const uint8_t _latch_pin, const uint8_t* _memory_frame );
   Display_HC595(Display_HC595&) = delete;
   Display_HC595& operator=(Display_HC595&) = delete;

   void clear();
   void setCursor( uint8_t pos );
   void update();
   void showDP( uint8_t pos );
   void hideDP( uint8_t pos );
};

