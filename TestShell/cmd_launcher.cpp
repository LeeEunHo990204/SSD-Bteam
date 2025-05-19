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
        ;//TODO
    }
    unsigned int read(int LBA) override {
        return 0;//TODO
    }
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
