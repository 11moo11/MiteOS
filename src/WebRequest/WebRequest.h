#ifndef WEB_REQUEST_H
#define WEB_REQUEST_H

#include "MiteOS.h"
#include <Arduino.h>
#include "WebRequestData.h"
#include <HTTPClient.h>

class WebRequest {
	public:
		static WebRequestData GET(String url);
		static WebRequestData POST(String url, String payload, String authorization = "", String content_type = "application/json");
};

#endif