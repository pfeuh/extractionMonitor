/*
 * file : k2000.cpp
 * Copyright (c) pfeuh <ze.pfeuh@gmail>
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "k2000.h"

/*******************/
/* Private methods */
/*******************/

/******************/
/* Public methods */
/******************/

K2000::K2000()
{
}

void K2000::begin(byte nb_leds, float duration_min, float duration_max)
{
    nbLeds = nb_leds;
    durationMin = duration_min;
    durationMax = duration_max;
    ledIndex = nbLeds - 1;
    milestone = millis();
    ledCommand = NULL;
    getRate = NULL;
}

void K2000::sequencer()
{
    if(getRate && ledCommand)
    {
        if(milestone <= millis())
        {
            rate = getRate();
            duration = ((durationMax - durationMin) * rate) / 100 + durationMin;
            //~ Serial.print(F("         "));
            //~ Serial.println(duration);
            milestone += duration;
            ledCommand(ledIndex, 0);
            ++ledIndex %= nbLeds;
            word temp_red = (K2000_LUMINOSITY * rate) / 100;
            word temp_green = K2000_LUMINOSITY - temp_red;
            word color = temp_red + (temp_green << 8);
            ledCommand(ledIndex, color);
        }
    }
}

void K2000::setLedCommand(void(*callback)(byte led_num, unsigned long int _color))
{
    ledCommand = callback;
}

void K2000::setGetRate(float(*callback)())
{
    getRate = callback;
}
