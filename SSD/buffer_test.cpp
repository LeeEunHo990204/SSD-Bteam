#if _DEBUG

#include "gmock/gmock.h"
#include "buffer.h"

using namespace testing;

class BufferTest : public Test {
public:
	Buffer buffer;
    void SetUp() override {
        buffer.initBuffer();
    }
    void TearDown() override {
        buffer.resetBuffer();
        buffer.writeBuffer();
    }
    std::vector<std::vector<CmdBuffer>> testGroups = {
        // Group 1
        {
            {"W", 0, 10},
            {"W", 1, 20},
            {"E", 0, 1},       // removes WRITE(0)
            {"W", 2, 30},
            {"E", 1, 2}        // removes WRITE(1) and WRITE(2)
        },
        // Expected buffer: []

        // Group 2
        {
            {"E", 10, 2},
            {"E", 12, 2},      // merge → ERASE(10,4)
            {"W", 14, 100},
            {"E", 14, 1},      // removes WRITE(14)
            {"W", 15, 200}
        },
        // Expected buffer: ERASE(10, 4), WRITE(15, 200)

        // Group 3
        {
            {"W", 20, 1},
            {"W", 21, 2},
            {"W", 22, 3},
            {"W", 23, 4},
            {"W", 24, 5}
        },
        // Expected buffer: all 5 writes (no erase): WRITE(20)...WRITE(24)

        // Group 4
        {
            {"E", 20, 5},       // removes WRITE(20)...WRITE(24)
            {"W", 25, 10},
            {"W", 26, 11},
            {"W", 27, 12},
            {"W", 28, 13}
        },
        // Expected buffer: ERASE(20,5), WRITE(25)...WRITE(28)

        // Group 5
        {
            {"W", 30, 111},
            {"W", 31, 222},
            {"W", 32, 333},
            {"W", 33, 444},
            {"W", 34, 555}
        },
        // Expected buffer: WRITE(30)...WRITE(34)

        // Group 6
        {
            {"E", 30, 3},       // removes WRITE(30), WRITE(31), WRITE(32)
            {"E", 33, 2},       // merge with previous → ERASE(30,5), removes WRITE(33), WRITE(34)
            {"W", 35, 99},
            {"W", 36, 88},
            {"W", 37, 77}
        },
        // Expected buffer: ERASE(30,5), WRITE(35), WRITE(36), WRITE(37)

        // Group 7
        {
            {"E", 35, 3},       // removes WRITE(35), WRITE(36), WRITE(37)
            {"W", 40, 1},
            {"W", 41, 2},
            {"W", 42, 3},
            {"W", 43, 4}
        },
        // Expected buffer: ERASE(30,5), ERASE(35,3), WRITE(40)...WRITE(43)

        // Group 8
        {
            {"E", 44, 1},        // just an erase
            {"E", 45, 1},        // merge possible
            {"E", 46, 1},
            {"E", 47, 1},
            {"E", 48, 1}         // all merge → ERASE(44,5)
        },
        // Expected buffer: ERASE(30,5), ERASE(35,3), WRITE(40)...WRITE(43), ERASE(44,5)

        // Group 9
        {
            {"W", 50, 100},
            {"W", 51, 200},
            {"W", 52, 300},
            {"W", 53, 400},
            {"W", 54, 500}
        },
        // Expected buffer: WRITE(50)...WRITE(54)

        // Group 10
        {
            {"E", 50, 5},        // removes WRITE(50)...WRITE(54)
            {"W", 55, 600},
            {"W", 56, 700},
            {"E", 56, 1},        // removes WRITE(56)
            {"W", 57, 800}
        }
        // Expected buffer: ERASE(50,5), WRITE(55), ERASE(56,1), WRITE(57)
    };
};

std::vector<std::vector<CmdBuffer>> resultGroups = {
    // Group 1
    {
        {"E", 0, 3 },
        {"", 0, 0 },
        {"", 0, 0 },
        {"", 0, 0 },
        {"", 0, 0 },
    },
    // Expected buffer: []

    // Group 2
    {
        {"E", 10, 4 },
        {"W", 15, 200 },
        {"", 0, 0 },
        {"", 0, 0 },
        {"", 0, 0 },
    },
    // Expected buffer: ERASE(10, 4), WRITE(15, 200)

    // Group 3
    {
        {"W", 20, 1},
        {"W", 21, 2},
        {"W", 22, 3},
        {"W", 23, 4},
        {"W", 24, 5}
    },
    // Expected buffer: all 5 writes (no erase): WRITE(20)...WRITE(24)

    // Group 4
    {
        {"E", 20, 5},
        {"W", 25, 10},
        {"W", 26, 11},
        {"W", 27, 12},
        {"W", 28, 13}
    },
    // Expected buffer: ERASE(20,5), WRITE(25)...WRITE(28)

    // Group 5
    {
        {"W", 30, 111},
        {"W", 31, 222},
        {"W", 32, 333},
        {"W", 33, 444},
        {"W", 34, 555}
    },
    // Expected buffer: WRITE(30)...WRITE(34)

    // Group 6
    {
    },
    // Expected buffer: ERASE(30,5), WRITE(35), WRITE(36), WRITE(37)

    // Group 7
    {
    },
    // Expected buffer: ERASE(30,5), ERASE(35,3), WRITE(40)...WRITE(43)

    // Group 8
    {
    },
    // Expected buffer: ERASE(30,5), ERASE(35,3), WRITE(40)...WRITE(43), ERASE(44,5)

    // Group 9
    {
    },
    // Expected buffer: WRITE(50)...WRITE(54)

    // Group 10
    {
    }
    // Expected buffer: ERASE(50,5), WRITE(55), ERASE(56,1), WRITE(57)
};

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

TEST_F(BufferTest, mergeBufferTest) {
    int testNum = 5;
    int cmdNum = 5;
    for (int i = 0; i < testNum; i++) {
        buffer.initBuffer();
        for (int j = 0; j < cmdNum; j++) {
            buffer.cmdBuffer.push_back(testGroups[i][j]);
            buffer.updateBuffer(testGroups[i][j].cmd, testGroups[i][j].LBA, testGroups[i][j].value);
        }
        for (int j = 0; j < 5; j++) {
            EXPECT_EQ(resultGroups[i][j].cmd, buffer.cmdBuffer[j].cmd);
            EXPECT_EQ(resultGroups[i][j].LBA, buffer.cmdBuffer[j].LBA);
            EXPECT_EQ(resultGroups[i][j].value, buffer.cmdBuffer[j].value);
        }
        buffer.resetBuffer();
        buffer.writeBuffer();
        buffer.readBuffer();
    }
}

#endif