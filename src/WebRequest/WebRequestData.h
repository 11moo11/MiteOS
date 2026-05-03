#ifndef WEB_REQUEST_DATA_H
#define WEB_REQUEST_DATA_H

#include <Arduino.h>

class WebRequestData {
	public:
		int httpResponseCode = -1;
		String responseData = "";
	public: 
		String getString() { return responseData; }
		bool isSuccess() { return httpResponseCode >= 200 && httpResponseCode < 300; }
};

#endif