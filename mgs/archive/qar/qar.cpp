#include "qar.h"

Qar::Qar(std::string filename) {
	this->filename = filename;
}

Qar::~Qar() {

}

void Qar::open() {
	std::ifstream qarDat;
	qarDat.open(filename, std::ios::binary);

	qarDat.seekg(-4, qarDat.end);
	qarDat.read((char*)&header.offset, 4);

	int32_t offset = _byteswap_ulong(header.offset);

	qarDat.seekg(offset, qarDat.beg);
	qarDat.read((char*)&header.numFiles, 2);
	qarDat.read((char*)&header.pad, 2);

	int16_t numFiles = _byteswap_ushort(header.numFiles);

	header.dataInfo.resize(numFiles);
	qarDat.read((char*)&header.dataInfo[0], numFiles * sizeof(QarDataInfo));	

	header.filenames.resize(numFiles);
	for (int i = 0; i < numFiles; i++) {
		std::getline(qarDat, header.filenames[i], '\0');
	}

	qarDat.close();
}

void Qar::extractAll(std::string output) {
	updateDir("Qar", output);

	std::ifstream qarDat;
	qarDat.open(filename, std::ios::binary);
	int16_t numFiles = _byteswap_ushort(header.numFiles);

	for (int i = 0; i < numFiles; i++) {
		int32_t size = _byteswap_ulong(header.dataInfo[i].fileSize);
		
		uint8_t* txn = new uint8_t[size];
		qarDat.read((char*)txn, size);

		writeDataToFile(txn, size, header.filenames[i], output);

		delete[] txn;

		int align = getAlignment(qarDat.tellg(), 0x80);
		qarDat.seekg(align, qarDat.cur);
	}

	qarDat.close();
}