#include "src/MiteOS.h"
#include "settings.h"
#include "src/Managers/BluetoothManager.h"

MiteOS miteOS(settings);

void setup() {
	miteOS.init();
}

void loop() {
	
}