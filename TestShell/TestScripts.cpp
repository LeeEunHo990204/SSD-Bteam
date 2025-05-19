#include "TestScripts.h"

void TestScripts1::runTestScenario() {
	for (int i = 0; i < MAX_LBA_NUM; i++) {
		readCompare(i, i);
	}
}

void TestScripts2::runTestScenario() {
	int loopCnt = 0;

	for (loopCnt = 0; loopCnt < 30; loopCnt++) {
		readCompare(4, 0x400);
		readCompare(0, 0x000);
		readCompare(3, 0x300);
		readCompare(1, 0x100);
		readCompare(2, 0x200);
	}
}

void TestScripts3::runTestScenario() {
	int loopCnt = 0;

	//TODO: Need to meet requirements
	for (loopCnt = 0; loopCnt < 200; loopCnt++) {
		readCompare(0, 0x10101010);
		readCompare(99, 0x5a5aa5a5);
	}
}
