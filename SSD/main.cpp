#include "gmock/gmock.h"

using namespace testing;
using namespace std;

int main() {
#if defined(_DEBUG)
	InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}