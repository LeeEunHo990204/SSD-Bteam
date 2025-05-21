#include "util.h"
#include <stdexcept>
#include <vector>
#include <sstream>

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

std::vector<std::string> splitBySpace(const std::string& input) {
	std::istringstream iss(input);
	std::vector<std::string> result;
	std::string word;

	if (input.empty()) {
		return result;
	}

	while (iss >> word) {
		result.push_back(word);
	}

	return result;
}