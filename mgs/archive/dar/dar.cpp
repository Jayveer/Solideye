#include "dar.h"

Dar::Dar(std::string filename) {
	this->filename = filename;
}

Dar::~Dar() {

}

void Dar::open() {
	std::ifstream darDat;
	darDat.open(filename, std::ios::binary);
	darDat.read((char*)&this->numFiles, 4);
	darDat.close();
}

void Dar::extractAll(std::string output) {
	updateDir("Dar", output);
	int32_t darNumFiles = _byteswap_ulong(this->numFiles);

	std::ifstream darDat;
	darDat.open(filename, std::ios::binary);
	darDat.seekg(4);

	for (int i = 0; i < darNumFiles; i++) {
		int align = 0;
		std::string name;
		std::getline(darDat, name, '\0');
		align = getAlignment(darDat.tellg(), 4);

		int32_t size;
		darDat.seekg(align, darDat.cur);
		darDat.read((char*)&size, 4);
		size = _byteswap_ulong(size);
		align = getAlignment(darDat.tellg(), 16);

		darDat.seekg(align, darDat.cur);
		uint8_t* file = new uint8_t[size];
		darDat.read((char*)file, size);
		writeDataToFile(file, size, name, output);

		delete[] file;
		darDat.seekg(1, darDat.cur);
	}
}