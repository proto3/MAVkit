#include <mavkit/MavMessengerInterface.h>
#include <mavkit/MavlinkUDP.h>
#include <mavkit/MavlinkSerial.h>
#include <mavkit/MavlinkLogReader.h>
#include <mavkit/MavlinkLogWriter.h>
#include <mavkit/MavlinkDisplay.h>

#include <iostream>
#include <chrono>
#include <fstream>
#include <cmath>
#include <unistd.h>
#include <thread>

#define BUFFER_LENGTH 512
#define SYS_ID 1
#define COMP_ID 2

using namespace std;
using namespace chrono;

MavMessengerInterface* master;
std::vector<MavMessengerInterface*> outputs;

//----------------------------------------------------------------------------//
// void usage()
// {
//     std::cout << "usage: mavkit master output1 output2 ..." << std::endl;
//     std::cout << "  master/output: IP address, tty path or log file." << std::endl;
// }
//----------------------------------------------------------------------------//
int main(int argc, char* argv[])
{
    // master = new MavlinkUDP("192.168.1.42", 14550);
    // master = new MavlinkUDP(14550);
    master = new MavlinkSerial("/dev/ttyACM0", 57600);
    // master = new MavlinkLogReader("log/log1");

    MavMessengerInterface* messenger;

    // messenger = new MavlinkUDP("192.168.1.42", 14550);
    // outputs.push_back(messenger);
    // master->append_listener(messenger);
    // messenger->append_listener(master);
    //
    messenger = new MavlinkUDP(14550);
    outputs.push_back(messenger);
    master->append_listener(messenger);
    messenger->append_listener(master);
    //
    // messenger = new MavlinkSerial("/dev/ttyACM0", 57600);
    // outputs.push_back(messenger);
    // master->append_listener(messenger);
    // messenger->append_listener(master);
    //
    // messenger = new MavlinkLogReader("log/log1");
    // outputs.push_back(messenger);
    // messenger->append_listener(master);
    //
    // messenger = new MavlinkLogWriter("log/");
    // outputs.push_back(messenger);
    // master->append_listener(messenger);

    messenger = new MavlinkDisplay();
    outputs.push_back(messenger);
    master->append_listener(messenger);

    master->join();

    return 0;
}
//----------------------------------------------------------------------------//
