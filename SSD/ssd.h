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
	static SSD& getInstance(void);
	void init(void);
	void write(int idx, unsigned int value);
	bool erase(int idx, int size);
	unsigned int read(int idx);
	unsigned int get(int idx);
	void set(int idx, unsigned int value);
private:
	SSD();
	~SSD();
	unsigned int storage[STORAGE_SIZE] = { 0, };
	const string nandFilename = "ssd_nand.txt";
	const string outputFilename = "ssd_output.txt";
	fstream file;
	const int LINE_LENGTH = 20;
	void createNandTxtFile(void);
	void updateStorageFromNandTxtFile(void);
	bool eraseInNandTxtFile(int idx, int size);
	bool isAddressValid(int idx);
};
