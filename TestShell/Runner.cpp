#include "Runner.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <functional>

static int isFilePathInvalid(std::string filePath) {
	
	if (filePath == "" || filePath.empty()) {
		return 1;
	}

	std::ifstream script(filePath);
	if (script.good()) {
		return 0;
	}
	else {
		return 1;
	}

	return 0;
}

int Runner::parseInputScripts() {

	int lineNumber = 1;
	
	std::string line;

	if (isFilePathInvalid(this->filePath)) {
		return -RUNNER_INVALID_FILE_PATH;
	}

	this->scriptsFile.open(this->filePath);
	if (!this->scriptsFile.is_open()) {
		std::cout << "Failed to open file: " << filePath << endl;
		return -RUNNER_SCRIPT_FILE_OPEN_FAILED;
	}

	// 스크립트 이름과 생성자 매핑
	const std::map<std::string, std::function<ITestScripts* (const std::string&, ICmdLauncher*)>> scriptFactory = {
		{ "1_FullWriteAndReadCompare", [](const std::string& name, ICmdLauncher* launcher) { return new TestScripts1(name, launcher); } },
		{ "2_PartialLBAWrite",         [](const std::string& name, ICmdLauncher* launcher) { return new TestScripts2(name, launcher); } },
		{ "3_WriteReadAging",          [](const std::string& name, ICmdLauncher* launcher) { return new TestScripts3(name, launcher); } },
		{ "4_EraseAndWriteAging",      [](const std::string& name, ICmdLauncher* launcher) { return new TestScripts4(name, launcher); } }
	};

	while (std::getline(this->scriptsFile, line)) {
		auto it = scriptFactory.find(line);
		if (it != scriptFactory.end()) {
			++this->scriptsNum;
			this->testScripts.push_back(it->second(line, &ssdLauncher));
		}
		else {
			std::cout << "Invalid script name: " << line << std::endl;
		}
	}

	this->scriptsFile.close();

	return 0;
}

int Runner::runScripts() {

	int ret = 0;

	for (auto scriptPtr : this->getTestScripts()) {

		std::cout << std::left << std::setw(40) << scriptPtr->getName() << " ___ Run...";
		scriptPtr->runTestScenario();
		if (scriptPtr->getResult() == 0) {
			std::cout << "Pass";
		}
		else {
			std::cout << "Fail!" << std::endl;
			break;
		}
		std::cout << std::endl;
	}

	return ret;
}