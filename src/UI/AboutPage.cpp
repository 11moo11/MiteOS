#include "AboutPage.h"
#include "../MiteOS.h"
#include "../Images/menu_icons.h"
#include <Fonts/FreeMonoBold9pt7b.h>
#include "../Managers/NetworkManager.h"

void AboutPage::drawPage() {
	MiteOS::drawButtonIcon(BTN_BACK, icon_left);
	
	mDisplay.setFont(&FreeMonoBold9pt7b);
	
	mDisplay.setCursor(0, 50);

	mDisplay.print("LibVer: ");
	mDisplay.println(WATCHY_LIB_VER);
	
	mDisplay.print("OsVer: ");
	mDisplay.println(MITE_OS_VER);

	mDisplay.print("Rev: v");
	mDisplay.println(MiteOS::getBoardRevision());

	mDisplay.print("Batt: ");
	float voltage = MiteOS::getBatteryVoltage();
	mDisplay.print(voltage);
	mDisplay.println("V");

	mDisplay.print("Uptime: ");
	mRTC.read(MiteOS::currentTime);
	time_t b = makeTime(osBootTime);
	time_t c = makeTime(MiteOS::currentTime);
	int totalSeconds = c-b;
	//int seconds = (totalSeconds % 60);
	int minutes = (totalSeconds % 3600) / 60;
	int hours = (totalSeconds % 86400) / 3600;
	int days = (totalSeconds % (86400 * 30)) / 86400; 
	mDisplay.print(days);
	mDisplay.print("d");
	mDisplay.print(hours);
	mDisplay.print("h");
	mDisplay.print(minutes);
	mDisplay.println("m");    
	if(WifiConfigured){
		mDisplay.print("SSID: ");
		mDisplay.println(lastWifiSSID);
		mDisplay.print("IP: ");
		mDisplay.println(IPAddress(lastWifiIPAddress).toString());
	}else{
		mDisplay.println("WiFi Not Connected");
	}
}

bool AboutPage::onButtonPressed(uint8_t buttonIndex) {
	if(buttonIndex == BTN_BACK) {
		MiteOS::showPage(GLOBAL_PAGE_SETTINGS);
		return true;
	}

	return false;
}