#ifndef TIME_FUNC_H
#define TIME_FUNC_H

#include "../Managers/AlarmManager.h"

class TimeFunc {
    public:
        static uint16_t timeDiff(AlarmData to) {
            AlarmData current = { true, false, 0, MiteOS::currentTime.Hour, MiteOS::currentTime.Minute };
            return timeDiff(current, to);
        }

        static uint16_t timeDiff(AlarmData from, AlarmData to) {
            uint16_t diff = 0;
            AlarmData dat = from;
            if(to.hour == from.hour && to.minute < from.minute) to.hour += 24;
            if(to.hour < from.hour) to.hour += 24;

            while(from.hour < to.hour) {
                diff += 60;
                from.hour++;
            }
            diff += to.minute - from.minute;

            return diff;
        }
};

#endif