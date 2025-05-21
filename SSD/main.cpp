#include "gmock/gmock.h"
#include "ssd.h"
#include "command_parser.h"
#include "buffer_controller.h"
#include "buffer.h"

using namespace testing;
using namespace std;

int main(int argc, char* argv[]) {
#if defined(_DEBUG)
	InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	CommandParser commandParser(argc, argv);
	if (commandParser.isValid() == false)
		return 1;
	SSD& ssd = SSD::getInstance();
	Buffer buffer(ssd);
	CommandInfo commandinfo = commandParser.getCommandInfo();
	BufferController bufferController(&buffer);

	return bufferController.run(commandinfo);
#endif
}

