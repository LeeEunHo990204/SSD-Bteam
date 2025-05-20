#include "TestScripts.h"

void TestScripts1::runTestScenario() {
	for (int i = 0; i < MAX_LBA_NUM; i++) {
		getShellDev()->write(i, i);
		readCompare(i, i);
	}
}

void TestScripts2::runTestScenario() {
	int loopCnt = 0;

	for (loopCnt = 0; loopCnt < 30; loopCnt++) {
		getShellDev()->write(4, 0x400);
		readCompare(4, 0x400);
		getShellDev()->write(0, 0x000);
		readCompare(0, 0x000);
		getShellDev()->write(3, 0x300);
		readCompare(3, 0x300);
		getShellDev()->write(1, 0x100);
		readCompare(1, 0x100);
		getShellDev()->write(2, 0x200);
		readCompare(2, 0x200);
	}
}
static uint32_t getRandomUint32() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);
	return dist(gen);
}

void TestScripts3::runTestScenario() {
	int loopCnt = 0;

	uint32_t randomVal;

	//TODO: Need to meet requirements
	for (loopCnt = 0; loopCnt < 200; loopCnt++) {
		randomVal = getRandomUint32();
		getShellDev()->write(0, randomVal);
		readCompare(0, randomVal);
		randomVal = getRandomUint32();
		getShellDev()->write(99, randomVal);
		readCompare(99, randomVal);
	}
}
