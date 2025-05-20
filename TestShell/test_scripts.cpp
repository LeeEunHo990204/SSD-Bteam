#include "gmock/gmock.h"
#include "TestScripts.h"

class MockLauncher : public ICmdLauncher {
public:
	MOCK_METHOD(void, write, (int LBA, unsigned int val), (override));
	MOCK_METHOD(string, read, (int LBA), (override));
	MOCK_METHOD(bool, erase, (int LBA, int size), (override));
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
		if (tsName == TS1_NAME) {
			testScripts = new TestScripts1(tsName, launcher);
		}
		else if (tsName == TS2_NAME) {
			testScripts = new TestScripts2(tsName, launcher);
		}
		else if (tsName == TS3_NAME) {
			testScripts = new TestScripts3(tsName, launcher);
		}
		else if (tsName == BASIC_NAME) {
			testScripts = new TestScripts1(tsName, launcher);
		}
		else {
			testScripts = nullptr;
		}
	}

	void deleteTS() {
		if (testScripts != nullptr)
			delete testScripts;
	}

	std::string EMPTY_SCRIPT_NAME = "empty_script";
	std::string TS1_NAME = "1_FullWriteAndReadCompare";
	std::string TS2_NAME = "2_PartialLBAWrite";
	std::string TS3_NAME = "3_PartialLBARead";
	std::string BASIC_NAME = "BasicTest";

	int RAND_NUM = 0x00110011;
	int RAND_LBA = 0x10;

	SSDCmdLauncher ssdCmdLauncher;
	MockLauncher mockLauncher;
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

TEST_F(TestScriptsFixture, TestCreateMockLauncher)
{
	//Arrange
	factoryCreateTSWithLauncher(TS1_NAME, &mockLauncher);
	//ACT, Assert
	EXPECT_EQ(testScripts->getName(), TS1_NAME);
	deleteTS();
}

TEST_F(TestScriptsFixture, TestMockBasicReadTest)
{
	//Arrange
	factoryCreateTSWithLauncher(BASIC_NAME, &mockLauncher);
	EXPECT_CALL(mockLauncher, read(::testing::_))
		.Times(1)
		.WillOnce(testing::Return("0x00000000"));
	//Act
	unsigned int readVal = std::stoul(testScripts->getShellDev()->read(0x00), nullptr, 16);
	//Assert
	EXPECT_EQ(readVal, 0x00000000);
	deleteTS();
}

TEST_F(TestScriptsFixture, TestMockBasicWriteTest)
{
	//Arrange
	factoryCreateTSWithLauncher(BASIC_NAME, &mockLauncher);
	EXPECT_CALL(mockLauncher, write(::testing::_, ::testing::_))
		.Times(1);
	//Act, Asser
	testScripts->getShellDev()->write(0x00, 0x00000000);
	deleteTS();
}

TEST_F(TestScriptsFixture, TestReadCompareDataMatch)
{
	//Arrange
	factoryCreateTSWithLauncher(BASIC_NAME, &mockLauncher);
	std::stringstream ss;
	ss << std::hex << std::uppercase << RAND_NUM;

	EXPECT_CALL(mockLauncher, write(RAND_LBA, RAND_NUM))
		.Times(1);
	EXPECT_CALL(mockLauncher, read(RAND_LBA))
		.Times(1)
		.WillOnce(testing::Return(ss.str()));

	//Act
	int readCompareResult = testScripts->readCompare(RAND_LBA, RAND_NUM);

	//Assert
	EXPECT_EQ(readCompareResult, READ_COMPARE_DATA_MATCH);
	deleteTS();
}

TEST_F(TestScriptsFixture, TestReadCompareDataMisMatch)
{
	//Arrange
	factoryCreateTSWithLauncher(BASIC_NAME, &mockLauncher);
	EXPECT_CALL(mockLauncher, write(RAND_LBA, RAND_NUM))
		.Times(1);
	EXPECT_CALL(mockLauncher, read(RAND_LBA))
		.Times(1)
		.WillOnce(testing::Return("0xFFFFFFFF"));

	//Act
	int readCompareResult = testScripts->readCompare(RAND_LBA, RAND_NUM);

	//Assert
	EXPECT_EQ(readCompareResult, READ_COMPARE_DATA_MISMATCH);
	deleteTS();
}

TEST_F(TestScriptsFixture, TestScript1)
{
	//Arrange
	int counter = 0;
	factoryCreateTSWithLauncher(TS1_NAME, &mockLauncher);

	EXPECT_CALL(mockLauncher, write(::testing::_, ::testing::_))
		.Times(100);
	EXPECT_CALL(mockLauncher, read(::testing::_))
		.Times(100)
		.WillRepeatedly(testing::Invoke([&counter](int) {
			std::stringstream ss;
			ss << std::hex << std::uppercase << counter++;
			return ss.str();
		}));

	//Act
	testScripts->runTestScenario();
	int testResult = testScripts->getResult();

	//Assert
	EXPECT_EQ(testResult, TEST_PASS);
}

TEST_F(TestScriptsFixture, TestScript2)
{
	int pattern[] = { 0x400, 0x000, 0x300, 0x100, 0x200 };
	int call_count = 0;

	//Arrange
	factoryCreateTSWithLauncher(TS2_NAME, &mockLauncher);

	EXPECT_CALL(mockLauncher, write(::testing::_, ::testing::_))
		.Times(150);
	EXPECT_CALL(mockLauncher, read(::testing::_))
		.Times(150)
		.WillRepeatedly(testing::Invoke([&call_count, &pattern](int) {
			int ret = pattern[call_count % 5];
			++call_count;

			std::stringstream ss;
			ss << std::hex << std::uppercase << ret;  // 대문자 16진수 ("1A" 등)
			return ss.str();
		}));

	//Act
	testScripts->runTestScenario();
	int testResult = testScripts->getResult();

	//Assert
	EXPECT_EQ(testResult, TEST_PASS);
}

#if 0
TEST_F(TestScriptsFixture, TestScript3)
{
	int pattern[] = { 0x10101010, 0x5a5aa5a5 };
	int call_count = 0;

	//Arrange
	factoryCreateTSWithLauncher(TS3_NAME, &mockLauncher);

	EXPECT_CALL(mockLauncher, write(::testing::_, ::testing::_))
		.Times(400);
	EXPECT_CALL(mockLauncher, read(::testing::_))
		.Times(400)
		.WillRepeatedly(testing::Invoke([&call_count, &pattern](int) {
		int ret = pattern[call_count % 2];
		++call_count;
		return ret;
			}));

	//Act
	testScripts->runTestScenario();
	int testResult = testScripts->getResult();

	//Assert
	EXPECT_EQ(testResult, TEST_PASS);
}
#endif
