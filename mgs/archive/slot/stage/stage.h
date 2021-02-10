#pragma once
#include "../slot.h"

#pragma pack(push, 4)
struct StageTable {
	char name[8];
	uint64_t hash;
	int32_t offset;
};
#pragma pop

class Stage : public Slot {
public:
	Stage(std::string filename);
	Stage(std::string filename, uint32_t sector);
	~Stage();

	void open();
	void extractAll(std::string output = "");
	void extract(uint16_t pageID, std::string output = "");
private:
	std::vector<StageTable> table;
	int64_t getNextPageOffset(uint16_t pageID);
};