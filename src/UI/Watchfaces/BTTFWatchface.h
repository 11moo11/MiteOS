#ifndef WATCHY_BTTF_H
#define WATCHY_BTTF_H

#include "../../Images/bttf.h"
#include "Watchface.h"

class BTTFWatchface : public Watchface {

    public:
        void draw();
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawBattery();

    private:
        void printCentered(uint16_t x, uint16_t y, String text);
        void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color, uint8_t width);

};

#endif