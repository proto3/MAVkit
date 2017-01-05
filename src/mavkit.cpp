#include <mavkit/MavlinkUDP.h>
#include <mavkit/MavlinkSerial.h>
#include <mavkit/MavlinkLogReader.h>
#include <mavkit/MavlinkLogWriter.h>
#include <mavkit/MavlinkDisplay.h>
#include <iostream>
#include <unistd.h>
#include <argp.h>

MavMessengerInterface* master = NULL;
std::vector<MavMessengerInterface*> outputs;

//----------------------------------------------------------------------------//
void usage()
{
    std::cout << "usage: mavkit master output1 output2 ..." << std::endl;
    std::cout << "  master/output: IP address, tty path or log file." << std::endl;
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
int main(int argc, char* argv[])
{
    int c;
    while (true)
    {
        static struct option long_options[] =
        {
            /* These options set a flag. */
            //   {"verbose", no_argument,       &verbose_flag, 1},
            //   {"brief",   no_argument,       &verbose_flag, 0},
            /* These options don’t set a flag.
            We distinguish them by their indices. */
            {"tty",        required_argument, 0, 'a'},
            {"udp_client", required_argument, 0, 'b'},
            {"udp_server", required_argument, 0, 'c'},
            {"file",       required_argument, 0, 'd'},
            {"log",        no_argument,       0, 'e'},
            {"display",    no_argument,       0, 'f'},
            {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;
        c = getopt_long (argc, argv, "abcdef", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
            // case 0:
            // {
            //     /* If this option set a flag, do nothing else now. */
            //     if (long_options[option_index].flag != 0)
            //     break;
            //     printf ("option %s", long_options[option_index].name);
            //     if (optarg)
            //     printf (" with arg %s", optarg);
            //     printf ("\n");
            //     break;
            // }
            case 'a':
            {
                if(optind + 1 > argc)
                {
                    std::cout << "./mavkit: option \'--tty\' requires two arguments" << std::endl;
                    exit(0);
                }

                char* device   = argv[--optind];
                char* baudrate_str = argv[++optind];
                optind++;
                if(isdigit(baudrate_str))
                {
                    add_messenger(new MavlinkSerial(device, atoi(baudrate_str)));
                }
                else
                {
                    //TODO baudrate not a number
                    usage();
                    exit(0);
                }
                break;
            }
            case 'b':
            {
                if(optind + 1 > argc)
                {
                    std::cout << "./mavkit: option \'--udp_client\' requires two arguments" << std::endl;
                    exit(0);
                }

                char* ip       = argv[--optind];
                char* port_str = argv[++optind];
                optind++;

                if(isdigit(port_str))
                {
                    add_messenger(new MavlinkUDP(ip, atoi(port_str)));
                }
                else
                {
                    std::cout << "./mavkit: option \'--upd_client\' port is not a number" << std::endl;
                    exit(0);
                }
                break;
            }
            case 'c':
            {
                if(isdigit(optarg))
                {
                    add_messenger(new MavlinkUDP(atoi(optarg)));
                }
                else
                {
                    std::cout << "./mavkit: option \'--upd_server\' port is not a number" << std::endl;
                    exit(0);
                }
                break;
            }
            case 'd':
            {
                add_messenger(new MavlinkLogReader(optarg, 1.0));
                break;
            }
            case 'e':
            {
                add_messenger(new MavlinkLogWriter("log/"));
                break;
            }
            case 'f':
            {
                add_messenger(new MavlinkDisplay());
                break;
            }
            default:
              exit(0);
        }
    }

      /* Instead of reporting ‘--verbose’
         and ‘--brief’ as they are encountered,
         we report the final status resulting from them. */
    //   if (verbose_flag)
    //     puts ("verbose flag is set");

    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
        printf ("%s ", argv[optind++]);
        putchar ('\n');
    }

    if(master == NULL)
    {
        std::cerr << "Need at least a master messenger interface." << std::endl;
        exit(0);
    }

    master->start();
    for(int i=0;i<outputs.size();i++)
        outputs[i]->start();

    //send Mavlink2 heartbeat to force protocol version
    while(true)
    {
        //MSG1
        mavlink_message_t msg;
        mavlink_msg_heartbeat_pack(1, 2, &msg, MAV_TYPE_SUBMARINE, MAV_AUTOPILOT_GENERIC, MAV_MODE_PREFLIGHT, 0, MAV_STATE_STANDBY);
        master->send_message(msg);
        usleep(1000000);
    }
    // master->join();

    return 0;
}
//----------------------------------------------------------------------------//
