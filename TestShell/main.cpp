#include "gmock/gmock.h"
#include <iostream>
#include "Shell.cpp"
using namespace testing;

int main(int argc, char** argv) {
#if defined(_DEBUG)
	InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	Shell shell;
	shell.run();
	return 0;
#endif
}