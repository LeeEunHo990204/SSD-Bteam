#include "Shell.h"

void Shell::run() {
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

std::string Shell::runCommand(std::string cmdLine) {
	std::vector<std::string> words = splitBySpace(cmdLine);
	Command* command = new Command(words);
	if (command->getValid() == false) {
		return "INVALID COMMAND";
	}

	auto cmd = CmdFactory::create(command->cmd);
	if (cmd != nullptr) return cmd->execute(cmdLauncher, command);

	if (command->cmd == "1_" || command->cmd == "1_FullWriteAndReadCompare") {
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
