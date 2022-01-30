#include "dar.h"

Dar::Dar(std::string filename) {
	this->filename = filename;
	setType(ARCHIVE_DAR);
}

Dar::~Dar() {

}

void Dar::open() {
	std::ifstream darDat;
	darDat.open(filename, std::ios::binary);
	darDat.read((char*)&this->numFiles, 4);
	darDat.close();
}

void Dar::addFiles() {
	for (const auto& file : std::filesystem::directory_iterator(filename)) {
		DarEntry entry;
		std::string fname = file.path().u8string();

		entry.filename = getCurrentDir(fname);
		entry.size = file.file_size();

		entry.data.resize(entry.size);
		bufferFromFile(fname, entry.data.data(), entry.size);
		files.push_back(entry);
		numFiles++;
	}
}

void Dar::pack(std::string output) {
	addFiles();
	std::string outputFile = "cache.dar";
	int darNumFiles = _byteswap_ulong(numFiles);
	appendDataToFile((uint8_t*)&darNumFiles, 4, outputFile, output);

	for (int i = 0; i < numFiles; i++) {
		appendStringToFile(files[i].filename, outputFile, output);

		updateDir(outputFile, output);
			int rem = getAlignment(getFileSize(output), 4);
		resetDir(output);

		appendPaddingToFile(rem, outputFile, output);

		uint32_t fsize = _byteswap_ulong(files[i].size);
		appendDataToFile((uint8_t*)&fsize, 4, outputFile, output);

		updateDir(outputFile, output);
			rem = getAlignment(getFileSize(output), 16);
		resetDir(output);

		appendPaddingToFile(rem, outputFile, output);

		appendDataToFile(files[i].data.data(), files[i].size, outputFile, output);
		appendPaddingToFile(1, outputFile, output);
	}
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