#include <iostream>
#include <sstream>
#include <vector>
#include <string>

struct Command {
	bool isValid;
	std::string cmd;
	std::vector<std::string> params;
};

class Shell {
public:
	void run() {
		std::string cmdLine;
		while (true) {
			std::cout << "shell> ";
			std::getline(std::cin, cmdLine);

			std::vector<std::string> words = splitBySpace(cmdLine);
			Command command = makeCommand(words);
			if (command.isValid == false) {
				// std::cout << "INVALID COMMAND" << std::endl;
				continue;
			}

			if (command.cmd == "write" || command.cmd == "WRITE") {
				std::cout << "Write Test." << std::endl;
				//TODO
				//system("./SSD write [LBA] [DATA]");
			}

			else if (command.cmd == "read" || command.cmd == "READ") {
				std::cout << "Read Test." << std::endl;
				//TODO
				//system("./SSD read [LBA]");
			}

			else if (command.cmd == "exit") {
				std::cout << "exit!!" << std::endl;
				break;
			}

			else {
				std::cout << "Invalid command. Please try again." << std::endl;
			}

		}
	}

private:
	std::vector<std::string> splitBySpace(const std::string& input) {
		std::istringstream iss(input);
		std::vector<std::string> result;
		std::string word;

		if (input.empty()) {
			return result;
		}

		while (iss >> word) {
			result.push_back(word);
		}

		return result;
	}

	Command makeCommand(std::vector<std::string> words) {
		Command command;
		if (words.size() == 0) {
			command.isValid = false;
			return command;
		}
		command.cmd = words[0];
		command.params.assign(words.begin() + 1, words.end());

		return command;
	}
};