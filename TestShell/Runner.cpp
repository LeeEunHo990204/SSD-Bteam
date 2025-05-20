#include "Runner.h"
#include <fstream>
#include <iostream>
#include <string>

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
		return -1;
	}

	this->scriptsFile.open(this->filePath);
	if (!this->scriptsFile.is_open()) {
		std::cout << "Failed to open file: " << filePath << endl;
		return -1;
	}

	while (std::getline(this->scriptsFile, line)) {
		if (line.compare("1_FullWriteAndReadCompare") == 0) {
			++this->scriptsNum;
			this->testScripts.push_back(new TestScripts1(line, &ssdLauncher));
		}
		else if (line.compare("2_PartialLBAWrite") == 0) {
			++this->scriptsNum;
			this->testScripts.push_back(new TestScripts2(line, &ssdLauncher));
		}
		else if (line.compare("3_WriteReadAging") == 0) {
			++this->scriptsNum;
			this->testScripts.push_back(new TestScripts3(line, &ssdLauncher));
		}
		else if (line.compare("4_EraseAndWriteAging") == 0) {
			++this->scriptsNum;
			//TODO: Need to implement
			//this->testScripts.push_back(new TestScripts3(line, &ssdLauncher));
		} 
		else {
			
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
			//script pass
			std::cout << "Pass";
		}
		else {
			//script fail
			std::cout << "Fail!" << std::endl;
			break;
		}
		std::cout << std::endl;
	}

	return ret;
}