#include "crypto.h"


Crypto::Crypto(const std::string& stagekey) {
	init(stagekey);
}

Crypto::~Crypto() {}


void Crypto::init(std::string folder) {
	uint64_t* filesKey = decryptKey(fileKey);

	uint8_t* xKey = (uint8_t*)&key[0];
	uint8_t* xFilesKey = (uint8_t*)&filesKey[0];

	int size = folder.length();

	char* folderKey = md5Bytes(folder);

	for (int i = 0; i < 64; i++) {
		xKey[i] = xFilesKey[i] ^ folderKey[i % 0x10];
		saltA[i] = (xKey[i] ^ 0x36);
		saltB[i] = (xKey[i] ^ 0x5c);
	}

	delete[] filesKey;
}

uint64_t* Crypto::decryptKey(uint64_t key[8]) {
	uint64_t* result = new uint64_t[8];

	CBlowFish blowfish((uint8_t*)&baseKey[1], 56);
	uint64_t last = baseKey[0];

	blowfish.Decrypt((uint8_t*)key, (uint8_t*)result, 64, CBlowFish::ECB);

	for (int i = 0; i < 8; i++) {
		result[i] ^= last;
		last = key[i];
	}

	return result;
}

void Crypto::calculateChecksum(uint8_t* file, int& size) {
	MD5 md5;
	md5.update(saltA, 64);
	md5.update(file, size);
	md5.finalize();

	char* hash1 = md5.bytedigest();

	MD5 md52;
	md52.update(saltB, 64);
	md52.update(hash1, 16);
	md52.finalize();

	char* hash2 = md52.bytedigest();
	memcpy(&file[size], hash2, 16);
	size += 0x10;
}

uint8_t* writePad(uint8_t* buffer, int& size, int& rem) {
	rem = size % 8;
	int padding = 8 - rem;
	int endBytes = size - rem;
	uint8_t* ptBuffer = new uint8_t[rem];

	for (int i = 0; i < padding; i++) {
		buffer[size + i] = padding;
	}

	buffer += size - rem;
	for (int i = 0; i < rem; i++) {
		ptBuffer[i] = buffer[i];
	}

	size += padding;
	return ptBuffer;
}

void addEndBytes(uint8_t* file, int& size, uint8_t* endBytes, int rem) {
	memcpy(&file[size], endBytes, rem);
	delete[] endBytes;
	size += rem;
}

void Crypto::encrypt(std::string& filepath, std::string output) {
	int size, rem;
	uint8_t* file = bufferFromFile(filepath, size, 0x18);
	uint8_t* endBytes = writePad(file, size, rem);

	decryptor.encodeFile(key, 0, size, file);
	addEndBytes(file, size, endBytes, rem);
	calculateChecksum(file, size);

	std::string outputFile = getCurrentDir(filepath) + ".enc";
	writeDataToFile(file, size, outputFile, output);
}

void Crypto::decrypt(std::string& filepath, std::string output) {
	int size;
	uint8_t* file = bufferFromFile(filepath, size);

	decryptor.decodeFile(key, 0, size, file);
	int outSize = size - 0x18;
	std::string outputFile = getCurrentDir(filepath) + ".dec";
	writeDataToFile(file, outSize, outputFile, output);
}