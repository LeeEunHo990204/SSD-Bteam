#pragma once

#include<iostream>
#include<fstream>
#include<string.h>

#include "TestScripts.h"

typedef enum RUNNER_STATUS {
	RUNNER_INIT = 0,
	RUNNER_RUNNING,
	RUNNER_DONE,
	RUNNER_MAX
};

class Runner
{
public:
	Runner(std::string filePath) : filePath(filePath), testScripts(nullptr), scriptsNum(0) {
	}
	~Runner() {
		if (testScripts) {
			delete testScripts;
		}
	}

	int parseInputScripts();
	int runScripts();

	std::string getFilePath() const {
		return filePath;
	}

	ITestScripts** getTestScripts() const {
		return testScripts;
	}

	int getScriptsNum() const {
		return scriptsNum;
	}

private:
	std::string filePath;
	std::ifstream scriptsFile;
	ITestScripts** testScripts;
	int scriptsNum;
};

