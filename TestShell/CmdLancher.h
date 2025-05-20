#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

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

        //cout << LBA << " " << val << endl;
        //cout << LBA << " " << val << " " << command.c_str() << endl;

        system(command.c_str());

    }
    string read(int LBA) override {
        //system("cp)
        if ((LBA >= 100) || (LBA < 0))
            return 0;

        string fileName = "SSD.exe";
        string command = fileName + " R " + to_string(LBA);
        system(command.c_str());

        ifstream infile("ssd_output.txt");
        string data;
        infile >> data;
        cout << "read : " << data << endl;
        infile.close();
        return data;// system("SSD.exe R 3");//TODO
    }
    bool erase(int LBA, int size) override {
        if ((LBA >= 100) || (LBA < 0))
            return false;
        int startAddress;
        int endAddress;
        string fileName = "SSD.exe";
        std::stringstream command;
        command << fileName << " E " << LBA << " " << size;
        cout << command.str() << endl;

        system(command.str().c_str());
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
protected:
    ICmdLauncher* m_cmdLauncher;
};

