#include "gmock/gmock.h"
#include "ssd.h"
#include "command_parser.h"

using namespace testing;
using namespace std;


int main(int argc, char* argv[]) {
#if defined(_DEBUG)
	InitGoogleMock();
	return RUN_ALL_TESTS();
#else
    
    CommandParser commandParser(argc,argv);
    SSD ssd;
    
    cout << "[logging] CMD :" << commandParser.getCommand() << endl;
    cout << "[logging] argc :" << argc << endl;
    if (commandParser.getCommand() == "R") {
        ssd.read(commandParser.getLBA());
    }
    else if (commandParser.getCommand() == "W") {
        
        ssd.write(commandParser.getLBA(), commandParser.getData());
    }
    else {
        cerr << "Unknown command: " << commandParser.getCommand() << endl;
        return 1;
    }

	return 0;
#endif
}

