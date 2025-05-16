#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

using namespace std;

class Ssd {
public:
	void init(void);
	void write(int idx, int value);
	unsigned int read(int idx);
private:
	unsigned int storage[100] = { 0, };
};
