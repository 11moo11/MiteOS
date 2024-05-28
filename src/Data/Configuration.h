#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <Preferences.h>
#include "../MiteOS.h"
#include "settings.h"

class Configuration : Preferences {
    public:
        static void init();
        
        static void loadAll();
        static void saveAll();
        
        static void saveSettings();
        static void loadSettings();
        
        static void saveAlarms();
        static void loadAlarms();
    private:
        static bool initialized;
        static Preferences preferences;
};
#endif