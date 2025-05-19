#include "gmock/gmock.h"
#include "ssd.h"
#include "command_parser.h"

using namespace testing;
using namespace std;



TEST(SsdTest, constructor) {
	SSD ssd;
}

TEST(SsdTest, writeTrue) {
	SSD ssd;
	ssd.write(0, 0x123);
	ssd.write(60, 0x456);
}

TEST(SsdTest, writeFalse) {
	SSD ssd;
	ssd.write(0, 0x123);
}

TEST(SsdTest, readTrue) {
	SSD ssd;
	ssd.read(0);
}

TEST(SsdTest, readFalse) {
	SSD ssd;
	ssd.read(100);
}

TEST(SsdTest, readWriteTrue) {
	SSD ssd;
	ssd.write(0, 0x123);
	EXPECT_EQ(0x123, ssd.read(0));
}

TEST(SsdTest, readWriteFalse) {
	SSD ssd;
	ssd.write(1, 0x123);
	EXPECT_NE(0x456, ssd.read(1));
}

TEST(SsdTest, writeDuplicateTrue) {
	SSD ssd;
	ssd.write(0, 0x12345678);
	ssd.write(50, 0x5678);
	ssd.write(0, 0x5678);
	EXPECT_EQ(0x5678, ssd.read(0));
}

TEST(SsdTest, ValidReadCommandParsing)
{
	std::vector<std::string> args = { "SSD.exe", "R", "1"};
	std::vector<char*> argv;
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
	CommandParser parser(argv.size(), argv.data());

	EXPECT_TRUE(parser.isValid());
	EXPECT_EQ(parser.getCommand(), "R");
	EXPECT_EQ(parser.getLBA(), 1);

}
TEST(SsdTest, InvalidLBAReadCommandParsing)
{
	std::vector<std::string> args = { "SSD.exe", "R", "1000" };
	std::vector<char*> argv;
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
	CommandParser parser(argv.size(), argv.data());

	EXPECT_TRUE(parser.isValid());
	EXPECT_EQ(parser.getCommand(), "R");
	EXPECT_EQ(parser.getLBA(), 1000);

}
TEST(SsdTest, ValidWriteCommandParsing)
{
	std::vector<std::string> args = { "SSD.exe", "W", "1", "0xFFFFFFFF"};
	std::vector<char*> argv;
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
	CommandParser parser(argv.size(), argv.data());

	EXPECT_TRUE(parser.isValid());
	EXPECT_EQ(parser.getCommand(), "W");
	EXPECT_EQ(parser.getLBA(), 1);
	EXPECT_EQ(parser.getData(), 0xFFFFFFFF);

}

TEST(SsdTest, InvalidWriteCommandParsing)
{
	std::vector<std::string> args = { "SSD.exe", "W", "1", "0xFFFFFFFF123" };
	std::vector<char*> argv;
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
	CommandParser parser(argv.size(), argv.data());

	EXPECT_FALSE(parser.isValid());
}