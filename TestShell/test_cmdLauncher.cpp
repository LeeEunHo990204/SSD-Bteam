#include "test.h"
#include "gmock/gmock.h"
#include "CmdLauncher.h"

using namespace testing;
using namespace std;

class MockUTCmdLauncher : public ICmdLauncher {
public:
    //MOCK_METHOD(void, write, (int LBA, unsigned int val), (override));
    //MOCK_METHOD(unsigned int, read, (int LBA), (override));
    void write(int LBA, unsigned int val) override {
        if ((LBA > MAX_LBA) || (LBA < MIN_LBA))
            return;
        array[LBA] = to_string(val);
    }
    std::string read(int LBA) override {
        if ((LBA > MAX_LBA) || (LBA < 0))
            return "ERROR";
        return array[LBA];
    }
    bool erase(int LBA, int size) override {
        if (LBA < 0 || LBA > MAX_LBA || size < 0 || size > 10) return false;
        for (int i = LBA; i < LBA + size; i++) {
            array[i] = to_string(0);
        }
        return true;
    }
private:
    std::string array[100] = { "", };
};

class UtFixture : public Test {

public:
    MockUTCmdLauncher  mockCmdLauncher;
    SSDCmdLauncher  ssdCmdLauncher;
#ifdef   MOCK_TEST
    ShellDeviceDriver cmdLauncher{ &mockCmdLauncher };
#else
    ShellDeviceDriver cmdLauncher{ &ssdCmdLauncher };
#endif
};

TEST_F(UtFixture, RWTestPass) {
    cmdLauncher.write(3, 0x12345678);
    EXPECT_EQ("0x12345678", cmdLauncher.read(3));
}

TEST_F(UtFixture, RWTestSequencePass) {
    cmdLauncher.write(1, 0x12345678);
    cmdLauncher.write(2, 0xDEADBEEF);
    cmdLauncher.write(3, 0x99999999);
    cmdLauncher.write(4, 0xABCD4321);
    cmdLauncher.write(91, 0x12345678);
    cmdLauncher.write(92, 0xDEADBEEF);
    cmdLauncher.write(93, 0x99999999);
    cmdLauncher.write(94, 0xABCD4321);

    EXPECT_EQ("0x12345678", cmdLauncher.read(1));
    EXPECT_EQ("0xDEADBEEF", cmdLauncher.read(2));
    EXPECT_EQ("0x99999999", cmdLauncher.read(3));
    EXPECT_EQ("0xABCD4321", cmdLauncher.read(4));
            
    EXPECT_EQ("0x12345678", cmdLauncher.read(91));
    EXPECT_EQ("0xDEADBEEF", cmdLauncher.read(92));
    EXPECT_EQ("0x99999999", cmdLauncher.read(93));
    EXPECT_EQ("0xABCD4321", cmdLauncher.read(94));
}

TEST_F(UtFixture, RWTestAllRangePass) {
    for (int i = 0; i < 100; i++) {
        cmdLauncher.write(i, 0x12345678);
    }
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ("0x12345678", cmdLauncher.read(i));
    }
}

TEST_F(UtFixture, RWTestOutOfLBA) {
    EXPECT_EQ("ERROR", cmdLauncher.read(100));
    cmdLauncher.write(MAX_LBA + 1, 0x12345678);
    EXPECT_EQ("ERROR", cmdLauncher.read(MAX_LBA + 1));
}

TEST_F(UtFixture, EraseTestPass) {
    EXPECT_TRUE(cmdLauncher.erase(1, 5));
}

TEST_F(UtFixture, EraseTestFail) {
    EXPECT_FALSE(cmdLauncher.erase(-1, 5));
}