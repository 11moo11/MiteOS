

#define STEPS_PER_KM 1400
#define KM_TO_MILES 0.621371

// Possible Values LANG_EN, LANG_DE, LANG_FR
#define LANG_EN

// This will dramatically reduce battery life and responsiveness of the watch
// Should only be used for making screenshot and never for daily driving
#define ENABLE_SCREENSHOTS true
#define DEBUG true




#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <Preferences.h>
#include "../MiteOS.h"
#include "settings.h"

#include "../Managers/PhoneConnectionManager.h"

class Configuration : Preferences {
    public:
        static Preferences preferences;
    public:
        static void init();
        
        static void loadAll();
        static void saveAll();
        
        static void saveSettings();
        static void loadSettings();
        
        static void saveAlarms();
        static void loadAlarms();
        
        static void saveBluetooth();
        static void loadBluetooth();
        
        static void saveHassConfig();
        static void loadHassConfig();
        
        static void saveNotification(uint8_t index, Notification n);
        static Notification loadNotification(uint8_t index);
        
        static void saveSteps();
        static std::array<uint32_t, 7> loadSteps(bool useCurrentData = true);
        static void saveTotalSteps();
        static uint32_t loadTotalSteps();
        
        static int getSize();
        static int usedSpace();
        static int freeSpace();
    private:
        static bool initialized;
};
#endif