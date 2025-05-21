#include "gmock/gmock.h"
#include "Shell.h"
using namespace testing;

class MockCmdLauncher : public ICmdLauncher {
public:
	//MOCK_METHOD(void, write, (int LBA, unsigned int val), (override));
	//MOCK_METHOD(unsigned int, read, (int LBA), (override));
	void write(int LBA, unsigned int val) override {
		array[LBA] = val;
	}
	std::string read(int LBA) override {
		std::stringstream ss;
		ss << std::hex << std::uppercase << array[LBA];
		return ss.str();
	}
	bool erase(int LBA, int size) override {
		if (LBA < 0 || LBA > STORAGE_SIZE) return false;
		for (int i = LBA; i < LBA + size; i++) {
			array[i] = 0;
		}
		return true;
	}
	void flush() override {
		;
	}
private:
	unsigned int array[STORAGE_SIZE] = { 0, };
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
	std::string actual = shell->runCommand("INVALID");
	EXPECT_EQ("INVALID COMMAND", actual);
}

TEST_F(ShellFixture, invalid_params_write) {
	std::string actual = shell->runCommand("WRITE 1");	
	EXPECT_EQ("INVALID COMMAND", actual);
}

TEST_F(ShellFixture, invalid_params_read) {
	std::string actual = shell->runCommand("READ");
	EXPECT_EQ("INVALID COMMAND", actual);
}

TEST_F(ShellFixture, invalid_params_fullwrite) {
	std::string actual = shell->runCommand("FULLWRITE 0x12345678");
	EXPECT_EQ("[FullWrite] Done", actual);
}

TEST_F(ShellFixture, write) {
	std::string actual = shell->runCommand("WRITE 1 0x12345678");
	EXPECT_EQ("[Write] Done", actual);
}

TEST_F(ShellFixture, read) {
	std::string actual = shell->runCommand("READ 1");
	EXPECT_EQ("[Read] LBA 1 : 0", actual);
}

TEST_F(ShellFixture, erase) {
	std::string actual = shell->runCommand("ERASE 1 5");
	EXPECT_EQ("[Erase] Done", actual);
}

TEST_F(ShellFixture, flush) {
	std::string actual = shell->runCommand("FLUSH");
	EXPECT_EQ("[Flush] Done", actual);
}

TEST_F(ShellFixture, flush_invalid_arguments) {
	std::string actual = shell->runCommand("FLUSH 1");
	EXPECT_EQ("INVALID COMMAND", actual);
}

TEST_F(ShellFixture, testscripts1) {
	std::string actual = shell->runCommand("1_");
	EXPECT_EQ("PASS", actual);
}

TEST_F(ShellFixture, testscripts4) {
	std::string actual = shell->runCommand("4_");
	EXPECT_EQ("PASS", actual);
}