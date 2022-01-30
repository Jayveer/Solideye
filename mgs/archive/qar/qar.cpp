#include "qar.h"

Qar::Qar(std::string filename) {
	this->filename = filename;
	setType(ARCHIVE_QAR);
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

void Qar::addFile(const std::filesystem::directory_entry& file, std::string& output) {
	int fsize = file.file_size();

	std::string fname = file.path().u8string();
	fname = getCurrentDir(fname);

	std::string name = file.path().stem().u8string();
	uint32_t strc = strcode(name.c_str()) ^ 0x03000000;

	header.filenames.push_back(fname);

	QarDataInfo qd;
	qd.fileInfo = strc;
	qd.fileSize = fsize;

	header.dataInfo.push_back(qd);

	int pad = getAlignment(fsize, 0x80);
	uint8_t* txn = bufferFromFile(file.path().u8string(), fsize, pad);
	appendDataToFile(txn, fsize, "cache.qar", output);
}

void Qar::addFiles(std::string& output) {
	for (const auto& file : std::filesystem::directory_iterator(filename)) {
		if (file.path().extension() == ".txn") {
			addFile(file, output);
		}
	}
}

void Qar::pack(std::string output) {
	addFiles(output);
	std::string outputFile = "cache.qar";
	header.numFiles = header.dataInfo.size();

	updateDir(outputFile, output);
		header.offset = getFileSize(output);
	resetDir(output);

	int datasize = header.numFiles * 8;

	genericSwap(&header.numFiles, 2, 2);
	appendDataToFile((uint8_t*)&header.numFiles, 2, outputFile, output);
	appendDataToFile((uint8_t*)&header.pad, 2, outputFile, output);

	genericSwap(header.dataInfo.data(), header.dataInfo.size() * 2, 4);
	appendDataToFile((uint8_t*)header.dataInfo.data(), datasize, outputFile, output);

	for (int i = 0; i < header.filenames.size(); i++) {
		appendStringToFile(header.filenames[i], outputFile, output);
	}

	updateDir(outputFile, output);
		int rem = getAlignment(getFileSize(output), 4);
	resetDir(output);
	appendPaddingToFile(rem, outputFile, output);

	genericSwap(&header.offset, 1, 4);
	appendDataToFile((uint8_t*)&header.offset, 4, outputFile, output);
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