#pragma once

#include "../../mgs/crypto/crypto.h"
#include "../../mgs/archive/dar/dar.h"
#include "../../mgs/archive/qar/qar.h"
#include "../../mgs/archive/slot/stage/stage.h"

typedef enum CLI_COMMAND {
	EXIT,
	EXTRACT,
	PACK,
	ENCRYPT,
	DECRYPT,
	UNSET
} CLI_COMMAND;

typedef enum CLI_FILETYPE {
	DAR,
	QAR,
	SLOT,
	STAGE,
	NONE
} CLI_FILETYPE;

class CLI {
public:
	CLI(int argc, char** argv);
	~CLI();

	void run(const char* programName, const char* version);
	void exit();
private:
	int argc;
	char** argv;
	int currentArg;

	const char*	 key;
	const char*  output;
	const char*  inputFilename;
	CLI_FILETYPE filetype = NONE;
	CLI_COMMAND  command = UNSET;

	void execute();
	void printUsage();
	bool checkInput();
	bool checkKeySet();
	void processArgs();
	void processFile();
	void printVersion();
	void printWelcome();
	void processOption();
	bool isOption(char* arg);
	void processCommand(CLI_COMMAND command);
	bool checkFileExists(const char* filename);
	bool checkFolderExists(const char* foldername);
	
	void setKey(char* arg);
	void setOutput(char* arg);
	void setFiletype(char* arg);
	void setOption(char* arg);
	void setFilename(char* arg);

	void setCommand(CLI_COMMAND command);

	void cliDefault();
	void cliCrypt(bool mode = 0);
	void cliArchive(bool mode = 0);

	void pack(Archive* archive);
	void extract(Archive* archive);
	bool isQar(std::string& filepath);
	bool isDar(std::string& filepath);
	bool isSlot(std::string& filepath);
	bool isStage(std::string& filepath);

	void archiveFree(Archive* archive);
	Archive* archiveFactory(const char* filename);

	const char* version;
	const char* programName;
	const char* EXIT_MESSAGE = "Exiting\n";
	const char* USAGE_MESSAGE = "USAGE:\t	help\n \
	\tSolideye.exe [-h | --help]\n \
	\n \
	\tversion\n \
	\tSolideye.exe [--version]\n \
	\n \
	\textract\n \
	\tSolideye.exe [-e   | --extract] <file> [-f | --filetype] <filetype> [-dict --dictionary] <dictionaryfile> [-o | --output] <outputdirectory>\n	\
	\n \
	\tpack\n \
	\tSolideye.exe [-p   | --pack   ] <file> [-f | --filetype] <filetype> [-o | --output] <outputdirectory>\n \
	\n \
	\tencrypt\n \
	\tSolideye.exe [-enc | --encrypt] <file> [-k | --key] <key> [-o | --output] <outputdirectory>\n \
	\n \
	\tdecrypt\n \
	\tSolideye.exe [-dec | --decrypt] <file> [-k | --key] <key> [-o | --output] <outputdirectory>\n";
};