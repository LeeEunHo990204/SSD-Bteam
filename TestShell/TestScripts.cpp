#include "TestScripts.h"

static uint32_t getRandomUint32() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);
	return dist(gen);
}

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

void TestScripts3::runTestScenario() {
	int loopCnt = 0;

	uint32_t randomVal;

	for (loopCnt = 0; loopCnt < 200; loopCnt++) {
		randomVal = getRandomUint32();
		getShellDev()->write(0, randomVal);
		readCompare(0, randomVal);
		randomVal = getRandomUint32();
		getShellDev()->write(99, randomVal);
		readCompare(99, randomVal);
	}
}

void TestScripts4::runTestScenario() {
	int loopCnt = 0;

	uint32_t randomVal;

	getShellDev()->erase(0, 3);

	for (loopCnt = 0; loopCnt < 30; loopCnt++) {

		for (uint32_t LBA = 2; LBA <= MAX_LBA - 2; LBA = LBA + 2)
		{
			randomVal = getRandomUint32();
			getShellDev()->write(0, randomVal);
			randomVal = getRandomUint32();
			getShellDev()->write(0, randomVal);
			getShellDev()->erase(LBA, 3); // LBA 2~4, 4~6 ...
		}
	}
}

void TestScripts5::runTestScenario() {
	int a;
	getShellDev()->flush();
	cin >> a;
	cin.ignore();
	cout << "erase 0 2" << endl;
	getShellDev()->erase(0, 2);
	cin >> a;
	cin.ignore();
	cout << "write 2 0x00000003" << endl;
	getShellDev()->write(2, 3);
	cin >> a;
	cin.ignore();
	cout << "erase 3 2" << endl;
	getShellDev()->erase(3, 2);
}

void TestScripts6::runTestScenario() {
	int a;
	getShellDev()->flush();
	cin >> a;
	cin.ignore();
	cout << "erase 0 10" << endl;
	getShellDev()->erase(0, 10);
	cin >> a;
	cin.ignore();
	cout << "erase 10 2" << endl;
	getShellDev()->erase(10, 2);
	cin >> a;
	cin.ignore();
	cout << "write 0 0x00000001" << endl;
	getShellDev()->write(0, 1);
	cin >> a;
	cin.ignore();
	cout << "write 1 0x00000002" << endl;
	getShellDev()->write(1, 2);
}