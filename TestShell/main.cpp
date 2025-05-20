
#include "gmock/gmock.h"
#include <iostream>
#include "Shell.h"
#include "Runner.h"
using namespace testing;


#include <iostream>

int main(int argc, char** argv) {

#if defined(_DEBUG)
	InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	if (argc == 1) {
		Shell* shell = new Shell(new SSDCmdLauncher, new TestScripts1());
		shell->run();
	}
	else if (argc == 2) {
		//TODO: Implementation of runner
		std::cout << "Runner execution" << std::endl;
		Runner* runner = new Runner();
	}
	else {
		//TODO: print help, useage message
	}
	return 0;
#endif
}