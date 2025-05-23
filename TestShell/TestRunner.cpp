#include "gmock/gmock.h"
#include "CmdLauncher.h"
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
	std::string DEFAULT_INVALID_SCRIPTS_PATH = "shell_scriptssss.txt";
	std::string ABNORMAL_SCRIPT_PATH = "shell_abnormal.txt";
	int DEFAULT_SCRIPTS_NUM = 4;
};

TEST_F(TestRunnerFixture, TestMatchingFIlePath) {
	//Arrange
	runner = new Runner(DEFAULT_SCRIPTS_PATH);

	//Act, Assert
	EXPECT_EQ(DEFAULT_SCRIPTS_PATH, runner->getFilePath());
	deleteRunner();
}

TEST_F(TestRunnerFixture, TestValidScriptsFile) {
	//Arrange
	runner = new Runner(DEFAULT_SCRIPTS_PATH);

	//Act
	int result = runner->parseInputScripts();

	//Assert
	EXPECT_EQ(result, 0);
	deleteRunner();
}

TEST_F(TestRunnerFixture, TestInvalidScriptsFile) {
	//Arrange
	runner = new Runner(DEFAULT_INVALID_SCRIPTS_PATH);

	//Act
	int result = runner->parseInputScripts();

	//Assert
	EXPECT_EQ(result, -1);
	deleteRunner();
}

TEST_F(TestRunnerFixture, TestParsingDefaultScripts) {
	//Arrange
	runner = new Runner(DEFAULT_SCRIPTS_PATH);

	//Act
	int result = runner->parseInputScripts();

	//Assert
	EXPECT_EQ(4, runner->getScriptsNum());
	 
	deleteRunner();
}

TEST_F(TestRunnerFixture, DISABLED_TestDefaultScripts) {
	//Arrange
	runner = new Runner(DEFAULT_SCRIPTS_PATH);

	//Act
	int result = runner->parseInputScripts();
	if (result != 0) {
		//parsing error;
		ASSERT_NE(result, 0);
	}
	result = runner->runScripts();

	//Assert
	EXPECT_EQ(result, 0);
	
	deleteRunner();
}