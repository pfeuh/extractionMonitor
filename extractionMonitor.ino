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


#include <EEPROM.h>
#define EEPROM_ADDR_MIN 0
#define EEPROM_ADDR_MAX (EEPROM_ADDR_MIN + sizeof(float))
#define EEPROM_ADDR_PULSE_PER_ROTATE (EEPROM_ADDR_MAX + sizeof(float))
#define EEPROM_ADDR_MEASUREMENT_WINDOW (EEPROM_ADDR_PULSE_PER_ROTATE + sizeof(float))
#define EEPROM_ADDR_DURATION_MIN (EEPROM_ADDR_MEASUREMENT_WINDOW + sizeof(float))
#define EEPROM_ADDR_DURATION_MAX (EEPROM_ADDR_DURATION_MIN + sizeof(float))
#define EEPROM_ADDR_LUMINOSITY (EEPROM_ADDR_DURATION_MAX + sizeof(float))
#define EEPROM_DATASIZE (EEPROM_ADDR_LUMINOSITY + sizeof(byte))

#include "tachy.h"
#define TACHY_PIN 3
TACHY tachy = TACHY(TACHY_PIN);

#include "k2000.h"
K2000 k2000 = K2000();
#define DURATION_MIN 500.0
#define DURATION_MAX 2000.0

#include <Adafruit_NeoPixel.h>
#define RGB_LEDS_PIN 4
#define NB_RGB_LEDS 8
Adafruit_NeoPixel rgbLeds = Adafruit_NeoPixel(NB_RGB_LEDS, RGB_LEDS_PIN, NEO_GBRW + NEO_KHZ800);

#include <arduinoMenu.h>
#include "menuData.h"
#include "sharedFunctions.h"
ARDUINO_MENU menu = ARDUINO_MENU();

const char PROGMEM title[] = "Extraction monitor v. " TACHY_VERSION;
const char PROGMEM timestamp[] = __DATE__ " " __TIME__;

char userCommand;
unsigned long int userValue;
unsigned long int milestone;

#define console Serial

// serial commands:
// '*' + [1:65535] set k2000 max duration
// '/' + [1:65535] set k2000 min duration
// '+' + [1:65535] set 100% value
// '-' + [1:65535] set 0% value
// 'p' + [1:65535] set pulses per rotate
// 'w' + [1:65]    set measurement window width in seconds
// 'i' + [1:255]   set led intensity (luminosity)
// 'l'             put current rate to low point 0%
// 'h'             put current rate to hi point 100%
// 'r'             read eeprom config
// 's'             store eeprom config
// '?'             view config

void print_P(const char* address)
{
    while(pgm_read_byte(address))
        console.write(pgm_read_byte(address++));
}

void println_P(const char* address)
{
    print_P(address);
    console.write('\n');
}

void printMaxCloggingDuration()
{
    console.print(F("max clogging set to "));
    console.print(k2000.getDurationMax());
    console.print(" msec\n");
}

void printMinCloggingDuration()
{
    console.print(F("min clogging set to "));
    console.print(k2000.getDurationMin());
    console.print(" msec\n");
}

void printLuminosity()
{
    console.print(F("luminosity set to "));
    console.print(k2000.getDurationMin());
    console.print("/255\n");
}

void printMax()
{
    console.print(F("100% set to "));
    console.print(tachy.getMax());
    console.print(" rpm\n");
}

void printMin()
{
    console.print(F("0% set to "));
    console.print(tachy.getMin());
    console.print(" rpm\n");
}

void printPulsePerRotate()
{
    console.print(F("pulses per rotate set to "));
    console.print(tachy.getPulsePerRotate());
    console.write('\n');
}

void printMeasurementWindow()
{
    console.print(F("measurement window set to "));
    console.print(tachy.getMeasurementWindow());
    console.print(F(" seconds\n"));
}

void printCalibrateLow()
{
    console.print(F("0% calibrated to "));
    console.print(tachy.getMin());
    console.print(" rpm\n");
}

void printCalibrateHi()
{
    console.print(F("100% calibrated to "));
    console.print(tachy.getMax());
    console.print(" rpm\n");
}

void printConfig()
{
    printMin();
    printMax();
    printPulsePerRotate();
    printMeasurementWindow();
    printMaxCloggingDuration();
    printMinCloggingDuration();
}

void ihmSequencer()
{
    if(millis() > milestone)
    {
        milestone += tachy.getMeasurementWindow() * 1000;
        //~ console.write('[');
        //~ console.print(tachy.getLowestValue());
        //~ console.write(':');
        //~ console.print(tachy.getHighestValue());
        //~ console.write(']');
        //~ console.write(' ');
        
        console.print(tachy.getRate());
        console.print("% ");
        console.print(tachy.getSpeed());
        console.print(" rpm\n");

        //~ console.print(tachy.getRate());
        //~ console.print(F("\n"));

        
    }
}

void setMaxCloggingDuration(float value)
{
    k2000.setDurationMax(value);
    printMaxCloggingDuration();
}

void setMinCloggingDuration(float value)
{
    k2000.setDurationMin(value);
    printMinCloggingDuration();
}

void setMax(float value)
{
    tachy.setMax(value);
    printMax();
}

void setMin(float value)
{
    tachy.setMin(value);
    printMin();
}

void setPulsePerRotate(float value)
{
    tachy.setPulsePerRotate(value);
    printPulsePerRotate();
}

void setMeasurementWindow(byte value)
{
    tachy.setMeasurementWindow(value);
    printMeasurementWindow();
}

void calibrateLow()
{
    tachy.calibrateLow();
    printCalibrateLow();
}

void calibrateHi()
{
    tachy.calibrateHi();
    printCalibrateHi();
}

void setLuminosity(byte value)
{
    k2000.setLuminosity(value);
    printLuminosity();
}

void readConfig()
{
    float min;
    float max;
    word ppr;
    byte measurementWindow;
    float dmin;
    float dmax;
    EEPROM.get(EEPROM_ADDR_MIN, min);
    EEPROM.get(EEPROM_ADDR_MAX, max);
    EEPROM.get(EEPROM_ADDR_PULSE_PER_ROTATE, ppr);
    EEPROM.get(EEPROM_ADDR_MEASUREMENT_WINDOW, measurementWindow);
    EEPROM.get(EEPROM_ADDR_DURATION_MIN, dmin);
    EEPROM.get(EEPROM_ADDR_DURATION_MAX, dmax);
    tachy.setMin(min);
    tachy.setMax(max);
    tachy.setPulsePerRotate(ppr);
    tachy.setMeasurementWindow(measurementWindow);
    k2000.setDurationMin(dmin);
    k2000.setDurationMax(dmax);
    //~ printConfig();
    console.print(F("Configuration read!\n"));
}

void storeConfig()
{
    EEPROM.put(EEPROM_ADDR_MIN, tachy.getMin());
    EEPROM.put(EEPROM_ADDR_MAX, tachy.getMax());
    EEPROM.put(EEPROM_ADDR_PULSE_PER_ROTATE, tachy.getPulsePerRotate());
    EEPROM.put(EEPROM_ADDR_MEASUREMENT_WINDOW, tachy.getMeasurementWindow());
    EEPROM.put(EEPROM_ADDR_DURATION_MIN, k2000.getDurationMin());
    EEPROM.put(EEPROM_ADDR_DURATION_MAX, k2000.getDurationMax());
    printConfig();
    console.print(F("Configuration stored\n"));
}

void executeCommand()
{
    switch(userCommand)
    {
        case '*':
            setMaxCloggingDuration(userValue);
            break;
        case '/':
            setMinCloggingDuration(userValue);
            break;
        case 'i':
            setLuminosity(userValue);
            break;
        case '+':
            setMax(userValue);
            break;
        case '-':
            setMin(userValue);
            break;
        case 'p':
            setPulsePerRotate(userValue);
            break;
        case 'w':
            setMeasurementWindow(userValue);
            break;
        case 'l':
            calibrateLow();
            break;
        case 'h':
            calibrateHi();
            break;
        case 'r':
            readConfig();
            break;
        case 's':
            storeConfig();
            break;
        case '?':
            printConfig();
            break;
        default:
            break;
    }
    userValue = 0;
    userCommand = '\0';
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
            case '*':
            case '/':
            case 'i':
            case '+':
            case '-':
            case 'p':
            case 'r':
            case 's':
            case 'w':
            case 'l':
            case 'h':
            case '?':
                userCommand = c;
                break;
            case '\n':
                executeCommand();
                break;
            default:
                break;
        }
    }
}

float getRate()
{
    return tachy.getRate();
}

void ledCommand(byte led_num, unsigned long int _color)
{
    rgbLeds.setPixelColor(led_num, _color);
    rgbLeds.show();
}

void editPulsesPerRotate(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        tachy.setPulsePerRotate(tachy.getPulsePerRotate() + 1.0);
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        tachy.setPulsePerRotate(tachy.getPulsePerRotate() - 1.0);
    menu.printVariable((int)tachy.getPulsePerRotate());
}

void editMeasurementWindow(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        tachy.setMeasurementWindow(tachy.getMeasurementWindow() + 1.0);
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        tachy.setMeasurementWindow(tachy.getMeasurementWindow() - 1.0);
    menu.printVariable(tachy.getMeasurementWindow());
}

void editMinSpeed(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        tachy.setMin(tachy.getMin() + 1.0);
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        tachy.setMin(tachy.getMin() - 1.0);
    menu.printVariable(tachy.getMin());
}

void editMaxSpeed(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        tachy.setMax(tachy.getMax() + 1.0);
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        tachy.setMax(tachy.getMax() - 1.0);
    menu.printVariable(tachy.getMax());
}

void setup()
{
    console.begin(9600);
    println_P(title);
    println_P(timestamp);

    tachy.begin();
    readConfig();

    pinMode(LED_BUILTIN, OUTPUT);
    
    rgbLeds.begin();
    
    k2000.begin(NB_RGB_LEDS);
    k2000.setLedCommand(&ledCommand);
    k2000.setGetRate(&getRate);
    
    menu.begin(MENU_BROWSER_NB_ENTRIES, MENU_DATA_tables);
}

void loop()
{
    tachy.sequencer();
    userSequencer();
    ihmSequencer();
    k2000.sequencer();
    menu.sequencer();
    
    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}

