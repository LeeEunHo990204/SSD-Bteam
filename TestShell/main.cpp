
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
	if (argc == 2) {
		std::string filePath(argv[1]);
		Runner* runner = new Runner(filePath);
		if (runner->parseInputScripts() != 0) {
			std::cout << "Failed to get script file" << std::endl;
			return 0;
		}
		runner->runScripts();
	}
	else {
		Shell* shell = new Shell(new SSDCmdLauncher, new TestScripts1());
		shell->run();
	}
	return 0;
#endif
}