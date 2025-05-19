#include "gmock/gmock.h"
#include "TestScripts.h"


class MockLauncher : public ICmdLauncher {
public:
	MOCK_METHOD(void, write, (int LBA, unsigned int val), (override));
	MOCK_METHOD(unsigned int, read, (int LBA), (override));
};

class TestScriptsFixture : public ::testing::Test
{
public:
	ITestScripts* testScripts;
	void factoryCreateTS(std::string tsName)
	{
		testScripts = new TestScripts1(tsName);
	}

	void factoryCreateTSWithLauncher(std::string tsName, ICmdLauncher* launcher)
	{
		testScripts = new TestScripts1(tsName, launcher);
	}
	
	void deleteTS() {
		if (testScripts != nullptr)
			delete testScripts;
	}

	std::string EMPTY_SCRIPT_NAME = "empty_script";
	std::string TS1_NAME = "1_FullWriteAndReadCompare";
	std::string TS2_NAME = "2_PartialLBAWrite";
	std::string TS3_NAME = "3_PartialLBARead";

	SSDCmdLauncher ssdCmdLauncher;
	MockLauncher mockCmdLauncher;
};

TEST_F(TestScriptsFixture, TestCreateTS1instance)
{
	//Arrange
	factoryCreateTS(TS1_NAME);

	//ACT, Assert
	EXPECT_EQ(testScripts->getName(), TS1_NAME);

	deleteTS();
}

TEST_F(TestScriptsFixture, TestCreateTS2instance)
{
	//Arrange
	factoryCreateTS(TS2_NAME);

	//ACT, Assert
	EXPECT_EQ(testScripts->getName(), TS2_NAME);

	deleteTS();
}

TEST_F(TestScriptsFixture, TestCreateTS3instance)
{
	//Arrange
	factoryCreateTS(TS3_NAME);

	//ACT, Assert
	EXPECT_EQ(testScripts->getName(), TS3_NAME);

	deleteTS();
}

TEST_F(TestScriptsFixture, TestCreateTS1WithLauncher)
{
	//Arrange
	factoryCreateTSWithLauncher(TS1_NAME, &ssdCmdLauncher);

	//ACT, Assert
	EXPECT_EQ(testScripts->getName(), TS1_NAME);

	deleteTS();
}

TEST_F(TestScriptsFixture, TestCreateTS2WithLauncher)
{
	//Arrange
	factoryCreateTSWithLauncher(TS2_NAME, &ssdCmdLauncher);

	//ACT, Assert
	EXPECT_EQ(testScripts->getName(), TS2_NAME);

	deleteTS();
}

TEST_F(TestScriptsFixture, TestCreateTS3WithLauncher)
{
	//Arrange
	factoryCreateTSWithLauncher(TS3_NAME, &ssdCmdLauncher);

	//ACT, Assert
	EXPECT_EQ(testScripts->getName(), TS3_NAME);

	deleteTS();
}