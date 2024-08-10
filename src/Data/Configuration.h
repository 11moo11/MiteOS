#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <Preferences.h>
#include "../MiteOS.h"
#include "settings.h"

#include "../Managers/PhoneConnectionManager.h"

#define STEPS_PER_KM 1400
#define KM_TO_MILES 0.621371

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
        
        static int getSize();
        static int usedSpace();
        static int freeSpace();
    private:
        static bool initialized;
};
#endif