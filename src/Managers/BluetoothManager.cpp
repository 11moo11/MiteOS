#include "BluetoothManager.h"
#include "../MiteOS.h"
#include <BLE2902.h>

#ifdef DEBUG
#define printDebug(a) Serial.println(a)
#endif
#ifndef DEBUG
#define printDebug(a)
#endif

BLECharacteristic *BluetoothManager::commandCharacteristic;
BLECharacteristic *BluetoothManager::notificationUpdateCharacteristic;
BLEService *BluetoothManager::pService;
BLEServer *BluetoothManager::pServer;
bool BluetoothManager::connected = false;

bool BluetoothManager::initialized = false;

char BluetoothManager::tmp_buffer[256];

RTC_DATA_ATTR bool btDeviceRegistered(false);
RTC_DATA_ATTR BLEAddress btLastDevice("0");

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("BLE Advertised Device found: ");
      Serial.println(advertisedDevice.toString().c_str());
    } // onResult
}; // MyAdvertisedDeviceCallbacks

class cb : public BLEServerCallbacks {
	void onConnect(BLEServer *pServer) {
		BluetoothManager::connected = true;	
		printDebug("BLE Device Connected");
	}
	void onDisconnect(BLEServer *pServer) {
		BluetoothManager::connected = false;
		printDebug("BLE Device Disconnected");
	}
};

class ccb : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
		std::string rxValue = pCharacteristic->getValue();

		Serial.println("Received value:");

		if (rxValue.length() > 0) {

			for (int i = 0; i < rxValue.length(); i++) {
				Serial.print(rxValue[i]);
			}
			Serial.println(" ");

			String value = rxValue.c_str();
			BluetoothManager::parseCommand(value);
		}
	}
};


static void my_gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* param) {
	switch(event) {
		case ESP_GAP_BLE_AUTH_CMPL_EVT: {
			BLEAddress address = BLEAddress(param->ble_security.auth_cmpl.bd_addr);
			BLEDevice::whiteListAdd(address);
			
			btLastDevice = address;
			btDeviceRegistered = true;
			
			Serial.print("Bonded with ");
			Serial.println(address.toString().c_str());
			break;
		}
	}
}

/* BLE interfacing functions
*/
void BluetoothManager::init() {
	if(initialized) return;
	printDebug("Initializing BT Device");
	BLEDevice::init("MiteWatch");
	esp_err_t err = esp_ble_gatt_set_local_mtu(256);
	pServer = BLEDevice::createServer();
	
	// add server callback so we can detect when we're connected.
	pServer->setCallbacks(new cb());
	
	// Security: device requires bonding
	BLESecurity* security = new BLESecurity();
	//security->setStaticPIN(1234);
	security->setAuthenticationMode(ESP_LE_AUTH_BOND);
	//security->setCapability(ESP_IO_CAP_NONE);
	//security->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
	
	initialized = true;
}

void BluetoothManager::bondDevice() {
	init();
	
	btDeviceRegistered = false;
	
	Serial.println("Registering device...");
	
	pServer->getAdvertising()->setScanFilter(false, false);

	BLEDevice::setCustomGapHandler(my_gap_event_handler);

	startBLEAdvertising();

	while(!btDeviceRegistered) {
		delay(100);
	}
	
	connected = pServer->getConnectedCount() > 0;
	
	Configuration::saveBluetooth();
}

void BluetoothManager::connectDevice() {
	if(!btDeviceRegistered) return;
	if(connected) return;
	
	init();
	
	BLEDevice::whiteListAdd(btLastDevice);
	pServer->getAdvertising()->setScanFilter(false, false);
	
	pService = pServer->createService(SERVICE_UUID);
	// define the characteristics and how they can be used
	notificationUpdateCharacteristic = pService->createCharacteristic(
		CHARACTERISTIC_UUID_TX,
		BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ
	);
	notificationUpdateCharacteristic->addDescriptor(new BLE2902());
	
	commandCharacteristic = pService->createCharacteristic(
		CHARACTERISTIC_UUID_RX,
		BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_BROADCAST
	);
	commandCharacteristic->setCallbacks(new ccb());
	
	//notificationUpdateCharacteristic->setCallbacks(new notification_update_callback());
	//notificationUpdateCharacteristic->setValue("");
	
	pService->start();
	
	startBLEAdvertising();
	
	while(!connected) {
		//pServer->connect(btLastDevice);
		delay(100);
	}
	/*
	Serial.println(pServer->getConnectedCount());
	connected = pServer->connect(btLastDevice);
	Serial.println(pServer->getConnectedCount());
	*/
}

void BluetoothManager::startBLEAdvertising() {
	printDebug("startBLEAdvertising");
	BLEAdvertising* advertising = pServer->getAdvertising();
	//advertising->setAppearance(192);
	//advertising->addServiceUUID(SERVICE_UUID);
	advertising->setScanResponse(true);
	//advertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
	//advertising->setMinPreferred(0x12);
	advertising->start();
}

void BluetoothManager::parseCommand(String value) {

	if (value.startsWith("ECHO=")) {
		value.replace("ECHO=", "");
		Serial.println(value);

	} else if (value.startsWith("NOTIFICATION_LIST")) {
		value.replace("NOTIFICATION_LIST=", "");
		//createNotificationList(value);
		Serial.println(value);
		value.toCharArray(tmp_buffer, 256, 0);

	} else if (value.startsWith("NEW_NOTIFICATION=")) {
		value.replace("NEW_NOTIFICATION=", "");
		//alertNewNotification(value);
		Serial.println(value);
		//NEW_NOTIFICATION={"appName":"Messages","category":"msg","id":0,"pName":"com.google.android.apps.messaging","text":"MHNYMA AΠO KINHTO","title":"Μωρό μου"}
		//NEW_NOTIFICATION={"appName":"Gmail","category":"email","id":0,"pName":"com.google.android.gm","subText":"themelisx@gmail.com","text":"Hello","title":"Παναγιώτης Θ"}

	} else if (value.startsWith("GET_LIST")) {
		//getNotificationList();
		Serial.println(value);

	} else if (value.startsWith("ICON=")) {
		value.replace("ICON=", "");
		/*
		int inputStringLength = value.length();  //Get length of input
		char *inputString = (char *)malloc(inputStringLength + 1);
		value.getBytes((unsigned char *)inputString, inputStringLength);

		int decodedLength = BASE64::decodeLength(inputString);
		uint8_t *base64Result = (uint8_t *)malloc(inputStringLength);

		BASE64::decode(inputString, base64Result);

		drawArrayJpeg(base64Result, sizeof(base64Result), 0, 0);  //last two are coordinates to draw image

		free(base64Result);
		free(inputString);
		*/
	} else {
		Serial.print("Unknown command: ");
		Serial.println(value);
	}
	BluetoothManager::waitingForResponse = false;
}

void BluetoothManager::requestNotifications() {
	Serial.println("Requesting notification list...");
	int notificationType = 0;  //All notifications
	sprintf(tmp_buffer, "GET_NOTIF_LIST=%d", notificationType);
	notificationUpdateCharacteristic->setValue(tmp_buffer);
	notificationUpdateCharacteristic->notify();
	
	waitingForResponse = true;
	while(waitingForResponse) {
		delay(100);
	}
}