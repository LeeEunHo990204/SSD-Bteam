#include <iostream>
#include <sstream>
#include <vector>
#include <string>

class Command {
public:
	std::string cmd;
	std::vector<std::string> params;

	Command(std::vector<std::string> words) {
		isValid = false;
		if (words.size() == 0) {
			return;
		}
		cmd = words[0];
		params.assign(words.begin() + 1, words.end());
	}

	bool getValid() {
		return isValid;
	}

private:
	bool isValid;
};

class Shell {
public:
	void run() {
		std::string cmdLine;
		while (true) {
			std::cout << "shell> ";
			std::getline(std::cin, cmdLine);

			std::vector<std::string> words = splitBySpace(cmdLine);
			Command* command = new Command(words);
			if (command->getValid() == false) {
				std::cout << "INVALID COMMAND" << std::endl;
				continue;
			}

			if (command->cmd == "write" || command->cmd == "WRITE") {
				std::cout << "Write Test." << std::endl;
				//TODO
				//system("./SSD write [LBA] [DATA]");
			}

			else if (command->cmd == "read" || command->cmd == "READ") {
				std::cout << "Read Test." << std::endl;
				//TODO
				//system("./SSD read [LBA]");
			}

			else if (command->cmd == "exit") {
				std::cout << "exit!!" << std::endl;
				break;
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
};