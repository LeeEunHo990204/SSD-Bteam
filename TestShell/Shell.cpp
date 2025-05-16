#include <iostream>

class Shell {
public:
	void run() {
		std::string cmd;
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
	}
};