#include "BluetoothManager.h"
#include "../MiteOS.h"

bool BluetoothManager::connected = false;
bool BluetoothManager::registeredForCallback = false;
BLERemoteCharacteristic* BluetoothManager::pRemoteCharacteristic;
BLEAdvertisedDevice* BluetoothManager::device;
BLEClient*  BluetoothManager::pClient;

/* Callbacks */
//callback called when an advertised device is found
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
	void onResult(BLEAdvertisedDevice advertisedDevice) {
		#ifdef DEBUG
		Serial.println("BLE Advertised Device found: " + String(advertisedDevice.toString().c_str()));
		#endif
		if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(BLEUUID(SERVICE_UUID))) {
			BLEDevice::getScan()->stop();
			BluetoothManager::device = new BLEAdvertisedDevice(advertisedDevice);
			#ifdef DEBUG
			Serial.println("%%%%%%%%%%%% Device Found %%%%%%%%%%%%%%%%%");
			#endif
		}
	}
};

//client client callback which responds to connection related events
class MyClientCallback : public BLEClientCallbacks {
	void onConnect(BLEClient* pclient) {
		BluetoothManager::connected = true;
	}

	void onDisconnect(BLEClient* pclient) {
		BluetoothManager::connected = false;
		#ifdef DEBUG
		Serial.println("%%%%%%%%%% Device has Disconnected %%%%%%%%");
		#endif
	}
};




/* BLE interfacing functions
*/
// Start by scanning for the device we want to communicate with a FreeRTOS task
void BluetoothManager::initBLE() {
	if (!connected) {
		xFindDevice((void *) 1);
		//xTaskCreatePinnedToCore( xFindDevice, "FIND_DEVICE", 4096, (void *) 1 , tskIDLE_PRIORITY, &xConnect, 0 );
	}else{
		Serial.println(sendBLE("/time", true));
	}
}

//Called from the
void BluetoothManager::xFindDevice(void * pvParameters ) {
	BLEDevice::init("");
	#ifdef DEBUG
	Serial.println("%%% Find Device Task Launched %%%");
	#endif
	
	BLEScan* pBLEScan = BLEDevice::getScan();
	pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
	pBLEScan->setInterval(40);           //I've found these interval and window values to work the best with android, but others may be better.
	pBLEScan->setWindow(39);
	pBLEScan->setActiveScan(true);
	pBLEScan->start(8);
	
	if (device) {
		#ifdef DEBUG
		Serial.println("%%% Device Found %%%");
		#endif
		formConnection((void *) 1);
		//xTaskCreatePinnedToCore( formConnection, "FIND_DEVICE", 4096, (void *) 1 , tskIDLE_PRIORITY, &xConnect, 0 );
	} else {
		#ifdef DEBUG
		Serial.println("%%%% Device Not Found %%%");
		#endif
	}
}

/* Opens connection to the server and allows us to access the characteristic that data
    is transmitted to, this should be called after xFindDevice completes, however it
    cannot be run in a seperate thread due to I2C
*/
void BluetoothManager::formConnection(void * pvParameters) {
  //if for some this function is called before we find the device then we need to
  //do the scan again and make sure that we have a device
  if (!device) {
    xFindDevice((void*) 1);
  }

  //create a client to communicate to the server through
  pClient = BLEDevice::createClient();

  //set callbacks for client, if it disconnects we need to know,
  //also we don't consider the device to be connected unless the client is connected
  pClient->setClientCallbacks(new MyClientCallback());

  //check that device is found again
  //attempting to connect to a null device will cause the device to crash
  if (device) {
    pClient->connect(device);
  } else {
    return;
  }

  //obtain a reference to the desired service
  //and check for null reference, this indicates failure
  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  if (!pRemoteService) {
    pClient->disconnect();
	#ifdef DEBUG
    Serial.println("%%%% Could not obtain remote service");
	#endif
	return;
  }

	std::map<std::string, BLERemoteCharacteristic*>* data = pRemoteService->getCharacteristics();
	std::map<std::string, BLERemoteCharacteristic*>::iterator it;

	for (it = data->begin(); it != data->end(); it++) {
		Serial.println(it->first.c_str());
	}
	
  //second verse same as the first, but for the characteristic
  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_NOTIFICATION_UPDATE);
  if (!pRemoteCharacteristic) {
    pClient->disconnect();
	#ifdef DEBUG
    Serial.println("%%%% Could not obtain remote characteristic");
	#endif
	return;
  }
}

String BluetoothManager::sendBLE(String command, bool hasReturnData) {
  String ret = "";

  //write our command to the remote characteristic
  if (pRemoteCharacteristic) {
    pRemoteCharacteristic->writeValue(command.c_str(), command.length());
	#ifdef DEBUG
    Serial.println("Wrote \"" + command + "\" to remote device");
	#endif
  } else {
    return ret;
  }

  if (hasReturnData) {
    while (ret[ret.length() - 1] != '*' && ret[ret.length() - 2] != '*' && ret[ret.length() - 3] != '*') {
      String receivedData = String(pRemoteCharacteristic->readValue().c_str());
      if (!receivedData.equals("null")) {
        ret = ret + receivedData;
        receivedData = "";
      }
    }
	#ifdef DEBUG
    Serial.println("Data Obtained from BLE Device: " + ret);
	#endif
  }
  return ret;
}