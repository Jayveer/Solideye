#pragma once

#include "../../mgs/archive/dar/dar.h"
#include "../../mgs/archive/qar/qar.h"
#include "../../mgs/archive/slot/stage/stage.h"

class CLI {
public:
	CLI(int argc, char** argv);
	~CLI();

	void run(std::string programName, std::string version);
	void exit();
private:
	int argc;
	char** argv;
	int currentArg = 1;

	void printUsage();
	bool checkInput();
	void processArgs();
	void extract(Archive* archive);

	const char* EXIT_MESSAGE = "Exiting\n";
	const char* USAGE_MESSAGE = "Usage:\t Solideye.exe <filename.slot | .dar | .qar | stagexx.dat> [OUTPUTDIRECTORY] \n";
};