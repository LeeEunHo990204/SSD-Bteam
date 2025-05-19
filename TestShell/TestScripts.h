#pragma once
#include <iostream>
#include "cmd_launcher.cpp"

#define READ_COMPARE_DATA_MISMATCH (-1)
#define READ_COMPARE_DATA_MATCH (0)

class ITestScripts
{
public:
	ITestScripts() : scriptName(""), testResult(0) {}
	ITestScripts(std::string name) : scriptName(name), testResult(0) {}
	ITestScripts(std::string name, ICmdLauncher* cmdLauncher) : scriptName(name) {
		testResult = 0;
		cmdLauncer = new ShellDeviceDriver(cmdLauncher);
	}

	int readCompare(int lba, int data) {
		getShellDev()->write(lba, data);

		if (data != getShellDev()->read(lba)) {
			return READ_COMPARE_DATA_MISMATCH; //ERROR: data mismatch
		}
		else {
			return READ_COMPARE_DATA_MATCH; //OK: data match
		}
	}
	virtual void setTestScenario() = 0;
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
	
	void setTestScenario() override;
	void runTestScenario() override;
};
