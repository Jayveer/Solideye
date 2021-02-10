#include "decryptor.h"

uint32_t Decryptor::makeCNFKey(uint32_t saltA, uint32_t saltB) {
    return (saltB << 0x07) + saltB + saltA;
}

uint32_t Decryptor::makeCNFKeyA(uint32_t saltA, uint32_t saltB) {
    return makeCNFKey(0xA78925D9, saltA) + saltB;
}

uint32_t Decryptor::makeCNFKeyB(uint32_t salt) {
    return makeCNFKey(0x7A88FB59, salt);
}

uint32_t Decryptor::makePageKeyA(uint32_t salt) {
    uint32_t pageKeyA = salt ^ 0x6576;
    pageKeyA <<= 0x10;
    return pageKeyA |= salt;
}

uint32_t Decryptor::makePageKeyB(uint32_t salt) {
    return salt * 0x0116;
}

uint32_t Decryptor::decodePage(uint16_t keyA, int offset, int size, uint8_t* src) {
    uint32_t pageKey = keyA ^ 0x9385;
    uint32_t pageKeyA = makePageKeyA(pageKey);
    uint32_t pageKeyB = makePageKeyB(pageKey);
    return decodeBuffer(pageKeyA, pageKeyB, offset, size, src);
}

uint32_t Decryptor::decodeCNF(uint32_t keyA, uint32_t keyB, int offset, int size, uint8_t* src) {
    uint32_t cnfKeyA = makeCNFKeyA(keyA, keyB);
    uint32_t cnfKeyB = makeCNFKeyB(keyA);
    return decodeBuffer(cnfKeyA, cnfKeyB, offset, size, src);
}

uint32_t Decryptor::decodeBuffer(uint32_t keyA, uint32_t keyB, unsigned int offset, unsigned int size, uint8_t* src) {
    uint32_t* srcP = (uint32_t*)src;
    size /= 4;

    for (int i = offset; i < size; i++) {
        uint32_t interval = keyA * KEY;
        srcP[i] ^= keyA;
        keyA = interval + keyB;
    }
    return keyA;
}