#include "util.h"
#include <stdexcept>

bool startsWith(const std::string& str, const std::string& prefix) {
	return str.size() >= prefix.size() &&
		str.compare(0, prefix.size(), prefix) == 0;
}

bool isOutOf4ByteRange(const std::string& hexStr) {
    try {
        unsigned long long value = std::stoull(hexStr, nullptr, 16);
        return value > 0xFFFFFFFFULL;
    }
    catch (const std::exception&) {
        return true;
    }
}