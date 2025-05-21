#pragma once
#include <string>
#include <vector>

bool startsWith(const std::string& str, const std::string& prefix);
bool isOutOf4ByteRange(const std::string& hexStr);
std::vector<std::string> splitBySpace(const std::string& input);