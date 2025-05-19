#include "gmock/gmock.h"
#include "ssd.h"
#include "command_parser.h"

using namespace testing;
using namespace std;



class SsdTestFixture : public Test {
public:
	SSD ssd;
};

TEST_F(SsdTestFixture, writeTrue) {
	ssd.write(0, 0x123);
	ssd.write(60, 0x456);
}

TEST_F(SsdTestFixture, writeFalse) {
	ssd.write(0, 0x123);
}

TEST_F(SsdTestFixture, readTrue) {
	ssd.read(0);
}

TEST_F(SsdTestFixture, readFalse) {
	ssd.read(100);
}

TEST_F(SsdTestFixture, readWriteTrue) {
	ssd.write(0, 0x123);
	EXPECT_EQ(0x123, ssd.read(0));
}

TEST_F(SsdTestFixture, readWriteFalse) {
	ssd.write(1, 0x123);
	EXPECT_NE(0x456, ssd.read(1));
}

TEST_F(SsdTestFixture, writeDuplicateTrue) {
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