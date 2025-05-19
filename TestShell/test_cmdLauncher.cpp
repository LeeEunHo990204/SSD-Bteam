#include "gmock/gmock.h"
#include "cmd_launcher.cpp"

using namespace testing;
using namespace std;

class MockCmdLauncher : public ICmdLauncher {
public:
    //MOCK_METHOD(void, write, (int LBA, unsigned int val), (override));
    //MOCK_METHOD(unsigned int, read, (int LBA), (override));
    void write(int LBA, unsigned int val) override {
        if (LBA >= 100)
            return;
        array[LBA] = val;
    }
    unsigned int read(int LBA) override {
        if (LBA >= 100)
            return 0;
        return array[LBA];
    }
private:
    unsigned int array[100] = { 0, };
};

class UtFixture : public Test {

public:
    MockCmdLauncher  mockCmdLauncher;
    SSDCmdLauncher  ssdCmdLauncher;
    ShellDeviceDriver cmdLauncher{ &mockCmdLauncher };
};

TEST_F(UtFixture, RWTestPass) {
    cmdLauncher.write(3, 0x12345678);
    EXPECT_EQ(0x12345678, cmdLauncher.read(3));
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
    EXPECT_EQ(0x12345678, cmdLauncher.read(1));
    EXPECT_EQ(0xDEADBEEF, cmdLauncher.read(2));
    EXPECT_EQ(0x99999999, cmdLauncher.read(3));
    EXPECT_EQ(0xABCD4321, cmdLauncher.read(4));

    EXPECT_EQ(0x12345678, cmdLauncher.read(91));
    EXPECT_EQ(0xDEADBEEF, cmdLauncher.read(92));
    EXPECT_EQ(0x99999999, cmdLauncher.read(93));
    EXPECT_EQ(0xABCD4321, cmdLauncher.read(94));
}

TEST_F(UtFixture, RWTestAllRangePass) {
    for (int i = 0; i < 100; i++) {
        cmdLauncher.write(i, 0x12345678);
    }
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(0x12345678, cmdLauncher.read(i));
    }
}

TEST_F(UtFixture, RWTestOutOfLBA) {
    cmdLauncher.write(100, 0x12345678);
    EXPECT_EQ(0, cmdLauncher.read(100));
}