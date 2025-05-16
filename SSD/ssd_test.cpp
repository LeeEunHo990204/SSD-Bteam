#include "gmock/gmock.h"
#include "ssd.h"

using namespace testing;
using namespace std;

TEST(SsdTest, constructor) {
	SSD ssd;
}

TEST(SsdTest, write1) {
	SSD ssd;
	ssd.write(0, 0x123);
}

TEST(SsdTest, read1) {
	SSD ssd;
	ssd.read(0);
}

TEST(SsdTest, read2) {
	SSD ssd;
	ssd.write(0, 0x123);
	EXPECT_EQ(0x123, ssd.read(0));
}

TEST(SsdTest, read3) {
	SSD ssd;
	EXPECT_EQ(0x123, ssd.read(1));
}

TEST(SsdTest, read4) {
	SSD ssd;
	EXPECT_EQ(0x12345678, ssd.read(2));
}