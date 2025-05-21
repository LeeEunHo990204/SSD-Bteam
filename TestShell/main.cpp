
#include "gmock/gmock.h"
#include <iostream>
#include <signal.h>
#include <fstream>
#include <cstdlib>
#include "Shell.h"
#include "Runner.h"
#include "Logger.h"
using namespace testing;


void signalHandlerForProcessExit(int signum) {
	Logger::getInstance().saveLog(true);
	std::exit(signum);
}

int main(int argc, char** argv) {

#if defined(_DEBUG)
	InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	if (signal(SIGINT, signalHandlerForProcessExit) == SIG_ERR) {
		std::cerr << "Failed to register signalHandlerForProcessExit" << std::endl;
		return 0;
	}
	system("mkdir Log");
	if (argc == 2) {
		std::string filePath(argv[1]);
		Runner* runner = new Runner(filePath);
		if (runner->parseInputScripts() != 0) {
			std::cout << "Failed to get script file" << std::endl;
			return 0;
		}
		runner->runScripts();
		delete runner;
	}
	else { 
		Shell* shell = new Shell(new SSDCmdLauncher, new TestScripts1());
		shell->run();
		delete shell;
	}
	return 0;
#endif
}