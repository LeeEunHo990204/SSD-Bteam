
#include "gmock/gmock.h"
#include <iostream>
#include "Shell.h"
using namespace testing;


#include <iostream>

int main(int argc, char** argv) {

#if defined(_DEBUG)
	InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	Shell* shell = new Shell(new SSDCmdLauncher, new TestScripts1());
	shell->run();
	return 0;
#endif
}