#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "CmdLancher.h"
#include "TestScripts.h"
#include "Logger.h"

#define STORAGE_SIZE 100

class Validation {
public:
	Validation() {
		cmdSet = {
			"write", "WRITE",
			"read", "READ",
			"erase", "ERASE",
			"erase_range", "ERASE_RANGE",
			"fullwrite", "FULLWRITE",
			"fullread", "FULLREAD",
			"1_", "1_FullWriteAndReadCompare",
			"2_", "2_PartialLBAWrite",
			"3_", "3_WriteReadAging",
			"4_", "4_EraseAndWriteAging",
			"exit", "EXIT",
			"help", "HELP"
		};
		paramCntMap = {
			{"write", 2}, {"WRITE", 2},
			{"read", 1}, {"READ", 1},
			{"erase", 2}, {"ERASE", 2},
			{"erase_range", 2}, {"ERASE_RANGE", 2},
			{"fullwrite", 1}, {"FULLWRITE", 1},
			{"fullread", 0}, {"FULLREAD", 0},
			{"1_", 0}, {"1_FullWriteAndReadCompare", 0},
			{"2_", 0}, {"2_PartialLBAWrite", 0},
			{"3_", 0}, {"3_WriteReadAging", 0},
			{"4_", 0 }, {"4_EraseAndWriteAging", 0}
		};
	}

	bool isValid(const std::string& cmd, const std::vector<std::string>& params) {
		if (cmdSet.count(cmd) == 0) return false;

		// EXIT, HELP
		if (paramCntMap.count(cmd) == 0) return true;

		int paramCnt = params.size();
		if (isValidParamCnt(cmd, paramCnt) == false) return false;
		if (cmd == "write" || cmd == "WRITE" || cmd == "fullwrite" || cmd == "FULLWRITE")
			return isValidValue(params[paramCnt - 1]);
		return true;
	}

private:
	std::unordered_set<std::string> cmdSet;
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
		mHelp.insert(make_pair("write (or WRITE)", "write 3 0xAAAABBBB -> write 3rd LBA with value 0xAAAABBBB"));
		mHelp.insert(make_pair("read (or READ)", "read 3 -> read 3rd LBA"));
		mHelp.insert(make_pair("erase (or ERASE)", "erase 3 5 -> erase 3~7 LBA(size:5)"));
		mHelp.insert(make_pair("erase_range (or ERASE_RANGE)", "erase_range 3 5 -> erase 3~5 LBA(range from 3 to 5 of LBA) "));
		mHelp.insert(make_pair("fullwrite (or FULLWRITE)", "fullwrite 0xABCDFFFF -> write all LBA with value 0xABCDFFFF"));
		mHelp.insert(make_pair("fullread (or FULLREAD)", "fullread -> all LBA read"));
		mHelp.insert(make_pair("1_ (or 1_FullWriteAndReadCompare)", "TestScript1: Full Write then Full Read 100times"));
		mHelp.insert(make_pair("2_ (or 2_PartialLBAWrite", "Full Write 3times then Full read once"));
		mHelp.insert(make_pair("3_ (or 3_WriteReadAging)", "Write random value on LBA 0 and 99 then ReadCompare for a loop 200 times"));
		mHelp.insert(make_pair("4_ (or 4_EraseAndWriteAging)", "TestScript4: Write and Overwrite and Erase LBA with it's scenario"));
		mHelp.insert(make_pair("exit (or EXIT)", "Exit testShell"));
		mHelp.insert(make_pair("help (or HELP)", "Print team name and members then explain all commands"));

	}

	Shell(ICmdLauncher* cmdLauncher) : cmdLauncher(cmdLauncher) {
		testScripts = nullptr;
	}

	void run() {
		std::string cmdLine;
		while (true) {
			std::cout << "shell> ";
			std::getline(std::cin, cmdLine);

			std::string result = runCommand(cmdLine);
			if (result == "EXIT") break;
			else if (result == "HELP") continue;
			std::cout << result << std::endl;
		}
	}

	std::string runCommand(std::string cmdLine) {
		std::vector<std::string> words = splitBySpace(cmdLine);
		Command* command = new Command(words);
		if (command->getValid() == false) {
			return "INVALID COMMAND";
		}

		if (command->cmd == "write" || command->cmd == "WRITE") {
			int LBA = stoi(command->params[0]);
			if (isOutOf4ByteRange(command->params[1])) {
				return "Out of 4-byte range!";
			}
			unsigned int val = stoul(command->params[1], nullptr, 16);
			cmdLauncher->write(LBA, val);
			return "[Write] Done";
		}

		else if (command->cmd == "read" || command->cmd == "READ") {
			int LBA = stoi(command->params[0]);
			if (LBA >= STORAGE_SIZE || LBA < 0)
				return std::string("[Read] LBA ") + std::to_string(LBA) + std::string(" : ") + std::string("ERROR");
			return std::string("[Read] LBA ") + std::to_string(LBA) + std::string(" : ") + cmdLauncher->read(LBA);
		}

		else if (command->cmd == "erase" || command->cmd == "ERASE") {
			int LBA = stoi(command->params[0]);
			int size = stoi(command->params[1]);
			int startLBA = 0;
			int endLBA = 0;
			if (LBA >= STORAGE_SIZE || LBA < 0) {
				cmdLauncher->erase(LBA, size);
				return "[Erase] ERROR";
			}
			startLBA = LBA;
			endLBA = LBA + size - 1;
			if (size < 0) {
				startLBA = LBA + size + 1;
				size *= -1;
				if (startLBA < 0) startLBA = 0;
				endLBA = LBA;
			}
			if (endLBA >= STORAGE_SIZE) endLBA = 99;
			for (int i = startLBA; i <= endLBA; i += 10) {
				if (endLBA - i + 1 < 10) {
					if (!cmdLauncher->erase(i, endLBA - i + 1)) "[Erase] ERROR";
				}
				else {
					if (!cmdLauncher->erase(i, 10)) "[Erase] ERROR";
				}
			}
			return "[Erase] Done";
		}

		else if (command->cmd == "erase_range" || command->cmd == "ERASE_RANGE") {
			int startLBA = stoi(command->params[0]);
			int endLBA = stoi(command->params[1]);

			if (startLBA >= STORAGE_SIZE || startLBA < 0 || endLBA >= STORAGE_SIZE || endLBA < 0) {
				cmdLauncher->erase(-1, -1);
				return "[Erase_range] ERROR";
			}
			
			if (startLBA > endLBA) {
				swap(startLBA, endLBA);
			}
			for (int i = startLBA; i <= endLBA; i += 10) {
				if (endLBA - i + 1 < 10) {
					if (!cmdLauncher->erase(i, endLBA - i + 1)) "[Erase_range] ERROR";
				}
				else {
					if (!cmdLauncher->erase(i, 10)) "[Erase_range] ERROR";
				}
			}

			return "[Erase_range] Done";
		}

		else if (command->cmd == "exit" || command->cmd == "EXIT") {
			return "EXIT";
		}

		else if (command->cmd == "help" || command->cmd == "HELP") {
			printHelp();
			return "HELP";
		}

		else if (command->cmd == "fullwrite" || command->cmd == "FULLWRITE") {
			if (isOutOf4ByteRange(command->params[0])) {
				return "Out of 4-byte range!";
			}
			unsigned int val = stoul(command->params[0], nullptr, 16);
			for (int LBA = 0; LBA < STORAGE_SIZE; LBA++) {
				cmdLauncher->write(LBA, val);
			}
			return "[FullWrite] Done";
		}

		else if (command->cmd == "fullread" || command->cmd == "FULLREAD") {
			std::string result = "[FullRead]";
			for (int LBA = 0; LBA < 100; LBA++) {
				result += std::string("LBA ") + std::to_string(LBA) + std::string(" : ") + cmdLauncher->read(LBA);
				if (LBA < 99) result += std::string("\n");
			}
			return result;
		}

		else if (command->cmd == "1_" || command->cmd == "1_FullWriteAndReadCompare") {
			setTestScripts(new TestScripts1("1_FullWriteAndReadCompare", cmdLauncher));
			testScripts->runTestScenario();
			if (testScripts->getResult() == 0) return "PASS";
			return "FAIL";
		}

		else if (command->cmd == "2_" || command->cmd == "2_PartialLBAWrite") {
			setTestScripts(new TestScripts2("2_PartialLBAWrite", cmdLauncher));
			testScripts->runTestScenario();
			if (testScripts->getResult() == 0) return "PASS";
			return "FAIL";
		}

		else if (command->cmd == "3_" || command->cmd == "3_WriteReadAging") {
			setTestScripts(new TestScripts3("3_WriteReadAging", cmdLauncher));
			testScripts->runTestScenario();
			if (testScripts->getResult() == 0) return "PASS";
			return "FAIL";
		}

		else if (command->cmd == "4_" || command->cmd == "4_EraseAndWriteAging") {
			setTestScripts(new TestScripts4("4_WriteReadAging", cmdLauncher));
			testScripts->runTestScenario();
			if (testScripts->getResult() == 0) return "PASS";
			return "FAIL";
		}
	}

	void setTestScripts(ITestScripts* testScripts) {
		this->testScripts = testScripts;
	}

private:
	ICmdLauncher* cmdLauncher;
	ITestScripts* testScripts;
	unordered_map<string, string>  mHelp;

	void printHelp() {
		cout << "################ TEAM : Best Reviewer : KDH, KSI, KDG, KDJ, PYH, LYH ################" << endl << endl;
		cout << std::left << std::setw(40) << ">> Command" << " : " << ">> Description" << endl;
		for (auto it = mHelp.begin(); it != mHelp.end(); it++) {
			cout << std::left << std::setw(40) << it->first << " : " << it->second << endl;
		}
	}

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

	bool isOutOf4ByteRange(const std::string& hexStr) {
		try {
			unsigned long long value = std::stoull(hexStr, nullptr, 16);
			return value > 0xFFFFFFFFULL;
		}
		catch (const std::exception&) {
			return true;
		}
	}
};