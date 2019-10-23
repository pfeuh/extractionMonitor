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

void K2000::begin(byte nb_leds)
{
    nbLeds = nb_leds;
    durationMin = K2000_FACTORY_DURATION_MIN;
    durationMax = K2000_FACTORY_DURATION_MAX;
    luminosity  = K2000_FACTORY_LUMINOSITY;
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
            milestone += duration;
            // k2000 is designed for this job...
            ledCommand(ledIndex, 0);
            ++ledIndex %= nbLeds;
            word temp_red = (luminosity * rate) / 100;
            word temp_green = luminosity - temp_red;
            word color = temp_red + (temp_green << 8);
            ledCommand(ledIndex, color);
        }
    }
}

void K2000::setDurationMin(float value)
{
    durationMin = value;
}

void K2000::setDurationMax(float value)
{
    durationMax = value;
}

float K2000::getDurationMin()
{
    return durationMin;
}

float K2000::getDurationMax()
{
    return durationMax;
}

void K2000::setLedCommand(void(*callback)(byte led_num, unsigned long int _color))
{
    ledCommand = callback;
}

void K2000::setGetRate(float(*callback)())
{
    getRate = callback;
}

void K2000::setLuminosity(byte value)
{
    luminosity = value;
}

byte K2000::getLuminosity()
{
    return luminosity;
}

