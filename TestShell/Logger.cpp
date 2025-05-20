#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <windows.h>

class Logger {
public:
	void print(std::string className, std::string message) {
		std::string timeString = getTimeString();
		std::ofstream file;

		file.open("../Log/latest.log", std::ios::app);
		std::ostringstream oss;
		oss << timeString << ' ' << std::left << std::setw(30) << className << " : " << message << std::endl;
		file << oss.str();
		file.close();
	}

	void printCurrentDirectory() {
		char buffer[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, buffer);
		std::cout << "현재 디렉토리: " << buffer << std::endl;
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