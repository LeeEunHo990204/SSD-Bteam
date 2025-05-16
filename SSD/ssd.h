#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

using namespace std;

class SSD {
public:
	SSD();
	void write(int idx, int value);
	unsigned int read(int idx);
private:
	unsigned int storage[100] = { 0, };
	const string filename = "ssd_nand.txt";
	fstream file;
	const int LINE_LENGTH = 20;
	void init(void);
};
