#include "gmock/gmock.h"
#include "CmdLancher.h"
#include "TestScripts.h"
#include "Runner.h"

class MockTestScripts : public ITestScripts {

};

class TestRunnerFixture : public ::testing::Test
{

};