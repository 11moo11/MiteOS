#ifndef TOTP_MANAGER_H
#define TOTP_MANAGER_H

#include <TOTP.h>
#include <Base32-Decode.h>


class TOTPHandler {
	public:
		static String GenerateTotp(uint8_t *key, int keyLength, long time);
		static String GenerateTotp(String in, long time);
};
#endif