#include "TestScripts.h"

void TestScripts1::setTestScenario() {

}

void TestScripts1::runTestScenario() {
	for (int i = 0; i < MAX_LBA_NUM; i++) {
		readCompare(i, i);
	}
}
