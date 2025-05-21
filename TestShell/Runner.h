#pragma once

#include<iostream>
#include<fstream>
#include<string.h>

#include "TestScripts.h"
#include "CmdLauncher.h"

typedef enum RUNNER_STATUS {
	RUNNER_INIT = 0,
	RUNNER_RUNNING,
	RUNNER_DONE,
	RUNNER_MAX
};

typedef enum RUNNER_ERROR {
	RUNNER_INVALID_FILE_PATH = 1,
	RUNNER_SCRIPT_FILE_OPEN_FAILED = 2,
	RUNNER_PARSING_SCRIPTS_FAIL = 3,
	RUNNER_EXECUTION_SCRIPTS_FAIL = 4,
	RUNNER_FAIL_MAX
};

class Runner
{
public:
	Runner(std::string filePath) : filePath(filePath), scriptsNum(0) {

	}

	~Runner() {
		for (auto ptr : testScripts) {
			delete ptr;
		}
		testScripts.clear();
	}

	int parseInputScripts();
	int runScripts();

	std::string getFilePath() const {
		return filePath;
	}

	std::vector<ITestScripts*> getTestScripts() const {
		return testScripts;
	}

	int getScriptsNum() const {
		return scriptsNum;
	}

private:
	std::string filePath;
	std::ifstream scriptsFile;
	vector<ITestScripts*> testScripts;
	int scriptsNum;
	SSDCmdLauncher ssdLauncher;
};

