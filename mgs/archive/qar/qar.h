#pragma once
#include "../archive.h"
#include "../../common/strcode.h"

struct QarDataInfo {
	uint32_t fileInfo;
	uint32_t fileSize;
};

struct QarHeader {
	int16_t numFiles;
	uint16_t pad = 0;
	std::vector<QarDataInfo> dataInfo;
	std::vector<std::string> filenames;
	int32_t offset;
};

class Qar : public Archive {
public:
	Qar(std::string filename);
	~Qar();

	void open();
	void pack(std::string output = "");
	void extractAll(std::string output = "");
private:
	QarHeader header;
	std::string filename = "cache.qar";

	void addFiles(std::string& output);
	void addFile(const std::filesystem::directory_entry& file, std::string& output);
};