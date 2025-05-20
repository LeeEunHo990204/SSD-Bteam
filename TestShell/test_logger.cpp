#include "gmock/gmock.h"
#include "Logger.cpp"
#include "CmdLancher.h"

class MockLauncher : public ICmdLauncher {
public:
	MOCK_METHOD(void, write, (int LBA, unsigned int val), (override));
	MOCK_METHOD(string, read, (int LBA), (override));
};

class LoggerFixture : public ::testing::Test
{
public:
	Logger* logger;
	MockLauncher* mockLauncher;
};

TEST_F(LoggerFixture, DISABLED_TestPrintLog)
{
	logger = new Logger();
	logger->print("test", "test");
}

TEST_F(LoggerFixture, DISABLED_TestPrintLog2Files)
{
	logger = new Logger();
	for (int i = 0; i < 200; i++) {
		logger->print("test" + to_string(i), "test");
	}
}

TEST_F(LoggerFixture, TestPrintLogOver3Files)
{
	logger = new Logger();
	for (int i = 0; i < 1000; i++) {
		logger->print("test" + to_string(i), "test");
	}
}