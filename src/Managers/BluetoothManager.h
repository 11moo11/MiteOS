#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include "BLEDevice.h"
#include "BLEHIDDevice.h"
#include "Arduino.h"

#define SERVICE_UUID "0000fdaa-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_NOTIFICATION_UPDATE "00000007-09da-4bed-9652-f507366fcfc5"

class BluetoothManager {
	public:
		static BLERemoteCharacteristic* pRemoteCharacteristic;
		static BLEAdvertisedDevice* device;
		static BLEClient*  pClient;
		static bool connected;
		static bool registeredForCallback;
	public:
		static void initBLE();
		static void formConnection(void * pvParameters);
		static void xFindDevice(void * pvParameters);
		static String sendBLE(String command, bool hasReturnData);
};

#endif