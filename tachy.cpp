/*
 * file : tachy.cpp
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

#include "tachy.h"

#define LOCKED true
#define UNLOCKED false

volatile unsigned long int TACHY_pulses;
volatile bool TACHY_locker;

void TACHY_countRoutine()
{
    TACHY_pulses++;
    TACHY_locker = LOCKED;
}

/*******************/
/* Private methods */
/*******************/

void TACHY::compute()
{
    unsigned long int new_pulses;

    // waiting lock
    while(1)
    {
        // as frequency is on several bytes, an interrupt can
        // corrupt it, we have to use a locker
        TACHY_locker = UNLOCKED;
        new_pulses = TACHY_pulses;
        if(TACHY_locker == UNLOCKED)
            // no interrupt since last lock,
            // frequency is OK
            break;
    }
    // computing pulses
    pulses = new_pulses - totalPulses;
    totalPulses = new_pulses;
    
    // computing speed
    speed = ((float)pulses  / ((float)pulsePerRotate * (float)measurementWindow)) * 60.0;
    
    // computing rate
    rate = ((speed - min) / (max - min)) * 100.0;
    if(rate > 100.0)
        rate = 100.0;
    else if(rate < 0.0)
        rate = 0.0;
    
    // computing min & max values
    if(lowestValue > speed)
        lowestValue = speed;
    else if(highestValue < speed)
        highestValue = speed;
}

/******************/
/* Public methods */
/******************/

TACHY::TACHY(int isr_pin)
{
    isrPin = isr_pin;
}

void TACHY::begin()
{
    TACHY_pulses = 0;
    totalPulses = 0;
    TACHY_locker = UNLOCKED;
    pulsePerRotate = TACHY_FACTORY_PULSE_PER_ROTATE;
    min = TACHY_FACTORY_MIN;
    max = TACHY_FACTORY_MAX;
    lowestValue = max;
    highestValue= min;
    milestone += getMeasurementWindow() * 1000;
    
    pinMode(isrPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(isrPin), TACHY_countRoutine, CHANGE);
}

void TACHY::sequencer()
{
    if(millis() >= milestone)
    {
        milestone += getMeasurementWindow() * 1000;
        compute();
    }
}

void TACHY::calibrateLow()
{
    min = speed;
}

void TACHY::calibrateHi()
{
    max = speed;
}

float TACHY::getMin()
{
    return min;
}

float TACHY::getMax()
{
    return max;
}

void TACHY::setMin(float value)
{
    min = value;
}

void TACHY::setMax(float value)
{
    max = value;
}

float TACHY::getSpeed()
{
    return speed;
}

void TACHY::setPulsePerRotate(word value)
{
    pulsePerRotate = value;
}

word TACHY::getPulsePerRotate()
{
    return pulsePerRotate;
}

float TACHY::getLowestValue()
{
    return lowestValue;
}

float TACHY::getHighestValue()
{
    return highestValue;
}

float TACHY::getRate()
{
    return rate;
}

void TACHY::setMeasurementWindow(byte value)
{
    measurementWindow = value;
}

byte TACHY::getMeasurementWindow()
{
    return measurementWindow;
}


