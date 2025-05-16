#include "gmock/gmock.h"
#include "ssd.h"

using namespace testing;
using namespace std;

TEST(SsdTest, write) {
	Ssd ssd;
	ssd.write(0, 0);
}

TEST(SsdTest, read) {
	Ssd ssd;
	ssd.read(0);
}