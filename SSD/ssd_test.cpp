#include "gmock/gmock.h"
#include "buffer.h"
#include "command_parser.h"
#include "mock.h"
#include "device_controller.h"
#include "buffer.h"

using namespace testing;
using namespace std;



class SsdTestFixture : public Test {
private:
	void SetUp(void) {
		ssd.init();
		buffer.ssd.init();
	}
	void TearDown(void) {
	}
public:
	SSD ssd;
	Buffer buffer;
};

TEST(MockTest, write) {
	MockDevice device;
	EXPECT_CALL(device, write(0, 0));
	device.write(0, 0);
}

TEST(MockTest, read) {
	MockDevice device;
	EXPECT_CALL(device, read(0));
	device.read(0);
}

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

TEST_F(SsdTestFixture, eraseTrue) {
	ssd.write(0, 0x5678);
	EXPECT_EQ(0x5678, ssd.read(0));
	EXPECT_TRUE(ssd.erase(0, 10));
	EXPECT_EQ(0x0, ssd.read(0));
}

TEST_F(SsdTestFixture, eraseFalse) {
	EXPECT_FALSE(ssd.erase(0, -1));
}

TEST(SsdTest, ValidReadCommandParsing)
{
	std::vector<std::string> args = { "SSD.exe", "R", "1"};
	std::vector<char*> argv;
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
	CommandParser parser(argv.size(), argv.data());	
	CommandInfo commandInfo = parser.getCommandInfo();

	EXPECT_TRUE(parser.isValid());
	EXPECT_EQ(commandInfo.command, "R");
	EXPECT_EQ(commandInfo.address, 1);

}
TEST(SsdTest, InvalidLBAReadCommandParsing)
{
	std::vector<std::string> args = { "SSD.exe", "R", "1000" };
	std::vector<char*> argv;
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
	CommandParser parser(argv.size(), argv.data());
	CommandInfo commandInfo = parser.getCommandInfo();

	EXPECT_TRUE(parser.isValid());
	EXPECT_EQ(commandInfo.command, "R");
	EXPECT_EQ(commandInfo.address, 1000);

}
TEST(SsdTest, ValidWriteCommandParsing)
{
	std::vector<std::string> args = { "SSD.exe", "W", "1", "0xFFFFFFFF"};
	std::vector<char*> argv;
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
	CommandParser parser(argv.size(), argv.data());
	CommandInfo commandInfo = parser.getCommandInfo();

	EXPECT_TRUE(parser.isValid());
	EXPECT_EQ(commandInfo.command, "W");
	EXPECT_EQ(commandInfo.address, 1);
	EXPECT_EQ(commandInfo.data, 0xFFFFFFFF);

}

TEST(SsdTest, InvalidWriteCommandParsing)
{
	std::vector<std::string> args = { "SSD.exe", "W", "1", "0xFFFFFFFF123" };
	std::vector<char*> argv;
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
	CommandParser parser(argv.size(), argv.data());

	EXPECT_FALSE(parser.isValid());
}

TEST(SsdTest, ValidEraseCommandParsing)
{
	std::vector<std::string> args = { "SSD.exe", "E", "1", "10" };
	std::vector<char*> argv;
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
	CommandParser parser(argv.size(), argv.data());
	CommandInfo commandInfo = parser.getCommandInfo();

	EXPECT_TRUE(parser.isValid());
	EXPECT_EQ(commandInfo.command, "E");
	EXPECT_EQ(commandInfo.address, 1);
	EXPECT_EQ(commandInfo.size, 10);

}

TEST(SsdTest, InvalidEraseCommandParsing)
{
	std::vector<std::string> args = { "SSD.exe", "E", "1", "11" };
	std::vector<char*> argv;
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
	CommandParser parser(argv.size(), argv.data());

	EXPECT_TRUE(parser.isValid());		// command parsing에서는 valid check true로 나옴, ssd 에서 튕겨냄.
}


TEST_F(SsdTestFixture, DeviceControllerValidWriteAndReadTest)
{
	std::vector<std::string> args = { "SSD.exe", "W", "1", "12345678" };
	std::vector<char*> argv;
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));
	CommandParser parser(argv.size(), argv.data());
	CommandInfo commandInfo = parser.getCommandInfo();

	DeviceController deviceController(&buffer.ssd, &buffer);
	
	EXPECT_EQ(deviceController.run(commandInfo), 0);	// run 수행 결과 이상 없음 확인

	args = { "SSD.exe", "R", "1" };  
	argv.clear();
	for (auto& s : args) argv.push_back(const_cast<char*>(s.c_str()));

	CommandParser parser2(argv.size(), argv.data());
	commandInfo = parser.getCommandInfo();

	EXPECT_EQ(deviceController.run(commandInfo), 0);	// run 수행 결과 이상 없음 확인
}