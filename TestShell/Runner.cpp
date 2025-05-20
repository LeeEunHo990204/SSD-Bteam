#include "Runner.h"
#include <fstream>

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
	this->scriptsNum = 4;

	if (isFilePathInvalid(this->filePath)) {
		return -1;
	}

	return 0;
}

int Runner::runScripts() {
	return 4;
}