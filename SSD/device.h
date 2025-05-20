#pragma once
class IDevice {
public:
	virtual void write(int idx, int value) = 0;
	virtual unsigned read(int idx) = 0;
	virtual bool erase(int idx, int size) = 0;
};