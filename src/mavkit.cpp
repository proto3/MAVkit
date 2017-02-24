#include <mavkit/Messengers.h>
#include <unistd.h>
#include <iostream>
#include <map>

enum messenger_type {
    TTY,
    UDP_CLT,
    UDP_SRV,
    TCP_CLT,
    TCP_SRV,
    FILE_W,
    FILE_R,
    DISPLAY,
    HELP
};

MavMessengerInterface* master = NULL;
std::vector<MavMessengerInterface*> outputs;

//----------------------------------------------------------------------------//
void usage()
{
    std::cout << "Usage:\n";
    std::cout << "  mavkit <master> <messenger1> <messenger2> ...\n\n";

    std::cout << "  Master messenger is duplex connected to every secondary connector.\n";
    std::cout << "  Secondary messengers are not connected between them.\n\n";

    std::cout << "  List of connectors :\n";
    std::cout << "    --tty        <baudrate> <device>\n";
    std::cout << "    --udp_client <address> <port>\n";
    std::cout << "    --udp_server <port>\n";
    std::cout << "    --tcp_client <address> <port>\n";
    std::cout << "    --tcp_server <port>\n";
    std::cout << "    --log\n";
    std::cout << "    --replay     <speed> <start>\n";
    std::cout << "    --display" << std::endl;
}
//----------------------------------------------------------------------------//
void add_messenger(MavMessengerInterface* messenger)
{
    if(master == NULL)
    {
        master = messenger;
    }
    else
    {
        outputs.push_back(messenger);
        master->append_listener(messenger);
        messenger->append_listener(master);
    }
}
//----------------------------------------------------------------------------//
bool isdigit(char *str)
{
    char c = str[0];
    int i = 1;
    while(c != '\0')
    {
        if(!isdigit(c))
        {
            return false;
        }
        c = str[i];
        i++;
    }
    return i > 1;
}
//----------------------------------------------------------------------------//
int parse_messenger_type(char *arg)
{
    std::map<std::string, messenger_type> type_map = {
        { "--tty", TTY },
        { "--udp_client", UDP_CLT },
        { "--udp_server", UDP_SRV },
        { "--tcp_client", TCP_CLT },
        { "--tcp_server", TCP_SRV },
        { "--log", FILE_W },
        { "--replay", FILE_R },
        { "--display", DISPLAY },

        { "-h",      HELP },
        { "--h",     HELP },
        { "-help",   HELP },
        { "--help",  HELP },
        { "-usage",  HELP },
        { "--usage", HELP }
    };

    if(type_map.find(arg) != type_map.end())
    {
        return type_map[arg];
    }
    else
    {
        return -1;
    }
}
//----------------------------------------------------------------------------//
char* get_next_arg(int &index, int argc, char** argv)
{
    if(index < argc)
        return argv[index++];

    return NULL;
}
//----------------------------------------------------------------------------//
bool create_tty(int &index, int argc, char** argv)
{
    char* baudrate_str = get_next_arg(index, argc, argv);
    if(baudrate_str == NULL)
    {
        std::cout << "Error : Missing baudrate." << std::endl;
        usage();
        return false;
    }
    char* device_str = get_next_arg(index, argc, argv);
    if(device_str == NULL)
    {
        std::cout << "Error : Missing device." << std::endl;
        usage();
        return false;
    }

    if(isdigit(baudrate_str))
    {
        add_messenger(new MavlinkSerial(device_str, atoi(baudrate_str)));
        return true;
    }
    else
    {
        std::cout << "Error : Baudrate not a number." << std::endl;
        usage();
        return false;
    }
}
//----------------------------------------------------------------------------//
bool create_udp_client(int &index, int argc, char** argv)
{
    char* ip_str = get_next_arg(index, argc, argv);
    if(ip_str == NULL)
    {
        std::cout << "Error : Missing IP." << std::endl;
        usage();
        return false;
    }
    char* port_str = get_next_arg(index, argc, argv);
    if(port_str == NULL)
    {
        std::cout << "Error : Missing port." << std::endl;
        usage();
        return false;
    }

    if(isdigit(port_str))
    {
        add_messenger(new MavlinkUDP(ip_str, atoi(port_str)));
        return true;
    }
    else
    {
        std::cout << "Error : Port not a number." << std::endl;
        usage();
        return false;
    }
}
//----------------------------------------------------------------------------//
bool create_udp_server(int &index, int argc, char** argv)
{
    char* port_str = get_next_arg(index, argc, argv);
    if(port_str == NULL)
    {
        std::cout << "Error : Missing port." << std::endl;
        usage();
        return false;
    }

    if(isdigit(port_str))
    {
        add_messenger(new MavlinkUDP(atoi(port_str)));
        return true;
    }
    else
    {
        std::cout << "Error : Port not a number." << std::endl;
        usage();
        return false;
    }
}
//----------------------------------------------------------------------------//
bool create_tcp_client(int &index, int argc, char** argv)
{
    char* ip_str = get_next_arg(index, argc, argv);
    if(ip_str == NULL)
    {
        std::cout << "Error : Missing IP." << std::endl;
        usage();
        return false;
    }
    char* port_str = get_next_arg(index, argc, argv);
    if(port_str == NULL)
    {
        std::cout << "Error : Missing port." << std::endl;
        usage();
        return false;
    }

    if(isdigit(port_str))
    {
        add_messenger(new MavlinkTCP(ip_str, atoi(port_str)));
        return true;
    }
    else
    {
        std::cout << "Error : Port not a number." << std::endl;
        usage();
        return false;
    }
}
//----------------------------------------------------------------------------//
bool create_tcp_server(int &index, int argc, char** argv)
{
    char* port_str = get_next_arg(index, argc, argv);
    if(port_str == NULL)
    {
        std::cout << "Error : Missing port." << std::endl;
        usage();
        return false;
    }

    if(isdigit(port_str))
    {
        add_messenger(new MavlinkTCP(atoi(port_str)));
        return true;
    }
    else
    {
        std::cout << "Error : Port not a number." << std::endl;
        usage();
        return false;
    }
}
//----------------------------------------------------------------------------//
bool create_file(int &index, int argc, char** argv)
{
    char* file_str = get_next_arg(index, argc, argv);
    if(file_str == NULL)
    {
        std::cout << "Error : Missing filename." << std::endl;
        usage();
        return false;
    }

    char* speed_str = get_next_arg(index, argc, argv);
    if(speed_str == NULL)
    {
        std::cout << "Error : Missing reading speed." << std::endl;
        usage();
        return false;
    }

    char* start_str = get_next_arg(index, argc, argv);
    if(start_str == NULL)
    {
        std::cout << "Error : Missing starting time." << std::endl;
        usage();
        return false;
    }

    add_messenger(new MavlinkLogReader(file_str, atof(speed_str), atof(start_str)));
    return true;
}
//----------------------------------------------------------------------------//
bool create_log(int &index, int argc, char** argv)
{
    add_messenger(new MavlinkLogWriter("/log"));
    return true;
}
//----------------------------------------------------------------------------//
bool create_display(int &index, int argc, char** argv)
{
    add_messenger(new MavlinkDisplay());
    return true;
}
//----------------------------------------------------------------------------//
int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        usage();
        return 0;
    }

    int arg_i = 1;
    while(arg_i < argc)
    {
        char* messenger_str = get_next_arg(arg_i, argc, argv);
        int messenger_type = parse_messenger_type(messenger_str);

        if(messenger_type == -1)
        {
            std::cout << "Error : " + std::string(messenger_str) + " not a valid messenger." << std::endl;
            usage();
            return -1;
        }

        switch (messenger_type)
        {
            case TTY:
                create_tty(arg_i, argc, argv);
                break;
            case UDP_CLT:
                create_udp_client(arg_i, argc, argv);
                break;
            case UDP_SRV:
                create_udp_server(arg_i, argc, argv);
                break;
            case TCP_CLT:
                create_tcp_client(arg_i, argc, argv);
                break;
            case TCP_SRV:
                create_tcp_server(arg_i, argc, argv);
                break;
            case FILE_R:
                create_file(arg_i, argc, argv);
                break;
            case FILE_W:
                create_log(arg_i, argc, argv);
                break;
            case DISPLAY:
                create_display(arg_i, argc, argv);
                break;
            case HELP:
                usage();
                return 0;
        }
    }

    if(master == NULL)
        return -1;

    master->start();
    for(int i = 0 ; i < outputs.size() ; i++)
        outputs[i]->start();

    // send Mavlink2 heartbeat to force protocol version
    while(true)
    {
        mavlink_message_t msg;
        mavlink_msg_heartbeat_pack(1, 2, &msg, MAV_TYPE_GENERIC, MAV_AUTOPILOT_INVALID, 0, 0, MAV_STATE_STANDBY);
        master->send_message(msg);
        usleep(1000000);
    }
    // master->join();

    return 0;
}
//----------------------------------------------------------------------------//
