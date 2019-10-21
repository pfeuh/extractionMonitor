#ifndef menuData_h
#define menuData_h

/*
 * file : menuData.h
 * Copyright (c) pfeuh <ze.pfeuh@gmail.com>
 * creation date : 2019/10/20 23:57:01
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

/********************************************/
/*                 WARNING!                 */
/*    This file is generated by yaamm.py,   */
/*  if you edit it, it will be overwritten  */
/*        by the next generation!           */
/********************************************/

// Table of %s root's title     00021
// Table of parent              00006
// Table of child               00006
// Table of next                00006
// Table of previous            00006
// Table of labels              00068
// Table of pointers to labels  00012
// Table of function callbacks  00012
// Table of edit callbacks      00012
// Table of types               00006
// ----------------------------------
// TOTAL                        00155

#include <Arduino.h>
#include <menuBrowser.h>

#define MENU_BROWSER_NB_ENTRIES 6
#define MENU_BROWSER_NB_VARIABLES 4
#define MENU_BROWSER_NB_FUNCTIONS 0

const char MENU_BROWSER_ROOT_LABEL[21] PROGMEM = "EXTRACTION's MONITOR";

// functions

// edit/display variables' functions
extern void editPulsesPerRotate(byte direction);
extern void editMeasurementWindow(byte direction);
extern void editMinSpeed(byte direction);
extern void editMaxSpeed(byte direction);

const byte MENU_DATA_parentTable[MENU_BROWSER_NB_ENTRIES] PROGMEM = 
{
   /* 000 */ MENU_BROWSER_NO_ENTRY,
   /* 001 */   0,
   /* 002 */   1,
   /* 003 */   1,
   /* 004 */   1,
   /* 005 */   1,
};

const byte MENU_DATA_childTable[MENU_BROWSER_NB_ENTRIES] PROGMEM = 
{
   /* 000 */   1,
   /* 001 */   2,
   /* 002 */ MENU_BROWSER_NO_ENTRY,
   /* 003 */ MENU_BROWSER_NO_ENTRY,
   /* 004 */ MENU_BROWSER_NO_ENTRY,
   /* 005 */ MENU_BROWSER_NO_ENTRY,
};

const byte MENU_DATA_nextTable[MENU_BROWSER_NB_ENTRIES] PROGMEM = 
{
   /* 000 */ MENU_BROWSER_NO_ENTRY,
   /* 001 */ MENU_BROWSER_NO_ENTRY,
   /* 002 */   3,
   /* 003 */   4,
   /* 004 */   5,
   /* 005 */ MENU_BROWSER_NO_ENTRY,
};

const byte MENU_DATA_previousTable[MENU_BROWSER_NB_ENTRIES] PROGMEM = 
{
   /* 000 */ MENU_BROWSER_NO_ENTRY,
   /* 001 */ MENU_BROWSER_NO_ENTRY,
   /* 002 */ MENU_BROWSER_NO_ENTRY,
   /* 003 */   2,
   /* 004 */   3,
   /* 005 */   4,
};

/*   0 */ const char labelDevices[] PROGMEM = "MODULES";
/*   1 */ const char labelTachy[] PROGMEM = "MESURE";
/*   2 */ const char labelPulsesPerRotate[] PROGMEM = "Impulsions/tour";
/*   3 */ const char labelMeasurementWindow[] PROGMEM = "Cycle mesure";
/*   4 */ const char labelMinSpeed[] PROGMEM = "Vitesse min";
/*   5 */ const char labelMaxSpeed[] PROGMEM = "VItesse max";

const char *const MENU_DATA_labelsTable[MENU_BROWSER_NB_ENTRIES] PROGMEM =
{
    /*   0 */ labelDevices,
    /*   1 */ labelTachy,
    /*   2 */ labelPulsesPerRotate,
    /*   3 */ labelMeasurementWindow,
    /*   4 */ labelMinSpeed,
    /*   5 */ labelMaxSpeed,
};

const PROGMEM MENU_BROWSER_FUNCTION_PTR MENU_DATA_execFunctionsTable[0] = 
{
};

const PROGMEM MENU_BROWSER_EDIT_PTR MENU_DATA_editFunctionsTable[4] = 
{
    /*   2   0 */ &editPulsesPerRotate,
    /*   3   1 */ &editMeasurementWindow,
    /*   4   2 */ &editMinSpeed,
    /*   5   3 */ &editMaxSpeed,
};

const byte MENU_DATA_itemTypeTable[6] PROGMEM = 
{
   /* 000 */ menuTypeMenu,
   /* 001 */ menuTypeMenu,
   /* 002 */ menuTypeVariable,
   /* 003 */ menuTypeVariable,
   /* 004 */ menuTypeVariable,
   /* 005 */ menuTypeVariable,
};

const word _MENU_DATA_tables[] =
{
    (word)MENU_BROWSER_ROOT_LABEL,
    (word)MENU_DATA_parentTable,
    (word)MENU_DATA_childTable,
    (word)MENU_DATA_nextTable,
    (word)MENU_DATA_previousTable,
    (word)MENU_DATA_labelsTable,
    (word)MENU_DATA_execFunctionsTable,
    (word)MENU_DATA_editFunctionsTable,
    (word)MENU_DATA_itemTypeTable,
};

word* MENU_DATA_tables = (word*)&_MENU_DATA_tables;

#endif
