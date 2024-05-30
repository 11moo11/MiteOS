#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <Preferences.h>
#include "../MiteOS.h"
#include "settings.h"

#define STEPS_PER_KM 1400
#define KM_TO_MILES 0.621371

class Configuration : Preferences {
    public:
        static void init();
        
        static void loadAll();
        static void saveAll();
        
        static void saveSettings();
        static void loadSettings();
        
        static void saveAlarms();
        static void loadAlarms();
        
        static void saveSteps();
        static std::array<uint32_t, 7> loadSteps();
    private:
        static bool initialized;
        static Preferences preferences;
};
#endif