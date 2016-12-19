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

void process_mavlink_message(mavlink_message_t &message);
void reader_loop();
void writer_loop();

MavMessengerInterface* master;
std::vector<MavMessengerInterface*> outputs;

//----------------------------------------------------------------------------//
void usage()
{
    std::cout << "usage: mavkit master output1 output2 ..." << std::endl;
    std::cout << "  master/output: IP address, tty path or log file." << std::endl;
}
//----------------------------------------------------------------------------//
int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        usage();
        return 0;
    }

    if(MavlinkUDP::is_valid_ip(argv[1]))
    {
        master = new MavlinkUDP(argv[1], 14550, 14551);
    }
    else if(MavlinkSerial::is_valid_tty(argv[1]))
    {
        master = new MavlinkSerial(argv[1], 57600);
    }
    else if(MavlinkLogReader::is_valid_file(argv[1]))
    {
        master = new MavlinkLogReader(argv[1]);
    }
    else
    {
        usage();
        return 0;
    }

    for(int i=2;i<argc;i++)
    {
        if(MavlinkUDP::is_valid_ip(argv[i]))
        {
            MavMessengerInterface* mavlink = new MavlinkUDP(argv[i], 14550, 14551);
            outputs.push_back(mavlink);
            master->append_listener(mavlink);
            mavlink->append_listener(master);
        }
        else if(MavlinkSerial::is_valid_tty(argv[i]))
        {
            MavMessengerInterface* mavlink = new MavlinkSerial(argv[i], 57600);
            outputs.push_back(mavlink);
            master->append_listener(mavlink);
            mavlink->append_listener(master);
        }
        else if(MavlinkLogReader::is_valid_file(argv[i]))
        {
            MavMessengerInterface* mavlink = new MavlinkLogReader(argv[i]);
            outputs.push_back(mavlink);
            master->append_listener(mavlink);
            mavlink->append_listener(master);
        }
        else
        {
            usage();
            return 0;
        }
    }

    MavMessengerInterface* mavlink2 = new MavlinkLogWriter("log/");
    outputs.push_back(mavlink2);
    master->append_listener(mavlink2);
    mavlink2->append_listener(master);

    MavMessengerInterface* mavlink = new MavlinkDisplay();
    outputs.push_back(mavlink);
    master->append_listener(mavlink);
    mavlink->append_listener(master);

    std::thread write_thread(writer_loop);
    write_thread.join();

    return 0;
}
//----------------------------------------------------------------------------//
void writer_loop()
{
    mavlink_message_t msg;
    uint8_t buf[BUFFER_LENGTH];
    uint16_t len;

    while(true)
    {
        //get uptime in ms
        // std::chrono::milliseconds uptime(0u);
        // uint32_t uptime_ms = 0;
        // if(std::ifstream("/proc/uptime", std::ios::in) >> uptime_ms)
        //   uptime = std::chrono::milliseconds(static_cast<unsigned long long>(uptime_ms));

        //MSG1
        mavlink_msg_heartbeat_pack(SYS_ID, COMP_ID, &msg, MAV_TYPE_HELICOPTER, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
        len = mavlink_msg_to_send_buffer(buf, &msg);
        master->send_message(msg);
        //
        // //MSG2
        // if(step < 5.0 || step > 10.0)
        //     mavlink_msg_mission_set_current_pack(SYS_ID, COMP_ID, &msg, 1, 2, 1);
        // else
        //     mavlink_msg_mission_set_current_pack(SYS_ID, COMP_ID, &msg, 1, 2, 0);
        // len = mavlink_msg_to_send_buffer(buf, &msg);
        // mavlink->send_message(msg);
        //
        // //MSG3
        // mavlink_msg_named_value_float_pack(SYS_ID, COMP_ID, &msg, uptime_ms, "depth", 3.0);
        // len = mavlink_msg_to_send_buffer(buf, &msg);
        // mavlink->send_message(msg);
        //
        // //MSG4
        // mavlink_msg_local_position_ned_pack(SYS_ID, COMP_ID, &msg, uptime_ms, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        // len = mavlink_msg_to_send_buffer(buf, &msg);
        // mavlink->send_message(msg);

        usleep(1000000);
    }
}
//----------------------------------------------------------------------------//
