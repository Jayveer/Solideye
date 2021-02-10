#pragma once
#include <inttypes.h>

class Decryptor {
private:
	const uint32_t KEY = 0x02E90EDD;
	uint32_t makePageKeyB(uint32_t salt);
	uint32_t makePageKeyA(uint32_t salt);
	uint32_t makeCNFKeyB(uint32_t salt);
	uint32_t makeCNFKeyA(uint32_t saltA, uint32_t saltB);
	uint32_t makeCNFKey(uint32_t saltA,  uint32_t saltB);
public:	
	uint32_t decodePage(uint16_t keyA, int offset, int size, uint8_t* src);
	uint32_t decodeCNF(uint32_t keyA, uint32_t keyB, int offset, int size, uint8_t* src);
	uint32_t decodeBuffer(uint32_t keyA, uint32_t keyB, unsigned int offset, unsigned int size, uint8_t* src);

};