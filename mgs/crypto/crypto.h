#pragma once
#include "../common/decryptor/decryptor.h"
#include "../3rdparty/blowfish/blowfish.h"
#include "../3rdparty/md5/md5.h"
#include "../common/fileutil.h"

class Crypto {
public:
	Crypto(const std::string& stagekey);
	~Crypto();
	void encrypt(std::string& filepath, std::string output = "");
	void decrypt(std::string& filepath, std::string output = "");
private:
	Decryptor decryptor;
	void init(std::string folder);
	uint64_t* decryptKey(uint64_t key[8]);
	void calculateChecksum(uint8_t* file, int& size);

	uint64_t key[8];
	uint8_t  saltA[64];
	uint8_t  saltB[64];

	uint64_t baseKey[8] = { 0xd1f59885c26df674, 0xd64455ceca2dac72, 0x6805a2be051df165, 0x0e8935eb9d526ce7,
							0x3f9c5dfe24ff32c3, 0x265b0547cf8941b3, 0x46b5469a63cce4f9, 0x4e8e5be641df0454 };

	uint64_t fileKey[8] = { 0x55c90c7288570853, 0xed8c980aca5fa7d1, 0x049adafd8bbacf84, 0x24dc27e04dfbf06a,
							0x28ca270d1136b6b2, 0x9325211259b10a4e, 0x530b3a635c942db5, 0xeed10ef7641bd497 };
};

