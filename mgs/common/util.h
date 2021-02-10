#pragma once
#include <sstream>

inline
std::string formatStrcode(std::string strcode) {
	uint8_t length = strcode.size();
	uint8_t pad = 6 - length;
	std::string prefix;

	for (int i = 0; i < pad; i++) {
		prefix += "0";
	}

	return (prefix + strcode);

}

inline
std::string intToHexString(int value) {
	std::stringstream ss;
	ss << std::hex << value;
	return formatStrcode(ss.str());
}