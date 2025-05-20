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

#include "ssd.h"

using namespace std;

struct CmdBuffer {
	string cmd;
	int LBA;
	unsigned int value;
};

class Buffer {
public:
	Buffer() {
		readBuffer();
	}
	void initBuffer(void);
	void updateBuffer(string cmd, int LBA, unsigned int value);
	void readBuffer(void);
	void writeBuffer(void);
	void flushBuffer(void);
	SSD ssd;
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
	void resetBuffer(void);
	int returnCmdBufferIndex(void);
	void mergeBuffer(void);
	void updateStorage(void);

	string toHexString(unsigned int value) {
		stringstream ss;
		ss << uppercase << hex << value;
		return ss.str();
	}
};
