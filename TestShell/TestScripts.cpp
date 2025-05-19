#include "TestScripts.h"


int TestScripts1::readCompare(int lba, int data) {

	getShellDev()->write(lba, data);

	if (data != getShellDev()->read(lba)) {
		return READ_COMPARE_DATA_MISMATCH; //ERROR: data mismatch
	}
	else {
		return READ_COMPARE_DATA_MATCH; //OK: data match
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