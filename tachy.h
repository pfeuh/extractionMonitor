#ifndef tachy_h
#define tachy_h

/*
 * file : tachy.h
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

#define TACHY_VERSION "1.00"

#define TACHY_FACTORY_MIN 0
#define TACHY_FACTORY_MAX 100
#define TACHY_FACTORY_PULSE_PER_ROTATE 2
#define TACHY_FACTORY_MEASUREMENT_WINDOW_SEC 1

#define TACHY_DIGITS_AFTER_DOT 2

//~ #define TACHY_CYCLE_DURATION_MSEC 1000

class TACHY
{
    public:
        TACHY(int isr_pin);
        void begin();
        void sequencer();
        void calibrateLow();
        void calibrateHi();
        float getMin();
        float getMax();
        void setMin(float value);
        void setMax(float value);
        float getSpeed();
        void setPulsePerRotate(word value);
        word getPulsePerRotate();
        float getLowestValue();
        float getHighestValue();
        float getClogging();
        void setMeasurementWindow(byte value);
        byte getMeasurementWindow();

    private:
        int isrPin;
        float speed;
        float min;
        float max;
        float lowestValue;
        float highestValue;
        word pulsePerRotate;
        float clogging;
        word pulses;
        unsigned long int totalPulses;
        unsigned long int milestone;
        byte measurementWindow;
    
        void compute();
    
};

#endif

