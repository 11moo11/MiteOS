#include "ActivityManager.h"

#include "../MiteOS.h"

uint32_t ActivityManager::getStepCount() {
	return accSensor.getCounter();
}

float ActivityManager::getWalkedDistance() {
	uint32_t steps = getStepCount();
	float dist = steps * 1.0 / STEPS_PER_KM;
	
	// TODO km -> mile
	
	return dist;
}

String ActivityManager::getWalkedDistanceStr() {
	return String(getWalkedDistance(), 2) + "km";;
}