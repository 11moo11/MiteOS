#ifndef LANG_H
#define LANG_H

#ifdef LANG_DE
	#include "lang_de.h"
#elif LANG_FR
	#include "lang_fr.h"
#else
	#include "lang_en.h"
#endif

/* Reimplementation with defines from:

DateStrings.cpp
 * Definitions for date strings for use with the Time library
 *
 * Updated for Arduino 1.5.7 18 July 2014
 *
 * No memory is consumed in the sketch if your code does not call any of the string methods
 * You can change the text of the strings, make sure the short strings are each exactly 3 characters 
 * the long strings can be any length up to the constant dt_MAX_STRING_LEN defined in TimeLib.h
 * 
 */


// the short strings for each day or month must be exactly dt_SHORT_STR_LEN
#define dt_SHORT_STR_LEN  3 // the length of short strings
#define dt_MAX_STRING_LEN 12 // Length of the longes name

static char buffer[dt_MAX_STRING_LEN+1];  // must be big enough for longest string and the terminating null

const char monthStr0[] PROGMEM = TXT_ERR;
const char monthStr1[] PROGMEM = TXT_JANUARY;
const char monthStr2[] PROGMEM = TXT_FEBRUARY;
const char monthStr3[] PROGMEM = TXT_MARCH;
const char monthStr4[] PROGMEM = TXT_APRIL;
const char monthStr5[] PROGMEM = TXT_MAY;
const char monthStr6[] PROGMEM = TXT_JUNE;
const char monthStr7[] PROGMEM = TXT_JULY;
const char monthStr8[] PROGMEM = TXT_AUGUST;
const char monthStr9[] PROGMEM = TXT_SEPTEMBER;
const char monthStr10[] PROGMEM = TXT_OCTOBER;
const char monthStr11[] PROGMEM = TXT_NOVEMBER;
const char monthStr12[] PROGMEM = TXT_DECEMBER;

const PROGMEM char * const PROGMEM monthNames_P[] = {
	monthStr0,monthStr1,monthStr2,monthStr3,monthStr4,monthStr5,monthStr6,
	monthStr7,monthStr8,monthStr9,monthStr10,monthStr11,monthStr12
};

const char monthShortNames_P[] PROGMEM = TXT_ERR TXT_JANUARY_SHORT TXT_FEBRUARY_SHORT TXT_MARCH_SHORT TXT_APRIL_SHORT TXT_MAY_SHORT TXT_JUNE_SHORT TXT_JULY_SHORT TXT_AUGUST_SHORT TXT_SEPTEMBER_SHORT TXT_OCTOBER_SHORT TXT_NOVEMBER_SHORT TXT_DECEMBER_SHORT;

const char dayStr0[] PROGMEM = TXT_ERR;
const char dayStr1[] PROGMEM = TXT_SUNDAY;
const char dayStr2[] PROGMEM = TXT_MONDAY;
const char dayStr3[] PROGMEM = TXT_TUESDAY;
const char dayStr4[] PROGMEM = TXT_WEDNESDAY;
const char dayStr5[] PROGMEM = TXT_THURSDAY;
const char dayStr6[] PROGMEM = TXT_FRIDAY;
const char dayStr7[] PROGMEM = TXT_SATURDAY;

const PROGMEM char * const PROGMEM dayNames_P[] = {
	dayStr0,dayStr1,dayStr2,dayStr3,dayStr4,dayStr5,dayStr6,dayStr7
};

const char dayShortNames_P[] PROGMEM = TXT_ERR TXT_SUNDAY_SHORT TXT_MONDAY_SHORT TXT_TUESDAY_SHORT TXT_WEDNESDAY_SHORT TXT_THURSDAY_SHORT TXT_FRIDAY_SHORT TXT_SATURDAY_SHORT;

class Lang {
	public:
		static char* monthStr(uint8_t month) {
			#ifdef SUNDAY_IS_ZERO
			month += 1;
			#endif

			strcpy_P(buffer, (PGM_P)pgm_read_ptr(&(monthNames_P[month])));
			return buffer;
		};
		
		static char* monthShortStr(uint8_t month) {
			#ifdef SUNDAY_IS_ZERO
			month += 1;
			#endif

			for (int i=0; i < dt_SHORT_STR_LEN; i++)      
				buffer[i] = pgm_read_byte(&(monthShortNames_P[i+ (month*dt_SHORT_STR_LEN)]));  
			buffer[dt_SHORT_STR_LEN] = 0;
			return buffer;
		};
		
		static char* dayStr(uint8_t day) {
			#ifdef SUNDAY_IS_ZERO
			day += 1;
			#endif

			strcpy_P(buffer, (PGM_P)pgm_read_ptr(&(dayNames_P[day])));
			return buffer;
		};
		
		static char* dayShortStr(uint8_t day) {
			#ifdef SUNDAY_IS_ZERO
			day += 1;
			#endif

			uint8_t index = day*dt_SHORT_STR_LEN;
			for (int i = 0; i < dt_SHORT_STR_LEN; i++)      
				buffer[i] = pgm_read_byte(&(dayShortNames_P[index + i]));  
			buffer[dt_SHORT_STR_LEN] = 0; 
			return buffer;
		};
};
#endif