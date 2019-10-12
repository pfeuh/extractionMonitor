/*
 * file : lasercutExtraction.ino
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

TACHY tachy = TACHY(3);

const char PROGMEM title[] = "lasercutExtraction v. " TACHY_VERSION;
const char PROGMEM timestamp[] = __DATE__ " " __TIME__;

char userCommand;
word userValue;

#define CONSOLE_OUT_DEV Serial

void print_P(const char* address)
{
    while(pgm_read_byte(address))
        CONSOLE_OUT_DEV.write(pgm_read_byte(address++));
}

void println_P(const char* address)
{
    print_P(address);
    CONSOLE_OUT_DEV.write('\n');
}

void executeCommand()
{
    switch(userCommand)
    {
        case '+':
            tachy.setMax((float) userValue);
            break;
        case '-':
            tachy.setMin((float) userValue);
            break;
        default:
            break;
    }
}

void userSequencer()
{
    char c;
    
    while(Serial.available())
    {
        c = Serial.read();
        switch(c)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                userValue = userValue * 10 + c - '0';
                break;
            case'+':
            case'-':
                userCommand = c;
                break;
            case '\n':
                executeCommand();
                break;
        }
    }
}

void setup()
{
    CONSOLE_OUT_DEV.begin(9600);
    println_P(title);
    println_P(timestamp);

    tachy.begin();
    tachy.setMin(2.0);
    tachy.setMax(20.0);

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    tachy.sequencer();
    userSequencer();
    
    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}

