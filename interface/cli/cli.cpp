#include "cli.h"

CLI::CLI(int argc, char** argv) {
	this->argc = argc;
	this->argv = argv;
}

CLI::~CLI() {

}

bool isStage(std::string& filepath) {
	if (getExtension(filepath) == ".dat") {
		std::string filename = getExtensionlessName(filepath);
		if (filename.find("stage") != std::string::npos)
			return true;
	}

	return false;
}

bool isSlot(std::string& filepath) {
	return getExtension(filepath) == ".slot";
}

bool isDar(std::string& filepath) {
	return getExtension(filepath) == ".dar";
}

bool isQar(std::string& filepath) {
	return getExtension(filepath) == ".qar";
}

void CLI::extract(Archive* archive) {
	std::string output = "";
	if (currentArg == argc - 1) output = argv[currentArg];

	archive->open();
	archive->extractAll(output);
}

void CLI::processArgs() {
	std::string input = argv[currentArg];
	currentArg++;

	if (isDar(input)) {
		Dar dar = Dar(input);
		extract(&dar);
		return;
	}

	if (isQar(input)) {
		Qar qar = Qar(input);
		extract(&qar);
		return;
	}

	if (isSlot(input)) {
		Slot slot = Slot(input);
		extract(&slot);
		return;
	}

	if (isStage(input)) {
		Stage stage = Stage(input);
		extract(&stage);
		return;
	}

	printf("Unsupported file type");
	exit();
}

bool CLI::checkInput() {
	if (argc > 1 && argc < 4) return true;
	printUsage();
	return false;
}

void CLI::run(std::string programName, std::string version) {
	printf("Running %s v%s: Visit https://github.com/Jayveer/Solideye for updates:\n", programName.c_str(), version.c_str());
	loadDictionary("dictionary.txt");
	if (!checkInput) return;
	processArgs();
}

void CLI::printUsage() {
	printf(this->USAGE_MESSAGE);
}

void CLI::exit() {
	printf(this->EXIT_MESSAGE);
}