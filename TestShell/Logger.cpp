#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>

class Logger {
public:
	void print(std::string className, std::string message) {
		std::string timeString = getTimeString();

	}

	void saveLog() {

	}

private:
	std::string getTimeString() {
		std::time_t now = std::time(nullptr);
		std::tm local_tm;
		localtime_s(&local_tm, &now);

		std::ostringstream oss;
		oss << std::string("[") << std::put_time(&local_tm, "%y.%m.%d %H:%M") << "]";
		return oss.str();
	}
};