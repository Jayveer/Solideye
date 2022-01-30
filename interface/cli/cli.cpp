#include "cli.h"

CLI::CLI(int argc, char** argv) {
	this->argc = argc;
	this->argv = argv;
}

CLI::~CLI() {

}

void CLI::processArgs() {
	for (currentArg = 1; currentArg < argc; currentArg++) {
		if (isOption(argv[currentArg])) processOption();
	}
}

void CLI::processOption() {
	const char* option = argv[currentArg];

	if (!strcmp(option, "-h") || !strcmp(option, "--help")) {
		printUsage();
		return;
	}

	if (!strcmp(option, "--version")) {
		printVersion();
		return;
	}

	if (!strcmp(option, "-e") || !strcmp(option, "--extract")) {
		command = EXTRACT;
		setFilename(argv[++currentArg]);
		return;
	}

	if (!strcmp(option, "-p") || !strcmp(option, "--pack")) {
		command = PACK;
		setFilename(argv[++currentArg]);
		return;
	}

	if (!strcmp(option, "-k") || !strcmp(option, "--key")) {
		setKey(argv[++currentArg]);
		return;
	}

	if (!strcmp(option, "-o") || !strcmp(option, "--output")) {
		setOutput(argv[++currentArg]);
		return;
	}

	if (!strcmp(option, "-enc") || !strcmp(option, "--encrypt")) {
		command = ENCRYPT;
		setFilename(argv[++currentArg]);
		return;
	}

	if (!strcmp(option, "-dec") || !strcmp(option, "--decrypt")) {
		command = DECRYPT;
		setFilename(argv[++currentArg]);
		return;
	}

	if (!strcmp(option, "-dict") || !strcmp(option, "--dictionary")) {
		loadDictionary(argv[++currentArg]);
		return;
	}

	if (!strcmp(option, "-f") || !strcmp(option, "--filetype")) {
		setFiletype(argv[++currentArg]);
		return;
	}

	printf("option %s not recognised\n", option);

}

void CLI::cliDefault() {
	setFilename(argv[1]);
	cliArchive(0);
}

void CLI::cliArchive(bool mode) {
	bool check = mode ? (checkFolderExists(inputFilename)) : (checkFileExists(inputFilename));
	if (!check) return;

	Archive* archive = archiveFactory(inputFilename);
	if (!archive) return;

	mode ? pack(archive) : extract(archive);
	archiveFree(archive);
}

void CLI::cliCrypt(bool mode) {
	std::string input = inputFilename;
	std::string outputFile = output == NULL ? "" : output;

	if (!checkFileExists(inputFilename)) return;
	if (!checkKeySet()) return;

	Crypto crypto(this->key);
	mode ? crypto.encrypt(input, outputFile) : crypto.decrypt(input, outputFile);
}


void CLI::processCommand(CLI_COMMAND command) {
	switch (command) {
	case UNSET:
		cliDefault();
		break;
	case EXTRACT:
		cliArchive(0);
		break;
	case PACK:
		cliArchive(1);
		break;
	case DECRYPT:
		cliCrypt(0);
		break;
	case ENCRYPT:
		cliCrypt(1);
		break;
	case EXIT:
		exit();
	}
}

Archive* CLI::archiveFactory(const char* filename) {
	std::string input = filename;

	if (isDar(input))   return new Dar(input);
	if (isQar(input))   return new Qar(input);
	if (isSlot(input))  return new Slot(input);
	if (isStage(input)) return new Stage(input);

	printf("Unsupported file type");
	return 0;
}

void CLI::archiveFree(Archive* archive) {
	switch (archive->getType()) {
	case ARCHIVE_DAR:
		delete (Dar*)archive;
		break;
	case ARCHIVE_QAR:
		delete (Qar*)archive;
		break;
	case ARCHIVE_SLOT:
		delete (Slot*)archive;
		break;
	case ARCHIVE_STAGE:
		delete (Stage*)archive;
		break;
	}
}

void CLI::extract(Archive* archive) {
	std::string outputFile = output == NULL ? "" : output;
	archive->open();
	archive->extractAll(outputFile);
}

void CLI::pack(Archive* archive) {
	std::string outputFile = output == NULL ? "" : output;
	archive->pack(outputFile);
}

void CLI::setFiletype(char* arg) {
	if (!strcmp(arg, "dar") || !strcmp(arg, "DAR")) {
		this->filetype = DAR;
		return;
	}

	if (!strcmp(arg, "qar") || !strcmp(arg, "QAR")) {
		this->filetype = QAR;
		return;
	}

	if (!strcmp(arg, "slot") || !strcmp(arg, "SLOT")) {
		this->filetype = SLOT;
		return;
	}

	if (!strcmp(arg, "stage") || !strcmp(arg, "STAGE")) {
		this->filetype = STAGE;
		return;
	}
}

void CLI::setOutput(char* arg) {
	this->output = arg;
}

void CLI::setFilename(char* arg) {
	this->inputFilename = arg;
}

void CLI::setKey(char* arg) {
	this->key = arg;
}

void CLI::setCommand(CLI_COMMAND command) {
	this->command = command;
}

void CLI::run(const char* programName, const char* version) {
	this->version = version;
	this->programName = programName;
	
	printWelcome();
	if (!checkInput()) return;
	processArgs();
	processCommand(this->command);
}

bool CLI::checkInput() {
	if (argc > 1 && argc < 10) return true;
	printUsage();
	return false;
}

bool CLI::checkKeySet() {
	if (this->key == NULL) {
		printf("Key not set, set key with -k. Usually this is the path to where the file resides");
		return 0;
	}
	return 1;
}

bool CLI::checkFolderExists(const char* foldername) {
	std::string input = foldername;

	if (!isDirectory(input)) {
		printf("directory %s, not found", foldername);
		return 0;
	}

	return 1;
}

bool CLI::checkFileExists(const char* filename) {
	std::string input = filename;

	if (!fileExists(input)) {
		printf("Input file %s, not found", filename);
		return 0;
	}

	return 1;
}

bool CLI::isStage(std::string& filepath) {
	if (this->filetype == STAGE) return true;
	if (this->filetype != NONE) return false;

	if (getExtension(filepath) == ".dat") {
		std::string filename = getExtensionlessName(filepath);
		if (filenameContainsString(filename, "stage"))
			return true;
	}

	return false;
}

bool CLI::isSlot(std::string& filepath) {
	if (this->filetype == SLOT) return true;
	if (this->filetype != NONE) return false;
	return getExtension(filepath) == ".slot";
}

bool CLI::isDar(std::string& filepath) {
	if (this->filetype == DAR) return true;
	if (this->filetype != NONE) return false;

	if (isDirectory(filepath)) {
		if (filenameContainsString(filepath, "Dar") || filenameContainsString(filepath, "dar"))
			return true;
	}

	return getExtension(filepath) == ".dar";
}

bool CLI::isQar(std::string& filepath) {
	if (this->filetype == QAR) return true;
	if (this->filetype != NONE) return false;

	if (isDirectory(filepath)) {
		if (filenameContainsString(filepath, "Qar") || filenameContainsString(filepath, "qar"))
			return true;
	}

	return getExtension(filepath) == ".qar";
}

bool CLI::isOption(char* arg) {
	return arg[0] == 0x2D;
}

void CLI::printWelcome() {
	printf("Running %s v%s: Visit https://github.com/Jayveer/Solideye for updates:\n", programName, version);
}

void CLI::printVersion() {
	setCommand(EXIT);
	printf("Version %s\n", version);
}

void CLI::printUsage() {
	setCommand(EXIT);
	printf(this->USAGE_MESSAGE);
}

void CLI::exit() {
	printf(this->EXIT_MESSAGE);
}
