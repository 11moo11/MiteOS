#include "WebRequest.h"

#include "../Managers/BluetoothManager.h"

WebRequestData WebRequest::GET(String url) {
	printDebug("GET " + url);
	#if PROXY_WEB_REQUESTS_THROUGH_PHONE
	return BluetoothManager::proxyGetWebRequest(url);
	#else
	WebRequestData data = WebRequestData();
	if (WifiConnectionManager::connectWifi()) {
		HTTPClient http;
		http.setConnectTimeout(3000); // 3 second max timeout
		http.begin(url.c_str());
			
		data.httpResponseCode = http.GET();
		if (data.httpResponseCode >= 200 && data.httpResponseCode < 300) {
			data.responseData = http.getString();
		}
		http.end();
	}
	printDebug(data.getString());
	return data;
	#endif
}

WebRequestData WebRequest::POST(String url, String payload, String authorization, String content_type) {
	printDebug("POST " + url);
	#if PROXY_WEB_REQUESTS_THROUGH_PHONE
	return BluetoothManager::proxyPostWebRequest(url, payload, authorization, content_type);
	#else
	WebRequestData data = WebRequestData();
	if (WifiConnectionManager::connectWifi()) {
		HTTPClient http;
		http.setConnectTimeout(3000); // 3 second max timeout
		http.begin(url.c_str());

		if(authorization && authorization.length() > 0)
			http.addHeader("Authorization", authorization);
		if(content_type && content_type.length() > 0)
			http.addHeader("Content-Type", content_type);
		
		data.httpResponseCode = http.POST(payload);
		if (data.httpResponseCode >= 200 && data.httpResponseCode < 300) {
			data.responseData = http.getString();
		}
		http.end();
	}
	printDebug(data.getString());
	return data;
	#endif
}