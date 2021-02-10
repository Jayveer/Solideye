#pragma once

#include <map>
#include <string>
#include <fstream>

extern std::map<unsigned int, std::string> FileNames;
extern void loadDictionary(const std::string& dictionary);