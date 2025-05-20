#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <thread>
#include <ctime>
#include <string>

using namespace std;

class Logger {
public:
	void print(std::string className, std::string message) {

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
};