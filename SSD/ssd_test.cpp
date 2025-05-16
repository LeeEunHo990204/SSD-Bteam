#include "gmock/gmock.h"
#include "ssd.h"

using namespace testing;
using namespace std;

TEST(SsdTest, init1) {
	Ssd ssd;
	ssd.init();
}

TEST(SsdTest, write1) {
	Ssd ssd;
	ssd.write(0, 0x123);
}

TEST(SsdTest, read1) {
	Ssd ssd;
	ssd.read(0);
}

TEST(SsdTest, read2) {
	Ssd ssd;
	ssd.write(0, 0x123);
	EXPECT_EQ(0x123, ssd.read(0));
}

TEST(SsdTest, read3) {
	Ssd ssd;
	ssd.init();
	EXPECT_EQ(0x123, ssd.read(1));
}

TEST(SsdTest, read4) {
	Ssd ssd;
	ssd.init();
	EXPECT_EQ(0x12345678, ssd.read(2));
}