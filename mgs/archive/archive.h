#pragma once

#include <string>
#include "../common/fileutil.h"

class Archive {
public:
	virtual void open() = 0;
	virtual void extractAll(std::string output = "") = 0;
};