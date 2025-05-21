#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

#include "device.h"

#define STORAGE_SIZE 100

using namespace std;

class SSD : public IDevice {
public:
	SSD();
	~SSD();
	void init(void);
	void write(int idx, unsigned int value);
	bool erase(int idx, int size);
	unsigned int read(int idx);
	bool isAddressValid(int idx);
	unsigned int get(int idx);
	void set(int idx, unsigned int value);
private:
	unsigned int storage[STORAGE_SIZE] = { 0, };
	const string filename = "ssd_nand.txt";
	fstream file;
	const int LINE_LENGTH = 20;
};
