#include <iostream>
#include <unistd.h>

#include <mavkit/MavMessengerInterface.h>
#include <mavkit/MavlinkUDP.h>
#include <mavkit/MavlinkSerial.h>

#define BUFFER_LENGTH 512
#define SYS_ID 1
#define COMP_ID 1

void process_mavlink_message(mavlink_message_t &message);

//----------------------------------------------------------------------------//
void usage()
{
    std::cout << "usage: mavkit link" << std::endl;
    std::cout << "  link: IP address or a tty path." << std::endl;
}
//----------------------------------------------------------------------------//
int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        usage();
        return 0;
    }

    MavMessengerInterface* mavlink;
    if(MavlinkUDP::is_valid_ip(argv[1]))
    {
        mavlink = new MavlinkUDP(argv[1], 14550, 14551);
    }
    else if(MavlinkSerial::is_valid_file(argv[1]))
    {
        mavlink = new MavlinkSerial(argv[1], 57600);
    }
    else
    {
        usage();
        return 0;
    }

    mavlink_message_t msg;
    uint8_t buf[BUFFER_LENGTH];
    uint16_t len;
    while(true)
    {
        //SEND-----------------------------------------//
        //MSG1
        mavlink_msg_heartbeat_pack(SYS_ID, COMP_ID, &msg, MAV_TYPE_HELICOPTER, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
        len = mavlink_msg_to_send_buffer(buf, &msg);
        mavlink->send_message(msg);

        //MSG2
        mavlink_msg_mission_set_current_pack(SYS_ID, COMP_ID, &msg, 1, 2, 1);
        len = mavlink_msg_to_send_buffer(buf, &msg);
        mavlink->send_message(msg);

        //MSG3
        //...

        //RECEIVE-------------------------------------//
        while(mavlink->receive_message(msg))
            process_mavlink_message(msg);

        usleep(100000);
    }

    return 0;
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

            //access message specific fields
            std::cout << "    type:            "            << (uint)heartbeat.type << std::endl;
            std::cout << "    autopilot:       "       << (uint)heartbeat.autopilot << std::endl;
            std::cout << "    base_mode:       "       << (uint)heartbeat.base_mode << std::endl;
            std::cout << "    custom_mode:     "     << (uint)heartbeat.custom_mode << std::endl;
            std::cout << "    system_status:   "   << (uint)heartbeat.system_status << std::endl;
            std::cout << "    mavlink_version: " << (uint)heartbeat.mavlink_version << std::endl;
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
