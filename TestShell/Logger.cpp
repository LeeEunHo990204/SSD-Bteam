#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;

class Logger {
public:
	void print(std::string className, std::string message) {
		std::string timeString = getTimeString();

	}

    void saveLog() {
        //mFile.open(mLogName);
        //for (int i = 0; i < (1024 * 3); i++) {
        //    mDataBuffer << "1";
        //}
        //mFile << mDataBuffer.str();

        int file_size = getLogfileSize();
        //cout << endl << "#############" << endl << mLogName << ":" << file_size << endl << "#############" << endl;
        if (file_size >= (1024 * 3)) {
            mFile.close();
            rename(mLogName.c_str(), mNewLogName.c_str());
            mFile.open(mLogName);
        }
    }

private:
    time_t mTimer = time(NULL);
    ostringstream mDataBuffer;
    ofstream mFile;
    const string mLogName = "latest.log";
    string mNewLogName = "timecode_TODO.log";
    
    int getLogfileSize()
    {
        return mFile.tellp();
    }

	std::string getTimeString() {
		std::time_t now = std::time(nullptr);
		std::tm local_tm;
		localtime_s(&local_tm, &now);

		std::ostringstream oss;
		oss << std::string("[") << std::put_time(&local_tm, "%y.%m.%d %H:%M") << "]";
		return oss.str();
	}
};