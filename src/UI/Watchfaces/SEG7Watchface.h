#ifndef WATCH_7_SEG_H
#define WATCH_7_SEG_H

#include "Watchface.h"

class SEG7Watchface : public Watchface {
    public:
        void draw();
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawWeather();
        void drawBattery();
};
#endif