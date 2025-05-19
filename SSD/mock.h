#pragma once
#include "gmock/gmock.h"
#include "device.h"

class MockDevice : public IDevice {
public:
	MOCK_METHOD(void, write, (int, int), (override));
	MOCK_METHOD(unsigned int, read, (int), (override));
};