#pragma once
#include <iostream>
#include <random>
#include <cstdint>
#include "CmdLancher.h"

#define READ_COMPARE_DATA_MISMATCH (-1)
#define READ_COMPARE_DATA_MATCH (0)
#define TEST_PASS (0)
#define TEST_FAIL (1)
#define MAX_LBA_NUM (100)

class ITestScripts
{
public:
	ITestScripts() : scriptName(""), testResult(TEST_PASS) {}
	ITestScripts(std::string name) : scriptName(name), testResult(TEST_PASS) {}
	ITestScripts(std::string name, ICmdLauncher* cmdLauncher) : scriptName(name) {
		testResult = 0;
		cmdLauncer = new ShellDeviceDriver(cmdLauncher);
	}

	int writeAndReadCompare(int lba, unsigned int data) {
		getShellDev()->write(lba, data);

		if (data != std::stoul(getShellDev()->read(lba), nullptr, 16)) {
			testResult = TEST_FAIL;
			return READ_COMPARE_DATA_MISMATCH; //ERROR: data mismatch
		}
		else {
			return READ_COMPARE_DATA_MATCH; //OK: data match
		}
	}

	int readCompare(int lba, int data) {

		if (data != std::stoul(getShellDev()->read(lba), nullptr, 16)) {
			testResult = TEST_FAIL;
			return READ_COMPARE_DATA_MISMATCH; //ERROR: data mismatch
		}
		else {
			return READ_COMPARE_DATA_MATCH; //OK: data match
		}
	}
	
	virtual void runTestScenario() = 0;

	int getResult() { return testResult; }
	std::string getName() { return scriptName; }
	ShellDeviceDriver* getShellDev() { return cmdLauncer; }
	
protected:
	std::string scriptName;
	int testResult;			// 0: pass, 1: fail

	ShellDeviceDriver* cmdLauncer;
};

class TestScripts1 : public ITestScripts
{
public:
	TestScripts1() : ITestScripts("TestScript1") {}
	TestScripts1(std::string name) : ITestScripts(name) {}
	TestScripts1(std::string name, ICmdLauncher* cmdLauncher) : ITestScripts(name, cmdLauncher) {}
	

	void runTestScenario() override;
};

class TestScripts2 : public ITestScripts
{
public:
	TestScripts2() : ITestScripts("TestScript2") {}
	TestScripts2(std::string name) : ITestScripts(name) {}
	TestScripts2(std::string name, ICmdLauncher* cmdLauncher) : ITestScripts(name, cmdLauncher) {}

	void runTestScenario() override;
};

class TestScripts3 : public ITestScripts
{
public:
	TestScripts3() : ITestScripts("TestScript3") {}
	TestScripts3(std::string name) : ITestScripts(name) {}
	TestScripts3(std::string name, ICmdLauncher* cmdLauncher) : ITestScripts(name, cmdLauncher) {}

	void runTestScenario() override;
};
