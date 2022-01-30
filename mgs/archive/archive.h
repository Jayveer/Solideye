#pragma once

#include <string>
#include "../common/fileutil.h"

enum ArchiveType {
	ARCHIVE_DAR,
	ARCHIVE_QAR,
	ARCHIVE_SLOT,
	ARCHIVE_STAGE
};

class Archive {
public:
	virtual void open() = 0;
	ArchiveType getType() { return type; }
	virtual void pack(std::string output = "") = 0;
	virtual void extractAll(std::string output = "") = 0;
protected:
	ArchiveType type;
	virtual void setType(ArchiveType type) { this->type = type; }
};