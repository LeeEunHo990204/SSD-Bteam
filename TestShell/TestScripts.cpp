#include "TestScripts.h"


int TestScripts1::readCompare(int lba, int data) {

	getShellDev()->write(lba, data);
	unsigned int readData = getShellDev()->read(lba);

	if (data != readData) {
		return 1; //ERROR: data mismatch
	}
	else {
		return 0; //OK: data match
	}

	return 0;
}

void TestScripts1::setTestScenario() {

}

void TestScripts1::runTestScenario() {

}

int TestScripts1::getResult() {
	return testResult;
}

std::string TestScripts1::getName() {
	return scriptName;
}