#include "stage.h"

Stage::Stage(std::string filename) : Slot(filename) {}

Stage::Stage(std::string filename, uint32_t sector) : Slot(filename, sector) {}

Stage::~Stage() {}

int64_t Stage::getNextPageOffset(uint16_t pageID) {
	int16_t numPages = _byteswap_ushort(header.numPages);
	return (pageID == numPages - 1) ? getFileSize(filename) : (_byteswap_ulong(table[pageID + 1].offset) * sector);
}

void Stage::open() {
	std::ifstream stageDat;
	stageDat.open(filename, std::ios::binary);

	int size = sizeof(SlotHeader);
	stageDat.read((char*)&this->header, size);
	uint32_t keyA = _byteswap_ulong(header.timestamp);
	uint32_t keyB = keyA ^ 0xF0F0;
	keyA = decryptor.decodeBuffer(keyA, keyB, 0, 0x0C, (uint8_t*)&header.version);

	int16_t numPages = _byteswap_ushort(header.numPages);

	table.resize(numPages);
	size = sizeof(StageTable) * numPages;
	stageDat.read((char*)&table[0], size);
	keyA = decryptor.decodeBuffer(keyA, keyB, 0, size, (uint8_t*)&table[0]);
}

void Stage::extract(uint16_t pageID, std::string output) {
	int16_t numPages = _byteswap_ushort(header.numPages);
	if (pageID > numPages) return;

	updateDir("stage", output);
	updateDir(table[pageID].name, output);

	int64_t nextOffset = getNextPageOffset(pageID);
	int64_t offset = _byteswap_ulong(table[pageID].offset) * sector;

	int64_t size = nextOffset - offset;
	extractFiles(size, offset, output, true);
}

void Stage::extractAll(std::string output) {
	int16_t numPages = _byteswap_ushort(header.numPages);

	for (int i = 0; i < numPages; i++) {
		extract(i, output);
	}	
}