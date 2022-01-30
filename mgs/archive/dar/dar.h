#pragma once
#include "../archive.h"

struct DarEntry {
	std::string filename;
	uint32_t size;
	std::vector<uint8_t> data;
};

class Dar : public Archive{
public:
	Dar(std::string filename);
	~Dar();

	void open();
	int32_t getNumFiles();
	void pack(std::string output = "");
	void extractAll(std::string output = "");
	std::vector<uint8_t> getFile(int32_t i);
private:
	int32_t numFiles = 0;
	std::vector<DarEntry> files;
	std::string filename = "cache.dar";

	void addFiles();
};