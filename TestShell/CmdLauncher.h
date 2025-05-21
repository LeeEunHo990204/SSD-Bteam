#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include "Logger.h"

using namespace std;

#define MAX_LBA (99)
#define MIN_LBA (0)

class ICmdLauncher {
public:
    virtual void write(int LBA, unsigned int val) = 0;
    virtual string read(int LBA) = 0;
    virtual bool erase(int LBA, int size) = 0;
};

class SSDCmdLauncher : public ICmdLauncher {
public:
    SSDCmdLauncher() = default;
    void write(int LBA, unsigned int val) override {
        std::stringstream stream;
        stream << std::hex << val;
        string fileName = "SSD.exe";
        string command = fileName + " W " + to_string(LBA) + " 0x" + stream.str();

        LOGGING("Write LBA : " + to_string(LBA) + ", val : " + to_string(val));
        system(command.c_str());
    }
    string read(int LBA) override {
     
        string fileName = "SSD.exe";
        string command = fileName + " R " + to_string(LBA);
        LOGGING("Read LBA : " + to_string(LBA));
        system(command.c_str());

        ifstream infile("ssd_output.txt");
        string data;
        infile >> data;
        
        infile.close();
        return data;// system("SSD.exe R 3");//TODO
    }
    bool erase(int LBA, int size) override {
        int startAddress = 0;
        int endAddress = 0;
        string fileName = "SSD.exe";
        std::stringstream command;
        command << fileName << " E " << LBA << " " << size;
        cout << command.str() << endl;

        LOGGING("Erase LBA : " + to_string(LBA) + ", size : " + to_string(size));
        system(command.str().c_str());
        if ((LBA > MAX_LBA) || (LBA < 0) || size < 0 || size > 10)
            return false;
        return true;
    }
private:
    const string filename = "ssd_nand.txt";
};

class ShellDeviceDriver
{
public:
    ShellDeviceDriver(ICmdLauncher* cmdLauncher) : m_cmdLauncher(cmdLauncher) {}
    void write(int LBA, unsigned int val) {
        m_cmdLauncher->write(LBA, val);
    }
    string read(int LBA) {
        return m_cmdLauncher->read(LBA);
    }
    bool erase(int LBA, int size) {
        return m_cmdLauncher->erase(LBA, size);
    }
protected:
    ICmdLauncher* m_cmdLauncher;
};

