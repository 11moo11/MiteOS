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
BLEService *BluetoothManager::updateService;
BLEServer *BluetoothManager::pServer;
bool BluetoothManager::connected = false;
bool BluetoothManager::operationInProgress = false;
String BluetoothManager::currentDataField;
boolean BluetoothManager::blockingCommandInProgress;
String *BluetoothManager::bleReturnString;

char tmp_buffer[256];

/*
void onNotificationEvent(String event){
	//event will have value 'posted' or 'removed' 
	Serial.printf("Notification Event '%s'\n", event);
}

void BluetoothManager::addData(String data) {
	printDebug("Received:" + data);
	currentDataField += data;
	if (!blockingCommandInProgress) {
		*bleReturnString = currentDataField;
	}
}
*/

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

/* BLE interfacing functions
*/
void BluetoothManager::initBLE() {
	printDebug("Initializing BT Device");
	BLEDevice::init("MiteWatch");
	esp_err_t err = esp_ble_gatt_set_local_mtu(256);
	pServer = BLEDevice::createServer();
	
	// add server callback so we can detect when we're connected.
	pServer->setCallbacks(new cb());
	
	// Security: device requires bonding
	//BLESecurity* security = new BLESecurity();
	//security->setStaticPIN(1234);
	//security->setAuthenticationMode(ESP_LE_AUTH_BOND);
	
	
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
	//startBLEAdvertising();
}

void BluetoothManager::startBLEAdvertising() {
	printDebug("startBLEAdvertising");
	/*
	// BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
	BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(SERVICE_UUID);
	pAdvertising->setScanResponse(true);
	pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
	pAdvertising->setMinPreferred(0x12);
	BLEDevice::startAdvertising();
	*/
	BLEAdvertising* advertising = pServer->getAdvertising();
	//advertising->setAppearance(192);
	//advertising->addServiceUUID(SERVICE_UUID);
	//advertising->setScanResponse(true);
	//advertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
	//advertising->setMinPreferred(0x12);
	advertising->start();
}

/*
// sends BLE command and returns data to a specific string. This function can be blocking (if you need it to perform a specific action) or non-blocking
// if you don't mind the data being used as its received.
bool BluetoothManager::sendBLE(String command, String *returnString, boolean blocking) {
	if (connected && !operationInProgress) {
		blockingCommandInProgress = blocking;
		operationInProgress = true;
		commandCharacteristic->setValue(command.c_str());
		commandCharacteristic->notify();

		printDebug("Sent BLE Command: " + command);

		if (blockingCommandInProgress) {
			currentDataField = "";

			unsigned long startTime = millis();
			while (operationInProgress && (startTime + 8000 > millis()))
				delay(25);

			operationInProgress = false;
			if (currentDataField.length() == 0) {
				return false;
			} else {
				*returnString = currentDataField;
				return true;
			}
		} else {
			currentDataField = "";
			bleReturnString = returnString;
			*returnString = currentDataField;

			unsigned long startTime = millis();
			while ((currentDataField.length() == 0) && (startTime + 1000 > millis()))
				delay(25);

			if (currentDataField.length() == 0) {
				operationInProgress = false;
				return false;
			} else {
				return true;
			}
		}
	}

	return false;
}

bool BluetoothManager::sendBLE(String command) {
	if (connected && !operationInProgress) {
		operationInProgress = true;
		commandCharacteristic->setValue(command.c_str());
		commandCharacteristic->notify();

		printDebug("Sent BLE Command: " + command);
		unsigned long startTime = millis();
		while (operationInProgress && (startTime + 200 > millis()))
		delay(25);
		return !operationInProgress;
	}
	return false;
}
*/

void BluetoothManager::parseCommand(String value) {

	if (value.startsWith("ECHO=")) {
		value.replace("ECHO=", "");
		Serial.println(value);

	} else if (value.startsWith("NOTIFICATION_LIST")) {
		value.replace("NOTIFICATION_LIST=", "");
		//createNotificationList(value);
		Serial.println(value);

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
}

void BluetoothManager::test() {
	Serial.println("Requesting notification list...");
	int notificationType = 0;  //All notifications
	sprintf(tmp_buffer, "GET_NOTIF_LIST=%d", notificationType);
	notificationUpdateCharacteristic->setValue(tmp_buffer);
	notificationUpdateCharacteristic->notify();
}