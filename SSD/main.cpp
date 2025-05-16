#include "gmock/gmock.h"
#include "ssd.h"

using namespace testing;
using namespace std;

bool isOutOf4ByteRange(const std::string& hexStr) {
	try {
		unsigned long long value = std::stoull(hexStr, nullptr, 16);
		return value > 0xFFFFFFFFULL;
	}
	catch (const std::exception&) {
		return true;
	}
}

int main(void) {
#if defined(_DEBUG)
	InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	string device, cmd;
	int address;
	string hexData = "";

	while (true) {
		cin >> device >> cmd >> address;

		if (cmd == "W")
			cin >> hexData;

		if (cin.fail()) {
			cerr << "입력 형식 오류!" << endl;
			cin.clear();
			cin.ignore(10000, '\n');
			continue;
		}

		if (address < 0 || address > 99)
		{
			cerr << "LBA Out of Range" << endl;
			return 1;
		}
		SSD ssd;

		if (cmd == "R") {
			cout << "0x" << hex << uppercase << ssd.read(address) << endl;
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
#endif
}

