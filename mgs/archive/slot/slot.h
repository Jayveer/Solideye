#pragma once
#include <time.h>

#include "../archive.h"
#include "../../config/cnf.h"
#include "../../common/decryptor/decryptor.h"
#include "../../3rdparty/zlib/wrapper/ZlibWrapper.h"

struct SlotHeader {
	uint32_t timestamp; //interchangable with strcode
	uint16_t version;
	uint16_t pageSize;  //multiplied by sector size
	uint16_t numPages;
	uint16_t unknownA;
	uint32_t unknownB;
};

struct SlotPageInfo {
	int64_t offset;
	int64_t size;
};

class Slot : public CNFHandler, public Archive {
protected:
	SlotHeader header;
	Decryptor decryptor;
	std::string filename;
	uint32_t sector = 0x800;
	std::vector<SlotPageInfo> pageInfo;

	void decryptPage(uint8_t* src, int64_t size);
	void decryptPageCNF(const std::string& stageName, uint8_t* src);
	void extractFiles(int64_t size, int64_t offset, std::string& output, bool isEncrypted = 0);
	uint8_t* _decryptPage(uint32_t key, uint32_t saltA, uint32_t saltB, uint32_t saltC, int offset, int size, bool makeKey = 0);

	int32_t getSectorSize();
	int64_t getNextSectorOffset(int64_t currentOffset);
	void processQar(std::string filename, std::string region, std::string* workDir);
	void processAfp(std::string filename, std::string region, std::string* workDir);
	void processVram(std::string filename, std::string region, std::string* workDir);
	void processEnc(std::string region, uint8_t** section, int64_t size, int64_t sectionSize);
	void processFile(std::string filename, std::string region, std::string* workDir, uint8_t* fileData, int size);
	void processSlot(std::string filename, int16_t page, int64_t offset, std::string region, std::string* workDir);
private:
	int64_t getMaxPageSize(int64_t offset);
	void initPageInfo(std::ifstream& slotDat);
	int64_t getMaxPageSize(std::ifstream& slotDat);
	uint8_t* decompressPage(uint8_t* compressedPage, int64_t compressedSize, int64_t decompressedSize);
public:
	~Slot();
	Slot(std::string filename);
	Slot(std::string filename, uint32_t sector);

	std::string getFilename();
	std::string getCreatedDate();

	void open();
	void extractAll(std::string output = "");
	void extract(uint16_t pageID, std::string output = "");

};