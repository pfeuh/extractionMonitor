#ifndef k2000_h
#define k2000_h

/*
 * file : k2000.h
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

#include <Arduino.h>

#define K2000_VERSION "1.00"

#define K2000_FACTORY_LUMINOSITY 20
#define K2000_FACTORY_DURATION_MIN 50.0
#define K2000_FACTORY_DURATION_MAX 1000.0


class K2000
{
    public:
        K2000();
        void begin(byte nb_leds);
        void sequencer();
        void setDurationMin(float value);
        void setDurationMax(float value);
        float getDurationMin();
        float getDurationMax();
        void setLedCommand(void(*callback)(byte led_num, unsigned long int _color));
        void setGetRate(float(*callback)());
        void setLuminosity(byte value);
        byte getLuminosity();

    private:
        void(*ledCommand)(byte led_num, unsigned long int _color);
        float (*getRate)();
        float rate;
        float duration;
        float durationMin;
        float durationMax;
        byte luminosity;

        unsigned long int milestone;
        byte nbLeds;
        byte ledIndex;
        
};

#endif

