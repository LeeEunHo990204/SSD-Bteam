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
	MockLauncher mockLauncher;
};

TEST_F(LoggerFixture, TestPrintLog)
{
	logger = new Logger();
	logger->print("test", "test");
}