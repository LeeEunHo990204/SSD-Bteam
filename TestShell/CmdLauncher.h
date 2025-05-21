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
    virtual void flush() = 0;
};

class SSDCmdLauncher : public ICmdLauncher {
public:
    SSDCmdLauncher() = default;
    void write(int LBA, unsigned int val);
    string read(int LBA);
    bool erase(int LBA, int size);
    void flush();
private:
    const string filename = "ssd_nand.txt";
};

class ShellDeviceDriver
{
public:
    ShellDeviceDriver(ICmdLauncher* cmdLauncher) : m_cmdLauncher(cmdLauncher) {}
    void write(int LBA, unsigned int val);
    string read(int LBA);
    bool erase(int LBA, int size);
protected:
    ICmdLauncher* m_cmdLauncher;
};