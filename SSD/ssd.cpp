#include "ssd.h"

void Ssd::write(int idx, int value) {
	storage[idx] = value;
}

unsigned Ssd::read(int idx) {
	return storage[idx];
}