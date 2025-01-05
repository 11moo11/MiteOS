#include "AlarmManager.h"

#include "../MiteOS.h"
#include "../UI/Page.h"
#include "../Data/DayMask.h"
#include "../func/TimeFunc.h"

void AlarmManager::checkTimers() {
	if(timer.triggered
	|| (timer.enableAlarm && MiteOS::currentTime.Hour == timer.hour && MiteOS::currentTime.Minute == timer.minute)) {
		timer.triggered = true;
		pageData.pageIndex = GLOBAL_PAGE_TIMER;
		AlertManager::vibMotor(100, 10);
	}
}

void AlarmManager::checkAlarms() {
	for(uint8_t i = 0; i < ALARM_COUNT; i++) {
		if(alarms[i].triggered
		|| (alarms[i].enableAlarm && MiteOS::currentTime.Hour == alarms[i].hour && MiteOS::currentTime.Minute == alarms[i].minute)) {
            
            DayMask dm = unpackDayMask(alarms[i].mode);
			if(dm.once) {
				// Alarm should only be triggered once, so we disable it and save it to the settings file
				alarms[i].enableAlarm = false;
				Configuration::saveAlarms(); 
			}else{
                bool shouldTrigger = false;

                // Check if the Workday is part of the DayMask, otherwise just continue and dont trigger
                switch(MiteOS::currentTime.Wday) {
                    case 1: shouldTrigger = dm.sunday; break;
                    case 2: shouldTrigger = dm.monday; break;
                    case 3: shouldTrigger = dm.tuesday; break;
                    case 4: shouldTrigger = dm.wednesday; break;
                    case 5: shouldTrigger = dm.thursday; break;
                    case 6: shouldTrigger = dm.friday; break;
                    case 7: shouldTrigger = dm.saturday; break;
                    default: break;
                }

                if(!shouldTrigger) continue;
            } 

			alarms[i].triggered = true;
			
			pageData.pageIndex = GLOBAL_PAGE_ALARM;
		}
	}
}

AlarmData AlarmManager::getNextAlarm() {
	AlarmData ad;
	uint16_t lowest = 10000;
	if(timer.enableAlarm) {
		ad = timer;
		lowest = TimeFunc::timeDiff(timer);
	}
	
	for(uint8_t i = 0; i < ALARM_COUNT; i++) {
		if(alarms[i].enableAlarm) {
			uint16_t diff = TimeFunc::timeDiff(alarms[i]);
			if(diff < lowest) {
				lowest = diff;
				ad = alarms[i];
			}
		}
	}

	return ad;
}