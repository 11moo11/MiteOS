#include "ActivityManager.h"

#include "../MiteOS.h"

RTC_DATA_ATTR uint32_t stepOffset = 0;

void ActivityManager::restoreSteps() {
	uint8_t dow = MiteOS::currentTime.Wday;
	stepOffset = Configuration::loadSteps(false)[dow - 1];
	if(stepOffset > 1000000) stepOffset = 0;
	stepOffset = 0;
	printDebug("Restored Steps: " + String(stepOffset));
}

void ActivityManager::resetSteps(bool save) {
	if(save) {
		saveSteps();
		Configuration::saveTotalSteps();
	}
	stepOffset = 0;
	accSensor.resetStepCounter();
}

void ActivityManager::saveSteps() {
	Configuration::saveSteps();
}

uint32_t ActivityManager::getStepCount() {
	uint32_t stepCount = accSensor.getCounter();
	if(stepCount > 1000000) stepCount = 0;
	stepCount += stepOffset;

	return stepCount;
}

float ActivityManager::getWalkedDistance() {
	uint32_t steps = getStepCount();
	float dist = steps * 1.0 / STEPS_PER_KM;
	
	// TODO km -> mile
	
	return dist;
}

String ActivityManager::getWalkedDistanceStr() {
	return String(getWalkedDistance(), 2) + "km";
}



float ActivityManager::getTotalWalkedDistance() {
	uint32_t steps = getTotalStepCount();
	float dist = steps * 1.0 / STEPS_PER_KM;
	
	// TODO km -> mile
	
	return dist;
}
String ActivityManager::getTotalWalkedDistanceStr() {
	return String(getTotalWalkedDistance(), 2) + "km";
}
uint32_t ActivityManager::getTotalStepCount() {
	return getStepCount() + Configuration::loadTotalSteps();
}