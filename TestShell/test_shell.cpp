#include "gmock/gmock.h"
#include "Shell.cpp"
using namespace testing;

class MockShell : public Shell {
public:
    //MOCK_METHOD(void, write, (int LBA, unsigned int val), (override));
    //MOCK_METHOD(unsigned int, read, (int LBA), (override));
    std::string runOneCommand(std::string cmdLine) {
		std::vector<std::string> words = splitBySpace(cmdLine);
		Command* command = new Command(words);
		if (command->getValid() == false) {
			return "INVALID COMMAND";
		}

		if (command->cmd == "write" || command->cmd == "WRITE") {
			return "WRITE";
		}

		else if (command->cmd == "read" || command->cmd == "READ") {
			return "READ";
		}

		else if (command->cmd == "exit") {
			return "EXIT";
		}
    }
private:
	std::vector<std::string> splitBySpace(const std::string& input) {
		std::istringstream iss(input);
		std::vector<std::string> result;
		std::string word;

		if (input.empty()) {
			return result;
		}

		while (iss >> word) {
			result.push_back(word);
		}

		return result;
	}
};

class ShellFixture : public Test {

public:
	MockShell  mockShell;
};

TEST(Command, SpaceCommand) {
	std::vector<std::string> words = { "\n" };
	Command* command = new Command(words);
}

TEST_F(ShellFixture, invalid_cmd) {
	std::string actual = mockShell.runOneCommand("INVALID");
	EXPECT_EQ("INVALID COMMAND", actual);
}

TEST_F(ShellFixture, invalid_params_write) {
	std::string actual = mockShell.runOneCommand("WRITE 1");
	EXPECT_EQ("INVALID COMMAND", actual);
}

TEST_F(ShellFixture, invalid_params_read) {
	std::string actual = mockShell.runOneCommand("READ");
	EXPECT_EQ("INVALID COMMAND", actual);
}

TEST_F(ShellFixture, write) {
	std::string actual = mockShell.runOneCommand("WRITE 1 0x12345678");
	EXPECT_EQ("WRITE", actual);
}

TEST_F(ShellFixture, read) {
	std::string actual = mockShell.runOneCommand("READ 1");
	EXPECT_EQ("READ", actual);
}