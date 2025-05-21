#include "CmdLauncher.h"

void SSDCmdLauncher::write(int LBA, unsigned int val) {
    std::stringstream stream;
    stream << std::hex << val;
    string fileName = "SSD.exe";
    string command = fileName + " W " + to_string(LBA) + " 0x" + stream.str();

    LOGGING("Write LBA : " + to_string(LBA) + ", val : " + to_string(val));
    system(command.c_str());
}
string SSDCmdLauncher::read(int LBA) {

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

bool SSDCmdLauncher::erase(int LBA, int size) {
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
void SSDCmdLauncher::flush() {
    std::stringstream stream;

    string fileName = "SSD.exe";
    string command = fileName + " F ";

    //cout << LBA << " " << val << endl;
    //cout << LBA << " " << val << " " << command.c_str() << endl;

    system(command.c_str());

}

void ShellDeviceDriver::write(int LBA, unsigned int val) {
    m_cmdLauncher->write(LBA, val);
}
string ShellDeviceDriver::read(int LBA) {
    return m_cmdLauncher->read(LBA);
}
bool ShellDeviceDriver::erase(int LBA, int size) {
    return m_cmdLauncher->erase(LBA, size);
}