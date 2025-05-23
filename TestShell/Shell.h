#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "CmdLauncher.h"
#include "TestScripts.h"
#include "Logger.h"
#include "util.h"

#define STORAGE_SIZE 100

class Validation {
public:
	Validation() {
		cmdSet = {
			"write", "WRITE",
			"read", "READ",
			"erase", "ERASE",
			"erase_range", "ERASE_RANGE",
			"flush", "FLUSH",
			"fullwrite", "FULLWRITE",
			"fullread", "FULLREAD",
			"1_", "1_FullWriteAndReadCompare",
			"2_", "2_PartialLBAWrite",
			"3_", "3_WriteReadAging",
			"4_", "4_EraseAndWriteAging",
			"5_", "5_Test",
			"6_", "6_Test",
			"exit", "EXIT",
			"help", "HELP"
		};
		paramCntMap = {
			{"write", 2}, {"WRITE", 2},
			{"read", 1}, {"READ", 1},
			{"erase", 2}, {"ERASE", 2},
			{"erase_range", 2}, {"ERASE_RANGE", 2},
			{"flush", 0}, {"FLUSH", 0},
			{"fullwrite", 1}, {"FULLWRITE", 1},
			{"fullread", 0}, {"FULLREAD", 0},
			{"1_", 0}, {"1_FullWriteAndReadCompare", 0},
			{"2_", 0}, {"2_PartialLBAWrite", 0},
			{"3_", 0}, {"3_WriteReadAging", 0},
			{"4_", 0 }, {"4_EraseAndWriteAging", 0},
			{"5_", 0 }, {"5_Test", 0},
			{"6_", 0 }, {"6_Test", 0},
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

class ICmd {
public:
	virtual ~ICmd() {}
	virtual std::string execute(ICmdLauncher* cmdLauncher, Command* command) = 0;
};

class Write : public ICmd {
public:
	std::string execute(ICmdLauncher* cmdLauncher, Command* command) override {
		int LBA = stoi(command->params[0]);
		if (isOutOf4ByteRange(command->params[1])) {
			return "Out of 4-byte range!";
		}
		if (LBA >= STORAGE_SIZE || LBA < 0) {
			return "[Write] ERROR";
		}
		unsigned int val = stoul(command->params[1], nullptr, 16);
		cmdLauncher->write(LBA, val);
		return "[Write] Done";
	}
};

class Read : public ICmd {
public:
	std::string execute(ICmdLauncher* cmdLauncher, Command* command) override {
		int LBA = stoi(command->params[0]);
		if (LBA >= STORAGE_SIZE || LBA < 0)
			return std::string("[Read] LBA ") + std::to_string(LBA) + std::string(" : ") + std::string("ERROR");
		return std::string("[Read] LBA ") + std::to_string(LBA) + std::string(" : ") + cmdLauncher->read(LBA);
	}
};

class Erase : public ICmd {
public:
	std::string execute(ICmdLauncher* cmdLauncher, Command* command) override {
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
};

class EraseRange : public ICmd {
public:
	std::string execute(ICmdLauncher* cmdLauncher, Command* command) override {
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
};

class Flush : public ICmd {
public:
	std::string execute(ICmdLauncher* cmdLauncher, Command* command) override {
		cmdLauncher->flush();
		return "[Flush] Done";
	}
};

class Exit : public ICmd {
public:
	std::string execute(ICmdLauncher* cmdLauncher, Command* command) override {
		Logger::getInstance().saveLog(true);
		return "EXIT";
	}
};

class Help : public ICmd {
public:
	Help() {
		mHelp.insert(make_pair("write (or WRITE)", "write 3 0xAAAABBBB -> write 3rd LBA with value 0xAAAABBBB"));
		mHelp.insert(make_pair("read (or READ)", "read 3 -> read 3rd LBA"));
		mHelp.insert(make_pair("erase (or ERASE)", "erase 3 5 -> erase 3~7 LBA(size:5)"));
		mHelp.insert(make_pair("erase_range (or ERASE_RANGE)", "erase_range 3 5 -> erase 3~5 LBA(range from 3 to 5 of LBA) "));
		mHelp.insert(make_pair("fullwrite (or FULLWRITE)", "fullwrite 0xABCDFFFF -> write all LBA with value 0xABCDFFFF"));
		mHelp.insert(make_pair("fullread (or FULLREAD)", "fullread -> all LBA read"));
		mHelp.insert(make_pair("1_ (or 1_FullWriteAndReadCompare)", "TestScript1: Full Write then Full Read 100times"));
		mHelp.insert(make_pair("2_ (or 2_PartialLBAWrite)", "Full Write 3times then Full read once"));
		mHelp.insert(make_pair("3_ (or 3_WriteReadAging)", "Write random value on LBA 0 and 99 then ReadCompare for a loop 200 times"));
		mHelp.insert(make_pair("4_ (or 4_EraseAndWriteAging)", "TestScript4: Write and Overwrite and Erase LBA with it's scenario"));
		mHelp.insert(make_pair("exit (or EXIT)", "Exit testShell"));
		mHelp.insert(make_pair("help (or HELP)", "Print team name and members then explain all commands"));
	}

	std::string execute(ICmdLauncher* cmdLauncher, Command* command) override {
		printHelp();
		return "HELP";
	}

private:
	unordered_map<string, string>  mHelp;
	void printHelp() {
		cout << "################ TEAM : Best Reviewer : KDH, KSI, KDG, KDJ, PYH, LEH ################" << endl << endl;
		cout << std::left << std::setw(40) << ">> Command" << " : " << ">> Description" << endl;
		for (auto it = mHelp.begin(); it != mHelp.end(); it++) {
			cout << std::left << std::setw(40) << it->first << " : " << it->second << endl;
		}
	}
};

class Fullwrite : public ICmd {
public:
	std::string execute(ICmdLauncher* cmdLauncher, Command* command) override {
		if (isOutOf4ByteRange(command->params[0])) {
			return "Out of 4-byte range!";
		}
		unsigned int val = stoul(command->params[0], nullptr, 16);
		for (int LBA = 0; LBA < STORAGE_SIZE; LBA++) {
			cmdLauncher->write(LBA, val);
		}
		return "[FullWrite] Done";
	}
};

class Fullread : public ICmd {
public:
	std::string execute(ICmdLauncher* cmdLauncher, Command* command) override {
		std::string result = "[FullRead]";
		for (int LBA = 0; LBA < 100; LBA++) {
			result += std::string("LBA ") + std::to_string(LBA) + std::string(" : ") + cmdLauncher->read(LBA);
			if (LBA < 99) result += std::string("\n");
		}
		return result;
	}
};

class CmdFactory {
public:
	static std::unique_ptr<ICmd> create(const std::string& cmdType) {
		if (cmdType == "write" || cmdType == "WRITE") {
			return std::make_unique<Write>();
		}
		if (cmdType == "read" || cmdType == "READ") {
			return std::make_unique<Read>();
		}
		if (cmdType == "erase" || cmdType == "ERASE") {
			return std::make_unique<Erase>();
		}
		if (cmdType == "erase_range" || cmdType == "ERASE_RANGE") {
			return std::make_unique<EraseRange>();
		}
		if (cmdType == "flush" || cmdType == "FLUSH") {
			return std::make_unique<Flush>();
		}
		if (cmdType == "exit" || cmdType == "EXIT") {
			return std::make_unique<Exit>();
		}
		if (cmdType == "help" || cmdType == "HELP") {
			return std::make_unique<Help>();
		}
		if (cmdType == "fullwrite" || cmdType == "FULLWRITE") {
			return std::make_unique<Fullwrite>();
		}
		if (cmdType == "fullread" || cmdType == "FULLREAD") {
			return std::make_unique<Fullread>();
		}
		return nullptr;
	}
};

class Shell {
public:
	Shell(ICmdLauncher* cmdLauncher, ITestScripts* testScripts) :
		cmdLauncher(cmdLauncher), testScripts(testScripts) {
	}

	Shell(ICmdLauncher* cmdLauncher) : cmdLauncher(cmdLauncher) {
		testScripts = nullptr;
	}

	void run();

	std::string runCommand(std::string cmdLine);

	void setTestScripts(ITestScripts* testScripts) {
		this->testScripts = testScripts;
	}

private:
	ICmdLauncher* cmdLauncher;
	ITestScripts* testScripts;
};