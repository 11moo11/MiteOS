#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H

#include <Arduino.h>

typedef struct AlarmData {
	bool enableAlarm;
	bool triggered;
	uint8_t mode;
	uint8_t hour;
	uint8_t minute;
} AlarmData;

class AlarmManager {
    public:
        static void checkTimers();
        static void checkAlarms();
        static AlarmData getNextAlarm();
};

#endif