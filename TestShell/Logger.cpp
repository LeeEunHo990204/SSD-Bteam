#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;

class Logger {
public:
	void print(std::string className, std::string message) {
		std::string timeString = getTimeString();

        mFile.open(mLogPath + mLogName, std::ios::app);
		std::ostringstream oss;
		oss << timeString << ' ' << std::left << std::setw(30) << className << " : " << message << std::endl;
        mFile << oss.str();
        mFile.flush();
        int file_size = getLogfileSize();
        mFile.close();

        if (file_size >= (1024 * 10)) {
            saveLog();
        }
	}

    void saveLog() {
        //for (int i = 0; i < (1024 * 3); i++) {
        //    mDataBuffer << "1";
        //}
        //mFile << mDataBuffer.str();
        //cout << endl << "#############" << endl << mLogName << ":" << file_size << endl << "#############" << endl;
        rename((mLogPath + mLogName).c_str(), (mLogPath + to_string(LogFileCnt) + ".log").c_str());
        LogFileCnt++;
    }

private:
    time_t mTimer = time(NULL);
    ostringstream mDataBuffer;
    ofstream mFile;
    const string mLogPath = "../Log/";
    const string mLogName = "latest.log";
    string mNewLogName = "timecode_TODO.log";
    
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
        cout << "!!";
        oss << "until_" << std::put_time(&local_tm, "%y%m%d_%Hh_%Mm_%Ss") << ".log";
        return oss.str();
    }
};