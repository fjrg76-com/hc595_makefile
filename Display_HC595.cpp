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

#include <SPI.h>
#include "Display_HC595.hpp"

#define SEG_A  0x80
#define SEG_B  0x01
#define SEG_C  0x10
#define SEG_D  0x04
#define SEG_E  0x08
#define SEG_F  0x40
#define SEG_G  0x20
#define SEG_DP 0x02

static constexpr uint8_t numbers_table[10] =
{
   SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F, // 0
   SEG_B + SEG_C,
   SEG_A + SEG_B + SEG_G + SEG_E + SEG_D,
   SEG_A + SEG_B + SEG_G + SEG_C + SEG_D,
   SEG_F + SEG_G + SEG_B + SEG_C,
   SEG_A + SEG_F + SEG_G + SEG_C + SEG_D,
   SEG_A + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G,
   SEG_A + SEG_B + SEG_C,
   SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G,
   SEG_A + SEG_F + SEG_G + SEG_B + SEG_C + SEG_D, // 9
};

// for HEX numeric system. You can easily convert it to the full alphabet:
static constexpr uint8_t chars_table[6] =
{
   SEG_A + SEG_B + SEG_C + SEG_E + SEG_F + SEG_G, // A
   SEG_C + SEG_D + SEG_E + SEG_F + SEG_G,
   SEG_A + SEG_D + SEG_E + SEG_F,
   SEG_B + SEG_C + SEG_D + SEG_E + SEG_G,
   SEG_A + SEG_D + SEG_E + SEG_F + SEG_G,
   SEG_A + SEG_E + SEG_F + SEG_G,                 // F
};

// TODO: symbols table

Display_HC595::Display_HC595( const uint8_t* _cathodes, const uint8_t _num_digits, const uint8_t _latch_pin, const uint8_t* _memory_frame )
   : cathodes{_cathodes}
   , memory_frame{_memory_frame}
   , len{_num_digits}
   , latch_pin{_latch_pin}
{
   pinMode( this->latch_pin, OUTPUT );
   digitalWrite( this->latch_pin, LOW );

   for( uint8_t i = 0; i < this->len; ++i )
   {
      pinMode( this->cathodes[ i ], OUTPUT );
      digitalWrite( this->cathodes[ i ], LOW );

      this->memory_frame[i] = 0;
   }
}

void Display_HC595::update()
{
   static uint8_t current_digit = 0;

   for( uint8_t i = 0; i < this->len; ++i )
   {
      digitalWrite( this->cathodes[ i ], LOW );
   }

   digitalWrite( this->latch_pin, LOW );
    SPI.beginTransaction( SPISettings( 16000000, MSBFIRST, SPI_MODE0 ) );
    SPI.transfer( this->memory_frame[ current_digit ] );
    SPI.endTransaction();
   digitalWrite( this->latch_pin, HIGH );

   digitalWrite( this->cathodes[ current_digit ], HIGH );

   ++current_digit;
   if( current_digit == this->len ) current_digit = 0;
}

void Display_HC595::clear()
{
   for( uint8_t i = 0; i < this->len; ++i )
   {
      this->memory_frame[ i ] = 0;
   }

   this->current_digit = 0;
   this->cursor = 0;
}

void Display_HC595::setCursor( uint8_t pos )
{
   this->cursor = pos < this->len ? pos : 0;
}

uint8_t Display_HC595::encode( uint8_t car )
{
   uint8_t ret_val = 0;

   if( 0x30 <= car and car < 0x3A ) 
      ret_val = numbers_table[ car - 0x30 ];

   else if( 'A' <= car and car < 'G' ) 
      ret_val = chars_table[ car - 'A' ];

   else if( 'a' <= car and car < 'g' ) 
      ret_val = chars_table[ car - 'a' ];

   else
      ret_val = 0;

   return ret_val;
}

size_t Display_HC595::write( uint8_t car )
{
   // anything but a decimal point (DP):
   if( car != '.' )
   {
      if( this->cursor >= this->len ) this->cursor = 0;

      if( car == '\n' )
      {
         this->cursor = 0;
      }
      else
      {
         this->memory_frame[ this->cursor ] = encode( car );

         ++this->cursor;
      }
   }

   // this DP treatment is different from that of activating/deactivating it individually:
   else
   {
      if( this->cursor > 0 )
      {
         this->memory_frame[ this->cursor - 1 ] |= SEG_DP;
      }
      else
      {
         this->memory_frame[0] = SEG_DP;
         ++this->cursor;
      }
   }

   return 1;
}

// this DP treatment is different from that of when it's part of a number or string:
void Display_HC595::showDP( uint8_t pos )
{
   if( pos < this->len )
   {
      this->memory_frame[ pos ] |= SEG_DP;
   }
}

void Display_HC595::hideDP( uint8_t pos )
{
   if( pos < this->len )
   {
      this->memory_frame[ pos ] &= ~(SEG_DP);
   }
}
