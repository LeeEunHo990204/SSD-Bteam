#include "gmock/gmock.h"
#include "TestScripts.h"

class TescScriptsFixture : public ::testing::Test
{
public:
	ITestScripts* testScripts;
	void factoryCreateTS1()
	{
		testScripts = new TestScripts1();
	}
	
	void deleteTS() {
		if (testScripts != nullptr)
			delete testScripts;
	}

	std::string EMPTY_SCRIPT_NAME = "empty_script";
};

TEST_F(TescScriptsFixture, TestCreateTS1instance)
{
	//Arrange, ACT
	//factoryCreateTS1();

	//Assert
	//EXPECT_NE(testScripts, nullptr);
}

