#include "TOTPManager.h"

#include "../MiteOS.h"

String TOTPHandler::GenerateTotp(uint8_t *key, int keyLength, long time) {
	TOTP totp = TOTP(key, keyLength, 30);

	// generate the TOTP code and, if different from the previous one, print to screen
	String newCode = String(totp.getCode(time));

	//printDebug(F("TOTP code: "));
	//printDebug(newCode);
	
	return newCode;
}

String TOTPHandler::GenerateTotp(String in, long time) {
	size_t maxout = 64;
	char out[maxout];
	
	int r = base32decode(in.c_str(), (unsigned char*) out, maxout);
	if (r < 0 || r > maxout) {
		printDebug("Could not decode string");
		return "------";
	}
	//printDebug(in);
	//printDebug("Decoded: ");
	//printDebug(out);

	//char[] test = { 0x48,0x65,0x6c,0x6c,0x6f,0x21,0xde,0xad,0xbe,0xef };
	//Serial.println(test);
	
	return GenerateTotp((uint8_t*) out, r, time);
}