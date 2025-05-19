#include "gmock/gmock.h"
#include "Shell.cpp"
using namespace testing;

class MockCmdLauncher : public ICmdLauncher {
public:
	//MOCK_METHOD(void, write, (int LBA, unsigned int val), (override));
	//MOCK_METHOD(unsigned int, read, (int LBA), (override));
	void write(int LBA, unsigned int val) override {
		array[LBA] = val;
	}
	unsigned int read(int LBA) override {
		return array[LBA];
	}
private:
	unsigned int array[100] = { 0, };
};

class ShellFixture : public Test {

public:
	Shell* shell = new Shell(new MockCmdLauncher, new TestScripts1());
	MockCmdLauncher* cmdLauncher = new MockCmdLauncher();
};

TEST(Command, disabled_SpaceCommand) {
	std::vector<std::string> words = { "\n" };
	Command* command = new Command(words);
}

TEST_F(ShellFixture, invalid_cmd) {
	std::string actual = shell->runOneCommand("INVALID");
	EXPECT_EQ("INVALID COMMAND\n", actual);
}

TEST_F(ShellFixture, invalid_params_write) {
	std::string actual = shell->runOneCommand("WRITE 1");
	EXPECT_EQ("INVALID COMMAND\n", actual);
}

TEST_F(ShellFixture, invalid_params_read) {
	std::string actual = shell->runOneCommand("READ");
	EXPECT_EQ("INVALID COMMAND\n", actual);
}

TEST_F(ShellFixture, invalid_params_fullwrite) {
	
}

TEST_F(ShellFixture, write) {
	std::string actual = shell->runOneCommand("WRITE 1 0x12345678");
	EXPECT_EQ("[WRITE] Done\n", actual);
}

TEST_F(ShellFixture, read) {
	std::string actual = shell->runOneCommand("READ 1");
	EXPECT_EQ("[READ] 1 : 0\n", actual);
}