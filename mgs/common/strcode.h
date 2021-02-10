#pragma once
#include "../common/ext_table.h"

inline
unsigned int strcode(const char* string) {
	unsigned char c;
	unsigned char* p = (unsigned char*)string;
	unsigned int id, mask = 0x00FFFFFF;

	for (id = 0; c = *p; p++) {
		id = ((id >> 19) | (id << 5));
		id += c;
		id &= mask;
	}

	return (!id) ? 1 : id;
}

inline
unsigned int strcode32(const char* string) {
	unsigned int c;
	signed   int n = 0;
	unsigned int id = 0;

	while ((c = *string++)) {
		id += ((id << (c & 0x0F)) | ((id >> 3) + (c << (n & 0x0F)) + c));
		n++;
	}
	return id;
}

inline
unsigned int fileStrcode(const char* string) {
	int i = 0;
	unsigned char c;
	unsigned char* p = (unsigned char*)string;

	for (c = *p; c != 0x2E; i++) {
		c = *(p + i);
	}

	char* filename = new char[i + 1];
	memcpy(filename, p, i - 1);
	filename[i - 1] = '\0';

	int extLength = strlen(string) - i;
	char* ext = new char[extLength + 1];
	memcpy(ext, &p[i], extLength);
	ext[extLength] = '\0';

	unsigned int hash = strcode(filename) ^ (getIDforExt(ext) << 24);
	delete[] filename;
	delete[] ext;

	return hash;
}