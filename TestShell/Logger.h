#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>

using namespace std;

class Logger {
public:
	static Logger& getInstance() {
		static Logger instance;
		return instance;
	}
	void print(std::string className, std::string message) {

		className = extractFunctionName(className);

		std::string timeString = getTimeString();

		mFile.open(mLogPath + mLogName, std::ios::app);
		std::ostringstream oss;
		oss << timeString << ' ' << std::left << std::setw(60) << className << " : " << message << std::endl;
		mFile << oss.str();
		mFile.flush();
		int file_size = getLogfileSize();
		mFile.close();


		if (file_size >= (1024 * 10)) {
			saveLog();
		}
	}

	void saveLog() {
		string logFileName;
		logFileName = getTimeString4FileName();
		if (mNewLogName.size() != 0) {
			if (mNewLogName != logFileName) //not same time stamp
				rename((mLogPath + mNewLogName + ".log").c_str(), (mLogPath + mNewLogName + ".zip").c_str());
			else
				rename((mLogPath + mNewLogName + ".log").c_str(), (mLogPath + mNewLogName + "_" + "sametime" + "_" + to_string(LogFileCnt) + ".zip").c_str());
		}
		rename((mLogPath + mLogName).c_str(), (mLogPath + logFileName + ".log").c_str());
		LogFileCnt++;
		mNewLogName = logFileName;
	}

private:
	Logger() {}
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	time_t mTimer = time(NULL);
	ostringstream mDataBuffer;
	ofstream mFile;
	const string mLogPath = "../Log/";
	const string mLogName = "latest.log";
	string mNewLogName = "";

	// For Test
	int LogFileCnt = 1;

	int getLogfileSize()
	{
		return mFile.tellp();
	}

	std::tm getTime() {
		std::time_t now = std::time(nullptr);
		std::tm local_tm;
		localtime_s(&local_tm, &now);

		return local_tm;
	}

	std::string getTimeString() {
		std::tm local_tm = getTime();

		std::ostringstream oss;
		oss << std::string("[") << std::put_time(&local_tm, "%y.%m.%d %H:%M") << "]";
		return oss.str();
	}

	std::string getTimeString4FileName() {
		std::tm local_tm = getTime();

		std::ostringstream oss;
		oss << "until_" << std::put_time(&local_tm, "%y%m%d_%Hh_%Mm_%Ss");
		return oss.str();
	}

	std::string extractFunctionName(const std::string& sig) {
		const std::vector<std::string> keywords = { "__cdecl", "__thiscall", "__stdcall", "__fastcall", "__vectorcall" };

		for (const auto& keyword : keywords) {
			size_t pos = sig.find(keyword);
			if (pos != std::string::npos) {
				return sig.substr(pos + keyword.length() + 1);
			}
		}

		size_t lastSpace = sig.rfind(' ');
		return (lastSpace != std::string::npos) ? sig.substr(lastSpace + 1) : sig;
	}
};

#if defined(__GNUC__) || defined(__clang__)
	#define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
	#define FUNCTION_SIGNATURE __FUNCSIG__
#else
	#define FUNCTION_SIGNATURE __func__  // fallback
#endif

#define LOGGING(message) \
    Logger::getInstance().print(FUNCTION_SIGNATURE, message);