#pragma once
#include <iostream>

class ITestScripts
{
public:
	virtual int readCompare(int lba, int data) = 0;
	virtual void setTestScenario() = 0;
	virtual void runTestScenario() = 0;
	virtual int getResult() = 0;

private:
	std::string scriptName;
	int testResult;			// 0: pass, 1: fail
};

class TestScripts1 : public ITestScripts
{
public:
	int readCompare (int lba, int data) override;
	void setTestScenario() override;
	void runTestScenario() override;
	int getResult() override;
};
