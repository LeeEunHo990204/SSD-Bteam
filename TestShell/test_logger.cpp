#include "gmock/gmock.h"
#include "Logger.cpp"
#include "CmdLancher.h"

class MockLauncher : public ICmdLauncher {
public:
	MOCK_METHOD(void, write, (int LBA, unsigned int val), (override));
	MOCK_METHOD(string, read, (int LBA), (override));
};

TEST(Logger, DISABLED_TestPrintLog)
{
	Logger::getInstance().print("test", "test");
}

TEST(Logger, DISABLED_TestPrintLog2Files)
{
	for (int i = 0; i < 200; i++) {
		Logger::getInstance().print("test" + to_string(i), "test");
	}
}

TEST(Logger, TestPrintLogOver3Files)
{
	for (int i = 0; i < 1000; i++) {
		Logger::getInstance().print("test" + to_string(i), "test");
	}
}