#include "gmock/gmock.h"
#include "ssd.h"
#include "command_parser.h"
#include "device_controller.h"
#include "buffer.h"

using namespace testing;
using namespace std;

//#define TEST_SHELL

int test_shell(void) {
	string device, cmd;
	int address;
	string hexData = "";

	while (cin >> device && device != "exit") {
		cin >> cmd >> address;

		if (cmd == "W")
			cin >> hexData;

		if (cin.fail()) {
			cerr << "입력 형식 오류!" << endl;
			cin.clear();
			cin.ignore(10000, '\n');
			continue;
		}

		SSD ssd;

		if (cmd == "R") {
			ssd.read(address);
		}
		else if (cmd == "W") {
			unsigned int data = 0;
			if (hexData.rfind("0x", 0) == 0) {
				data = std::stoul(hexData, nullptr, 16);
			}
			ssd.write(address, data);
		}
		else {
			cerr << "Unknown command: " << cmd << endl;
			return 1;
		}
	}

	return 0;
}

int main(int argc, char* argv[]) {
#if defined(_DEBUG)
	InitGoogleMock();
	return RUN_ALL_TESTS();
#elif defined(TEST_SHELL)
	return test_shell();
#else

	CommandParser commandParser(argc, argv);
	if (commandParser.isValid() == false)
		return 1;
	Buffer buffer;
	CommandInfo commandinfo = commandParser.getCommandInfo();
	DeviceController deviceController(&buffer.ssd, &buffer);
	
	return deviceController.run(commandinfo);
#endif
}

