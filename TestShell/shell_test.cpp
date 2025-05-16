#include "gmock/gmock.h"
#include "Shell.cpp"
using namespace testing;

TEST(Command, SpaceCommand) {
	std::vector<std::string> words = { "\n" };
	Command* command = new Command(words);
	///tesetteste
}