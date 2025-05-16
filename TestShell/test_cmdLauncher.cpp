#include "gmock/gmock.h"
#include "cmd_launcher.cpp"

using namespace testing;
using namespace std;

class MockCmdLauncher : public ICmdLauncher {
public:
    //MOCK_METHOD(void, write, (int LBA, unsigned int val), (override));
    //MOCK_METHOD(unsigned int, read, (int LBA), (override));
    void write(int LBA, unsigned int val) override {
        array[LBA] = val;
    }
    unsigned int read(int LBA) override {
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

TEST_F(UtFixture, WriteTest) {
    cmdLauncher.write(3, 0x12345678);
    EXPECT_EQ(0x12345678, cmdLauncher.read(3));
}
