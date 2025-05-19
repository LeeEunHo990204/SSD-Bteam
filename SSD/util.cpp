#include "util.h"
#include <stdexcept>

bool isOutOf4ByteRange(const std::string& hexStr) {
    try {
        unsigned long long value = std::stoull(hexStr, nullptr, 16);
        return value > 0xFFFFFFFFULL;
    }
    catch (const std::exception&) {
        return true;
    }
}