#include "gmock/gmock.h"
#include "TestScripts.h"

class TescScriptsFixture : public ::testing::Test
{
public:
	ITestScripts* testScripts;
	void factoryCreateTS(std::string tsName)
	{
		testScripts = new TestScripts1(tsName);
	}
	
	void deleteTS() {
		if (testScripts != nullptr)
			delete testScripts;
	}

	std::string EMPTY_SCRIPT_NAME = "empty_script";
	std::string TS1_NAME = "1_FullWriteAndReadCompare";
	std::string TS2_NAME = "2_PartialLBAWrite";
	std::string TS3_NAME = "3_PartialLBARead";
};

TEST_F(TescScriptsFixture, TestCreateTS1instance)
{
	//Arrange
	factoryCreateTS(TS1_NAME);

	//ACT, Assert
	EXPECT_EQ(testScripts->getName(), TS1_NAME);

	deleteTS();
}

TEST_F(TescScriptsFixture, TestCreateTS2instance)
{
	//Arrange
	factoryCreateTS(TS2_NAME);

	//ACT, Assert
	EXPECT_EQ(testScripts->getName(), TS2_NAME);

	deleteTS();
}

TEST_F(TescScriptsFixture, TestCreateTS3instance)
{
	//Arrange
	factoryCreateTS(TS3_NAME);

	//ACT, Assert
	EXPECT_EQ(testScripts->getName(), TS3_NAME);

	deleteTS();
}

