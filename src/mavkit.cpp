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
#include <argp.h>

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
    bool master_assigned = false;

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
            {"udp_server", required_argument, 0, 'b'},
            {"udp_client", required_argument, 0, 'c'},
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

        MavMessengerInterface *messenger;
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
                messenger = new MavlinkSerial(optarg, 57600);
                if(!master_assigned)
                {
                    master = messenger;
                    master_assigned = true;
                }
                else
                {
                    outputs.push_back(messenger);
                    master->append_listener(messenger);
                    messenger->append_listener(master);
                }
                break;
            }
            case 'b':
            {
                messenger = new MavlinkUDP(optarg, 14550);
                if(!master_assigned)
                {
                    master = messenger;
                    master_assigned = true;
                }
                else
                {
                    outputs.push_back(messenger);
                    master->append_listener(messenger);
                    messenger->append_listener(master);
                }
                break;
            }
            case 'c':
            {
                messenger = new MavlinkUDP(atoi(optarg));//TODO test atoi
                if(!master_assigned)
                {
                    master = messenger;
                    master_assigned = true;
                }
                else
                {
                    outputs.push_back(messenger);
                    master->append_listener(messenger);
                    messenger->append_listener(master);
                }
                break;
            }
            case 'd':
            {
                messenger = new MavlinkLogReader(optarg);
                if(!master_assigned)
                {
                    master = messenger;
                    master_assigned = true;
                }
                else
                {
                    outputs.push_back(messenger);
                    master->append_listener(messenger);
                    messenger->append_listener(master);
                }
                break;
            }
            case 'e':
            {
                messenger = new MavlinkLogWriter("log/");
                if(!master_assigned)
                {
                    master = messenger;
                    master_assigned = true;
                }
                else
                {
                    outputs.push_back(messenger);
                    master->append_listener(messenger);
                    messenger->append_listener(master);
                }
                break;
            }
            case 'f':
            {
                messenger = new MavlinkDisplay();
                if(!master_assigned)
                {
                    master = messenger;
                    master_assigned = true;
                }
                else
                {
                    outputs.push_back(messenger);
                    master->append_listener(messenger);
                    messenger->append_listener(master);
                }
                break;
            }
            // default:
            //   abort ();
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

    master->join();

    return 0;
}
//----------------------------------------------------------------------------//
