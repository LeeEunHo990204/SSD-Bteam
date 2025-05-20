#pragma once

#include <iostream>
#include <fstream>
#include <direct.h>
#include <io.h>
#include <string>
#include <vector>
#include <windows.h>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

struct CmdBuffer {
	string cmd;
	int LBA;
	int value;
};

class Buffer {
public:
	void initBuffer(void);
	void updateBuffer(void);
	void readBuffer(void);
	void writeBuffer(void);
#if !_DEBUG
private:
#endif
	vector<string> txtBuffer;
	vector<CmdBuffer> cmdBuffer;
	const string dirName = "buffer";
	const string txtPath = dirName + "\\ * .txt";
	bool isBufferDir(void) {
		return _access(dirName.c_str(), 0) == 0;
	}
	void readBufferDir(void);
	void convertCmdToTxt(void);
	void convertTxtToCmd(void);
	void printCmdBuffer(void);
	void printTxtBuffer(void);

	string toHexString(unsigned int value) {
		stringstream ss;
		ss << uppercase << hex << value;
		return ss.str();
	}
};
