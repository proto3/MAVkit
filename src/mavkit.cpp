#include <mavkit/MavMessengerInterface.h>
#include <mavkit/MavlinkUDP.h>
#include <mavkit/MavlinkSerial.h>
#include <mavkit/MavlinkFile.h>

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

MavMessengerInterface* mavlink;

//----------------------------------------------------------------------------//
void usage()
{
    std::cout << "usage: mavkit link" << std::endl;
    std::cout << "  link: IP address, tty path or log file." << std::endl;
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
        mavlink = new MavlinkUDP(argv[1], 14550, 14551);
    }
    else if(MavlinkSerial::is_valid_tty(argv[1]))
    {
        mavlink = new MavlinkSerial(argv[1], 57600);
    }
    else if(MavlinkFile::is_valid_file(argv[1]))
    {
        mavlink = new MavlinkFile(argv[1]);
    }
    else
    {
        usage();
        return 0;
    }

    std::thread read_thread(reader_loop);
    std::thread write_thread(writer_loop);

    read_thread.join();
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
        mavlink->send_message(msg);
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
void reader_loop()
{
    mavlink_message_t msg;
    while(true)
    {
        while(mavlink->receive_message(msg))
        {
            process_mavlink_message(msg);
        }
    }
}
//----------------------------------------------------------------------------//
void process_mavlink_message(mavlink_message_t &msg)
{
    switch(msg.msgid)
    {
        case MAVLINK_MSG_ID_HEARTBEAT:
        {
            mavlink_heartbeat_t heartbeat;
            mavlink_msg_heartbeat_decode(&msg, &heartbeat);
            std::cout << "heartbeat" << std::endl;

            // access message specific fields
            // std::cout << "    type:            "            << (uint)heartbeat.type << std::endl;
            // std::cout << "    autopilot:       "       << (uint)heartbeat.autopilot << std::endl;
            // std::cout << "    base_mode:       "       << (uint)heartbeat.base_mode << std::endl;
            // std::cout << "    custom_mode:     "     << (uint)heartbeat.custom_mode << std::endl;
            // std::cout << "    system_status:   "   << (uint)heartbeat.system_status << std::endl;
            // std::cout << "    mavlink_version: " << (uint)heartbeat.mavlink_version << std::endl;
            break;
        }
        case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
        {
            mavlink_param_request_list_t param_request_list;
            mavlink_msg_param_request_list_decode(&msg, &param_request_list);
            std::cout << "param request list" << std::endl;
            break;
        }
        case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
        {
            mavlink_local_position_ned_t local_position_ned;
            mavlink_msg_local_position_ned_decode(&msg, &local_position_ned);
            std::cout << "local position ned" << std::endl;
            break;
        }
        case MAVLINK_MSG_ID_MISSION_SET_CURRENT:
        {
            mavlink_mission_set_current_t mission_set_current;
            mavlink_msg_mission_set_current_decode(&msg, &mission_set_current);
            std::cout << "mission set current" << std::endl;
            break;
        }
        case MAVLINK_MSG_ID_REQUEST_DATA_STREAM:
        {
            mavlink_request_data_stream_t request_data_stream;
            mavlink_msg_request_data_stream_decode(&msg, &request_data_stream);
            std::cout << "request data stream" << std::endl;
            break;
        }
        case MAVLINK_MSG_ID_NAMED_VALUE_FLOAT:
        {
            mavlink_named_value_float_t named_value_float;
            mavlink_msg_named_value_float_decode(&msg, &named_value_float);
            std::cout << "named value float" << std::endl;
            break;
        }
        case MAVLINK_MSG_ID_STATUSTEXT:
        {
            mavlink_statustext_t statustext;
            mavlink_msg_statustext_decode(&msg, &statustext);
            std::cout << "status text" << std::endl;
            std::cout << statustext.text << std::endl;
            break;
        }
        default:
        {
            std::cout << "Unsupported packet -> ";
            std::cout << " SYS: "     << (int)msg.sysid;
            std::cout << ", COMP: "   << (int)msg.compid;
            std::cout << ", SEQ: "    << (int)msg.seq;
            std::cout << ", LEN: "    << (int)msg.len;
            std::cout << ", MSG ID: " << (int)msg.msgid << std::endl;
            break;
        }
    }
    std::cout << "----------------------" << std::endl;
}
//----------------------------------------------------------------------------//
