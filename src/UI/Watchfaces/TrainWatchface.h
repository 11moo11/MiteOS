#ifndef WATCHY_TRAIN_H
#define WATCHY_TRAIN_H

/*
SmartWatch
https://github.com/theRealc2c2/SmartWatch

Based on Bahn-for-Watchy
https://github.com/BraininaBowl/Bahn-for-Watchy

Based on SmatWatch
https://github.com/theRealc2c2/SmartWatch

Face for Watchy watch
https://watchy.sqfmi.com

Based on Train
https://github.com/uCBill/Multi_face_Watchy/blob/main/train.h
*/

#include "Watchface.h"

class TrainWatchface : public Watchface {

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