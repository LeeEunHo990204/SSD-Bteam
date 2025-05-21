#if _DEBUG

#include "gmock/gmock.h"
#include "buffer.h"

using namespace testing;

class BufferTest : public Test {
public:
	SSD& ssd = SSD::getInstance();
	Buffer buffer{ ssd };
};

TEST_F(BufferTest, initBuffer) {
	buffer.initBuffer();
}

TEST_F(BufferTest, convertCmdToTxt) {
	buffer.cmdBuffer.clear();
	buffer.cmdBuffer.push_back({ "W", 50, 0xABDC });
	buffer.cmdBuffer.push_back({ "E", 40, 4 });
	buffer.cmdBuffer.push_back({ "W", 60, 0xBABA });
	buffer.cmdBuffer.push_back({ "", 0, 0 });
	buffer.cmdBuffer.push_back({ "", 0, 0 });

	buffer.convertCmdToTxt();

	buffer.printTxtBuffer();
}

TEST_F(BufferTest, convertTxtToCmd) {
	buffer.cmdBuffer.clear();
	buffer.cmdBuffer.push_back({ "W", 50, 0xABDC });
	buffer.cmdBuffer.push_back({ "E", 40, 4 });
	buffer.cmdBuffer.push_back({ "W", 60, 0xBABA });
	buffer.cmdBuffer.push_back({ "", 0, 0 });
	buffer.cmdBuffer.push_back({ "", 0, 0 });

	buffer.convertCmdToTxt();

	buffer.convertTxtToCmd();
	buffer.printCmdBuffer();
}

TEST_F(BufferTest, writeBuffer) {
	buffer.cmdBuffer.clear();
	buffer.cmdBuffer.push_back({ "W", 50, 0xABDC });
	buffer.cmdBuffer.push_back({ "E", 40, 4 });
	buffer.cmdBuffer.push_back({ "W", 60, 0xBABA });
	buffer.cmdBuffer.push_back({ "", 0, 0 });
	buffer.cmdBuffer.push_back({ "", 0, 0 });
	buffer.convertCmdToTxt();

	buffer.writeBuffer();
}

TEST_F(BufferTest, readBuffer) {
	buffer.cmdBuffer.clear();
	buffer.cmdBuffer.push_back({ "W", 50, 0xABDC });
	buffer.cmdBuffer.push_back({ "E", 40, 4 });
	buffer.cmdBuffer.push_back({ "W", 60, 0xBABA });
	buffer.cmdBuffer.push_back({ "E", 10, 10 });
	buffer.cmdBuffer.push_back({ "W", 30, 30 });
	buffer.convertCmdToTxt();
	buffer.writeBuffer();

	buffer.readBuffer();

	buffer.printCmdBuffer();
}

#endif