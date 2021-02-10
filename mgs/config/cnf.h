#pragma once
#include <vector>

#include "../common/util.h"
#include "../common/strcode.h"
#include "../../dictionary/dictionary.h"

struct DataCNFTag {
	uint32_t id;
	int32_t size;
	int64_t offset;
};

struct DataCNF {
	uint32_t numTags;
	uint32_t pad;
	DataCNFTag tags[];
};

struct CNFREGION {
	const char* name;
	int id;
};

enum CNFTYPE {
	ASCII,
	BINARY
};

class CNFHandler {
public:
	virtual int32_t getSectorSize() = 0;
	virtual int64_t getNextSectorOffset(int64_t currentOffset) = 0;
	virtual void processQar(std::string filename, std::string region, std::string* workDir) = 0;
	virtual void processAfp(std::string filename, std::string region, std::string* workDir) = 0;
	virtual void processVram(std::string filename, std::string region, std::string* workDir) = 0;
	virtual void processEnc(std::string region, uint8_t** section, int64_t size, int64_t sectionSize) = 0;
	virtual void processFile(std::string filename, std::string region, std::string* workDir, uint8_t* fileData, int size) = 0;
	virtual void processSlot(std::string filename, int16_t page, int64_t offset, std::string region, std::string* workDir) = 0;
};

class DataConfig {
public:
	DataConfig(std::string filename);
	DataConfig(uint8_t* data, int size);

	~DataConfig();
	void read(CNFTYPE type = ASCII);
	void setWorkDir(std::string& workdir);
	void setHandler(CNFHandler* cnfHandler);
	const char* getRegionForID(uint32_t id);
private:
	int dataSize = 0;
	DataCNF* cnf = {};
	uint8_t* data = {};
	std::string filename;
	uint8_t* section = {};
	uint64_t dataPtr = 0;
	std::string* workdir = {};
	CNFHandler* cnfHandler = {};
	std::string currentRegion;
	int64_t currentEncSize = 0;
	std::stringstream textStream;
	int64_t currentSectionSize = 0;

	void readASCII();
	void readBinary();
	uint32_t getCNFSize();
	void initTextStream();
	void initBinaryStream();
	void readEnc(DataCNFTag tag);
	void readFlag(DataCNFTag tag);
	void readSlot(DataCNFTag tag);
	void readLine(std::string line);
	void readFlag(std::string line);
	void readSection(DataCNFTag tag);
	void readVram(std::vector<std::string> tokens);
	void readSlot(std::vector<std::string> tokens);
	void readLine(DataCNFTag tag, DataCNFTag nextTag);
	void readFile(DataCNFTag tag, DataCNFTag nextTag);

	void endSection(DataCNFTag tag);
	void startSection(DataCNFTag tag);

	const CNFREGION cnfRegion[7] = {
		{ "end",		 0x00000000	},
		{ "nocache",	 0x00010000	},
		{ "cache",		 0x00000002	},
		{ "resident",	 0x00000003 },
		{ "delayload",	 0x00000004	},
		{ "delayload_w", 0x00000005	},
		{ "sound",		 0x00000010 }
	};
};