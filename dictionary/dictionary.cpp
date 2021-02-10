#include "dictionary.h"
#include "../mgs/common/strcode.h"

std::map<unsigned int, std::string> FileNames;

void loadDictionary(const std::string& dictionary) {
	std::string str;
	std::ifstream file(dictionary);

	if (!file) return;

	while (std::getline(file, str)) {
		int hashed = strcode(str.c_str());
		FileNames[hashed] = str;
	}
}