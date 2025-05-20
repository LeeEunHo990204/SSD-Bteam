#include "gmock/gmock.h"
#include "CmdLancher.h"
#include "TestScripts.h"
#include "Runner.h"



class TestRunnerFixture : public ::testing::Test
{
public:
	Runner* runner;

	int factoryCreateRunner(std::string filePath) {
		runner = new Runner(filePath);
	}

	void deleteRunner(void) {
		if (runner != nullptr) {
			delete runner;
		}
	}

	std::string DEFAULT_SCRIPTS_PATH = "shell_scripts.txt";
	int DEFAULT_SCRIPTS_NUM = 4;
};

TEST_F(TestRunnerFixture, TestMatchingFIlePath) {
	//Arrange
	runner = new Runner(DEFAULT_SCRIPTS_PATH);

	//Act, Assert
	EXPECT_EQ(DEFAULT_SCRIPTS_PATH, runner->getFilePath());
	deleteRunner();
}

TEST_F(TestRunnerFixture, TestParsingInputFile) {
	//Arrange
	runner = new Runner(DEFAULT_SCRIPTS_PATH);

	//Act
	int result = runner->parseInputScripts();

	//Assert
	EXPECT_EQ(DEFAULT_SCRIPTS_NUM, runner->getScriptsNum());
	deleteRunner();
}