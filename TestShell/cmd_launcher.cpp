#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

using namespace std;

class ICmdLauncher {
public:
	virtual void write(int LBA, unsigned int val) = 0;
	virtual unsigned int read(int LBA) = 0;
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
    unsigned int read(int LBA) override {
        //system("cp)
        string fileName = "SSD.exe";
        string command = fileName + " R " + to_string(LBA);
        system(command.c_str());

        ifstream infile("ssd_output.txt");
        string str;
        unsigned int data = 0;
        infile >> str;
        cout << "read : " << str << endl;
        infile.close();
        if (str != "ERROR") {
            data = stoul(str, nullptr, 16);
        }
        return data;// system("SSD.exe R 3");//TODO
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
    unsigned int read(int LBA) {
        return m_cmdLauncher->read(LBA);
    }
protected:
    ICmdLauncher* m_cmdLauncher;
};
