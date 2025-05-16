#pragma once

class Ssd {
public:
	void write(int idx, int value);
	unsigned int read(int idx);
private:
	unsigned int storage[100] = { 0, };
};
