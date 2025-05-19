#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "cmd_launcher.cpp"
#include "TestScripts.h"

class Validation {
public:
	Validation() {
		paramCntMap = {
			{"write", 2}, {"WRITE", 2},
			{"read", 1}, {"READ", 1},
			{"fullwrite", 1}, {"FULLWRITE", 1},
			{"fullread", 0}, {"FULLREAD", 0},
			{"1_", 0}, {"1_FullWriteAndReadCompare", 0},
			{"2_", 0}, {"2_PartialLBAWrite", 0},
			{"3_", 0}, {"3_WriteReadAging", 0}
		};
	}

	bool isValid(const std::string& cmd, const std::vector<std::string>& params) {
		if (paramCntMap.count(cmd) == 0) return true;
		int paramCnt = params.size();
		if (isValidParamCnt(cmd, paramCnt) == false) return false;
		if(cmd == "write" || cmd == "WRITE" || cmd == "fullwrite" || cmd == "FULLWRITE")
			return isValidValue(params[paramCnt - 1]);
		return true;
	}

private:
	std::unordered_map<std::string, int> paramCntMap;
	
	bool isValidParamCnt(const std::string& cmd, const int& paramCnt) {
		if (paramCntMap.count(cmd) == 0) return true;
		return paramCntMap[cmd] == paramCnt;
	}

	bool isValidValue(const std::string& val) {
		return (val.length() == 10) && startsWith(val, "0x");
	}

	bool startsWith(const std::string& str, const std::string& prefix) {
		return str.size() >= prefix.size() &&
			str.compare(0, prefix.size(), prefix) == 0;
	}
};

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

		Validation* validation = new Validation();
		isValid = validation->isValid(cmd, params);
	}

	bool getValid() {
		return isValid;
	}

private:
	bool isValid;
};

class Shell {
public:
	Shell(ICmdLauncher* cmdLauncher, ITestScripts* testScripts) :
		cmdLauncher(cmdLauncher), testScripts(testScripts) {
	}

	void run() {
		std::string cmdLine;
		while (true) {
			std::cout << "shell> ";
			std::getline(std::cin, cmdLine);

			std::string result = runOneCommand(cmdLine);
			if (result == "EXIT") break;
			else if (result == "HELP") continue;
			std::cout << result;
		}
	}

	std::string runOneCommand(std::string cmdLine) {
		std::vector<std::string> words = splitBySpace(cmdLine);
		Command* command = new Command(words);
		if (command->getValid() == false) {
			return "INVALID COMMAND\n";
		}

		if (command->cmd == "write" || command->cmd == "WRITE") {
			int LBA = stoi(command->params[0]);
			unsigned int val = stoul(command->params[1]);
			cmdLauncher->write(LBA, val);
			return "[Write] Done\n";
		}

		else if (command->cmd == "read" || command->cmd == "READ") {
			int LBA = stoi(command->params[0]);
			return std::string("[Read] LBA ") + std::to_string(LBA) + std::string(" : ") + std::to_string(cmdLauncher->read(LBA)) + std::string("\n");
		}

		else if (command->cmd == "exit" || command->cmd == "EXIT") {
			return "EXIT";
		}

		else if (command->cmd == "help" || command->cmd == "HELP") {
			return "HELP";
		}

		else if (command->cmd == "fullwrite" || command->cmd == "FULLWRITE") {
			unsigned int val = stoul(command->params[0]);
			for (int LBA = 0; LBA < 100; LBA++) {
				cmdLauncher->write(LBA, val);
			}
			return "[FullWrite] Done\n";
		}

		else if (command->cmd == "fullread" || command->cmd == "FULLREAD") {
			std::string result = "[FullRead]";
			for (int LBA = 0; LBA < 100; LBA++) {
				result += std::string("LBA ") + std::to_string(LBA) + std::string(" : ") + std::to_string(cmdLauncher->read(LBA)) + std::string("\n");
			}
			return result;
		}

		else if (command->cmd == "1_" || command->cmd == "1_FullWriteAndReadCompare") {
			setTestScripts(new TestScripts1);
			testScripts->runTestScenario();
			if (testScripts->getResult() == 0) return "PASS\n";
			return "FAIL\n";
		}

		else if (command->cmd == "2_" || command->cmd == "2_PartialLBAWrite") {
			//setTestScripts(new TestScripts2);
			testScripts->runTestScenario();
			if (testScripts->getResult() == 0) return "PASS\n";
			return "FAIL\n";
		}

		else if (command->cmd == "3_" || command->cmd == "3_WriteReadAging") {
			//setTestScripts(new TestScripts3);
			testScripts->runTestScenario();
			if (testScripts->getResult() == 0) return "PASS\n";
			return "FAIL\n";
		}
	}

	void setTestScripts(ITestScripts* testScripts) {
		this->testScripts = testScripts;
	}

private:
	ICmdLauncher* cmdLauncher;
	ITestScripts* testScripts;

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