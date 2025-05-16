#include "gmock/gmock.h"
#include <iostream>

int main(int argc, char** argv) {
	
	std::string cmd;

	::testing::InitGoogleMock();
	RUN_ALL_TESTS();

	while (true) {
		std::cout << "shell> ";
		std::cin >> cmd;

		if (cmd == "write" || cmd == "WRITE") {
			std::cout << "Write Test." << std::endl;
			//TODO
			system("./SSD write [LBA] [DATA]");
		} 

		if (cmd == "read" || cmd == "READ") {
			std::cout << "Read Test." << std::endl;
			//TODO
			system("./SSD read [LBA]");
		}

		if (cmd == "exit") {
			std::cout << "exit!!" << std::endl;
			break;
		}

		else {
			std::cout << "Invalid command. Please try again." << std::endl;
		}
	}
	
	return 0;
}