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
    
    while(1)
    {
        TACHY_locker = UNLOCKED;
        new_pulses = TACHY_pulses;
        if(TACHY_locker == UNLOCKED)
            break;
        else
            Serial.write('.');
    }
    pulses = new_pulses - totalPulses;
    totalPulses = new_pulses;
    
    speed = (float)pulses / (float)TACHY_PULSE_PER_ROTATE;
    rate = ((speed - min) / (max - min)) * 100.0;
    
}

void TACHY::updateOutput()
{
    Serial.print(speed, TACHY_DIGITS_AFTER_DOT);
    Serial.write(' ');
    Serial.println(rate, TACHY_DIGITS_AFTER_DOT);
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
    min = TACHY_FACTORY_MIN;
    max = TACHY_FACTORY_MAX;
    milestone = millis() + TACHY_CYCLE_DURATION_MSEC;
    
    pinMode(isrPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(isrPin), TACHY_countRoutine, CHANGE);
}

void TACHY::sequencer()
{
    if(millis() >= milestone)
    {
        milestone = millis() + TACHY_CYCLE_DURATION_MSEC;
        compute();
        updateOutput();
    }
}

void TACHY::calibrateLow()
{
}

void TACHY::calibrateHi()
{
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
    Serial.print(F("new min is "));
    Serial.println(min, TACHY_DIGITS_AFTER_DOT);
}

void TACHY::setMax(float value)
{
    max = value;
    Serial.print(F("new max is "));
    Serial.println(max, TACHY_DIGITS_AFTER_DOT);
}

float TACHY::getSpeed()
{
    return speed;
}
