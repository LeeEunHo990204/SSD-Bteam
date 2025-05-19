#pragma once
#include <iostream>

class ITestScripts
{
public:
	ITestScripts() : scriptName(""), testResult(0) {}
	ITestScripts(std::string name) : scriptName(name), testResult(0) {}

	virtual int readCompare(int lba, int data) = 0;
	virtual void setTestScenario() = 0;
	virtual void runTestScenario() = 0;
	
	virtual int getResult() = 0;
	virtual std::string getName() = 0;
	
protected:
	std::string scriptName;
	int testResult;			// 0: pass, 1: fail
};

class TestScripts1 : public ITestScripts
{
public:
	TestScripts1() : ITestScripts("TestScript1") {}
	TestScripts1(std::string name) : ITestScripts(name) {}
	int readCompare (int lba, int data) override;
	void setTestScenario() override;
	void runTestScenario() override;
	int getResult() override;
	std::string getName() override;
};
