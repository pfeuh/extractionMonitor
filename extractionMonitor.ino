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
#define EEPROM_ADDR_DIRECTION (EEPROM_ADDR_LUMINOSITY + sizeof(byte))
#define EEPROM_ADDR_BAUDRATE (EEPROM_ADDR_DIRECTION + sizeof(byte))
#define EEPROM_ADDR_EXPORT_SWITCH (EEPROM_ADDR_BAUDRATE + sizeof(byte))
#define EEPROM_ADDR_CLOGGING_SWITCH (EEPROM_ADDR_EXPORT_SWITCH + sizeof(bool))
#define EEPROM_DATASIZE (EEPROM_ADDR_CLOGGING_SWITCH + sizeof(bool))

#define ERR_NOT_IMPLEMENTED_YET 1

#define dpeek pgm_read_dword

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
MENU_ENCODER encoder = MENU_ENCODER(5, 6, 7);
LiquidCrystal_I2C display(0x27, 20, 4);
ARDUINO_MENU menu = ARDUINO_MENU();

const char PROGMEM appVersion[] = "1.01";
const char PROGMEM compilationTimestamp[] = __DATE__ " " __TIME__;
const char PROGMEM unitSecond[] = " sec";
const char PROGMEM unitRpm[] = " tr/min";
const char PROGMEM unitPerCent[] = " %s";
const char PROGMEM unitMilliSec[] = " ms";
const char PROGMEM unitBaud[] = " baud";

const long serialBaudrate[] PROGMEM = 
{
    0,
    9600,
    19200,
    38400,
    57600,
    74880,
    115200
};
#define NB_BAUDRATES (sizeof(serialBaudrate) / sizeof(long))

byte serialBaudrateIndex = 1; // 9600 bauds
bool speedSwitch = true; 
bool cloggingSwitch = true; 

long unsigned int EXPORTER_milestone;
long unsigned int EXPORTER_runningState;

/*************/
/* callbacks */
/*************/

void ledCommand(byte led_num, unsigned long int _color)
{
    rgbLeds.setPixelColor(led_num, _color);
    rgbLeds.show();
}

float getRate()
{
    return tachy.getRate();
}

/************************/
/* Factorized functions */
/************************/

void printExportFlag(bool flag)
{
    if(flag)
        menu.printVariable(F("Exporter"));
    else
        menu.printVariable(F("Ne pas exporter"));
}

void printActiveFlag(bool flag)
{
    if(flag)
        menu.printVariable(F("Active"));
    else
        menu.printVariable(F("Inactive"));
}

/*****************************/
/* Functions for arduinoMenu */
/*****************************/

void editCurrentSpeed(byte direction)
{
    if(direction){};
    menu.printVariable(tachy.getSpeed());
    menu.print_P(unitRpm);
}

void editCurrentWindowWidth(byte direction)
{
    if(direction){};
    menu.printVariable(tachy.getMeasurementWindow());
    menu.print_P(unitSecond);
}

void editCurrentMin(byte direction)
{
    if(direction){};
    menu.printVariable(tachy.getMin());
    menu.print_P(unitRpm);
}

void editCurrentMax(byte direction)
{
    if(direction){};
    menu.printVariable(tachy.getMax());
    menu.print_P(unitRpm);
}

void editCurrentClogging(byte direction)
{
    if(direction){};
    menu.printVariable(tachy.getRate());
    menu.print_P(unitPerCent);
}

void editCurrentDurationMin(byte direction)
{
    if(direction){};
    menu.printVariable((unsigned long int)k2000.getDurationMin());
    menu.print_P(unitMilliSec);
}

void editCurrentDurationMax(byte direction)
{
    if(direction){};
    menu.printVariable((unsigned long int)k2000.getDurationMax());
    menu.print_P(unitMilliSec);
}

void editCurrentLuminosity(byte direction)
{
    if(direction){};
    menu.printVariable(k2000.getLuminosity());
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
    {
        if(tachy.getMeasurementWindow() != 255)
            tachy.setMeasurementWindow(tachy.getMeasurementWindow() + 1);
        else
            tachy.setMeasurementWindow(1);
    }
    else if(direction == MENU_BROWSER_DATA_DECREASE)
    {
        if(tachy.getMeasurementWindow() != 1)
            tachy.setMeasurementWindow(tachy.getMeasurementWindow() - 1);
        else
            tachy.setMeasurementWindow(255);
    }
    menu.printVariable(tachy.getMeasurementWindow());
    menu.print_P(unitSecond);
}

void editMinSpeed(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        tachy.setMin(tachy.getMin() + 1.0);
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        tachy.setMin(tachy.getMin() - 1.0);
    menu.printVariable(tachy.getMin());
    menu.print_P(unitRpm);
}

void editMaxSpeed(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        tachy.setMax(tachy.getMax() + 1.0);
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        tachy.setMax(tachy.getMax() - 1.0);
    menu.printVariable(tachy.getMax());
    menu.print_P(unitRpm);
}

void editDurationMin(byte direction)
{
    if(direction){};
    //~ if(direction == MENU_BROWSER_DATA_INCREASE)
        //~ durationMin +=1;
    //~ else if(direction == MENU_BROWSER_DATA_DECREASE)
        //~ durationMin -=1;
    //~ menu.printVariable(durationMin);
}

void editDurationMax(byte direction)
{
    if(direction){};
    //~ if(direction == MENU_BROWSER_DATA_INCREASE)
        //~ durationMax +=1;
    //~ else if(direction == MENU_BROWSER_DATA_DECREASE)
        //~ durationMax -=1;
    //~ menu.printVariable(durationMax);
}

void editLuminosity(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        k2000.setLuminosity(k2000.getLuminosity() + 4);
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        k2000.setLuminosity(k2000.getLuminosity() - 4);
    menu.printVariable(k2000.getLuminosity() / 4);
}

byte calibMin()
{
    tachy.setMin(tachy.getSpeed());
    return 0;
}

byte calibMax()
{
    tachy.setMax(tachy.getSpeed());
    return 0;
}

void editVersion(byte direction)
{
    if(direction){};
    menu.printVariable_P(appVersion);
}

void editTimestamp(byte direction)
{
    if(direction){};
    menu.printVariable_P(compilationTimestamp);
}

byte readConfig()
{
    float min;
    float max;
    word ppr;
    byte measurementWindow;
    float dmin;
    float dmax;
    bool direction;
    
    EEPROM.get(EEPROM_ADDR_MIN, min);
    EEPROM.get(EEPROM_ADDR_MAX, max);
    EEPROM.get(EEPROM_ADDR_PULSE_PER_ROTATE, ppr);
    EEPROM.get(EEPROM_ADDR_MEASUREMENT_WINDOW, measurementWindow);
    EEPROM.get(EEPROM_ADDR_DURATION_MIN, dmin);
    EEPROM.get(EEPROM_ADDR_DURATION_MAX, dmax);
    EEPROM.get(EEPROM_ADDR_DIRECTION, direction);
    EEPROM.get(EEPROM_ADDR_BAUDRATE, serialBaudrateIndex);
    if(serialBaudrateIndex >= NB_BAUDRATES)
        serialBaudrateIndex = 1; // 96000 bauds
    EEPROM.get(EEPROM_ADDR_EXPORT_SWITCH, speedSwitch);
    EEPROM.get(EEPROM_ADDR_CLOGGING_SWITCH, cloggingSwitch);
    
    tachy.setMin(min);
    tachy.setMax(max);
    tachy.setPulsePerRotate(ppr);
    tachy.setMeasurementWindow(measurementWindow);
    k2000.setDurationMin(dmin);
    k2000.setDurationMax(dmax);
    k2000.setDirection(direction);
    EXPORTER_setBaudRate();
    
    return 0;
}

byte storeConfig()
{
    EEPROM.put(EEPROM_ADDR_MIN, tachy.getMin());
    EEPROM.put(EEPROM_ADDR_MAX, tachy.getMax());
    EEPROM.put(EEPROM_ADDR_PULSE_PER_ROTATE, tachy.getPulsePerRotate());
    EEPROM.put(EEPROM_ADDR_MEASUREMENT_WINDOW, tachy.getMeasurementWindow());
    EEPROM.put(EEPROM_ADDR_DURATION_MIN, k2000.getDurationMin());
    EEPROM.put(EEPROM_ADDR_DURATION_MAX, k2000.getDurationMax());
    EEPROM.put(EEPROM_ADDR_DIRECTION, k2000.getDirection());
    EEPROM.put(EEPROM_ADDR_BAUDRATE, serialBaudrateIndex);
    EEPROM.put(EEPROM_ADDR_EXPORT_SWITCH, speedSwitch);
    EEPROM.put(EEPROM_ADDR_CLOGGING_SWITCH, cloggingSwitch);
    
    return 0;
}

byte factory()
{
    return ERR_NOT_IMPLEMENTED_YET;
}
void editSerialBaudrate(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
    {
        if(serialBaudrateIndex <(NB_BAUDRATES - 1))
            serialBaudrateIndex +=1;
    }
    else if(direction == MENU_BROWSER_DATA_DECREASE)
    {
        if(serialBaudrateIndex)
            serialBaudrateIndex -=1;
    }
    if (serialBaudrateIndex)
    {
        menu.printVariable(dpeek(serialBaudrate + serialBaudrateIndex));
        menu.print_P(unitBaud);
    }
    else
        printExportFlag(false);
    if(direction != MENU_BROWSER_DATA_JUST_DISPLAY)
        EXPORTER_setBaudRate();
}

void editSpeedSwitch(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        speedSwitch = true;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        speedSwitch = false;
    printExportFlag(speedSwitch);
}

void editCloggingSwitch(byte direction)
{
    if(direction == MENU_BROWSER_DATA_INCREASE)
        cloggingSwitch = true;
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        cloggingSwitch = false;
    printExportFlag(cloggingSwitch);
}

void editDirection(byte direction)
{
    // Warning! "direction" parameter is for increase/decrease
    if(direction == MENU_BROWSER_DATA_INCREASE)
        k2000.setDirection(true);
    else if(direction == MENU_BROWSER_DATA_DECREASE)
        k2000.setDirection(false);
    if(k2000.getDirection())
        menu.printVariable(F("sens horaire"));
    else
        menu.printVariable(F("sens anti-horaire"));
}

void editCurrentPulsePerTurn(byte direction)
{
    if(direction){};
    menu.printVariable((long)tachy.getPulsePerRotate());
}

/**********************/
/* exportation module */
/**********************/

void EXPORTER_setBaudRate()
{
    Serial.flush();
    switch(serialBaudrateIndex)
    {
        case 1: // 9600
            Serial.begin(9600);
            break;
        case 2: // 19200
            Serial.begin(19200);
            break;
        case 3: // 38400,
            Serial.begin(38400);
            break;
        case 4: // 57600
            Serial.begin(57600);
            break;
        case 5: // 74880
            Serial.begin(74880);
            break;
        case 6: // 115200
            Serial.begin(115200);
        default:
            break;
    }
}
void EXPORTER_sequencer()
{
    if(serialBaudrateIndex)
        if(millis() >= EXPORTER_milestone)
        {
            EXPORTER_milestone += tachy.getMeasurementWindow() * 1000;
            if(speedSwitch)
            {
                Serial.print(tachy.getSpeed());
                Serial.write('\t');
            }
            if(speedSwitch)
            {
                Serial.print(tachy.getRate());
                Serial.write('\t');
            }
            Serial.write('\n');
        }
}

/*******************************/
/* regular Arduino's functions */
/*******************************/

void setup()
{
    tachy.begin();
    readConfig();

    pinMode(LED_BUILTIN, OUTPUT);
    
    rgbLeds.begin();
    
    k2000.begin(NB_RGB_LEDS);
    k2000.setLedCommand(&ledCommand);
    k2000.setGetRate(&getRate);
    
    EXPORTER_setBaudRate();
    
    menu.begin(MENU_BROWSER_NB_ENTRIES, MENU_DATA_tables, &display, &encoder);
}

void loop()
{
    tachy.sequencer();
    k2000.sequencer();
    menu.sequencer();
    EXPORTER_sequencer();
    
    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}

