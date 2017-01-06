#include <mavkit/MavlinkTCP.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>

//----------------------------------------------------------------------------//
MavlinkTCP::MavlinkTCP(std::string target_ip, int target_port)
: reading_thread(NULL)
{
    if(target_port < 0 || target_port > 65535)
        throw std::logic_error(std::string("Port outside range."));

    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&gcAddr, 0, sizeof(struct sockaddr_in));
    gcAddr.sin_family = AF_INET;
    gcAddr.sin_port = htons(target_port);
    gcAddr.sin_addr.s_addr = inet_addr(target_ip.c_str());

    if(connect(sock, (struct sockaddr*) &gcAddr, sizeof(struct sockaddr)) == -1)
    {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
    }

    std::cout << "to " << target_ip << ":" << target_port << std::endl;

    reading_thread = new std::thread(&MavlinkTCP::read_loop, this);
}
//----------------------------------------------------------------------------//
MavlinkTCP::MavlinkTCP(int local_port)
: reading_thread(NULL)
{
    if(local_port < 0 || local_port > 65535)
        throw std::logic_error(std::string("Port outside range."));

    struct sockaddr_in locAddr;
    memset(&locAddr, 0, sizeof(struct sockaddr_in));
    locAddr.sin_family = AF_INET;
    locAddr.sin_port = htons(local_port);
    locAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to local_port
    int csock = socket(AF_INET, SOCK_STREAM, 0);
    if(bind(csock,(struct sockaddr*)&locAddr, sizeof(struct sockaddr)) == -1)
    {
        close(csock);
        throw std::logic_error(std::string("error bind failed: ") + strerror(errno));
    }

    if(listen(csock, 2) == -1)
    {
        perror("listen()");
        exit(errno);
    }

    struct sockaddr_in csin;
    memset(&csin, 0, sizeof(struct sockaddr_in));

    socklen_t sinsize = sizeof(csin);

    sock = accept(csock, (struct sockaddr*)&csin, &sinsize);

    if(sock == -1)
    {
        throw std::logic_error(std::string("error accept failed: ") + strerror(errno));
        exit(errno);
    }

    //make socket non blocking
    if (fcntl(sock, F_SETFL, FASYNC) < 0)
    {
        close(sock);
        throw std::logic_error(std::string("error setting nonblocking: ") + strerror(errno));
    }

    std::cout << "from " << inet_ntoa(locAddr.sin_addr) << ":" << local_port << std::endl;

    reading_thread = new std::thread(&MavlinkTCP::read_loop, this);
}
//----------------------------------------------------------------------------//
MavlinkTCP::~MavlinkTCP()
{
    close(sock);
}
//----------------------------------------------------------------------------//
bool MavlinkTCP::is_valid_ip(const char* ip)
{
    char* dst[INET_ADDRSTRLEN];
    return inet_pton(AF_INET, ip, dst) == 1;
}
//----------------------------------------------------------------------------//
void MavlinkTCP::append_listener(MavMessengerInterface* listener)
{
    if(listener != NULL)
        listeners.push_back(listener);
}
//----------------------------------------------------------------------------//
void MavlinkTCP::start()
{
    if(reading_thread == NULL)
        reading_thread = new std::thread(&MavlinkTCP::read_loop, this);
}
//----------------------------------------------------------------------------//
void MavlinkTCP::join()
{
    reading_thread->join();
}
//----------------------------------------------------------------------------//
void MavlinkTCP::read_loop()
{
    size_t length = 256;
    uint8_t buffer[length];
    mavlink_status_t status;
    mavlink_message_t msg;
    while(true)
    {
        socklen_t fromlen = sizeof(struct sockaddr);
        ssize_t nb_read = recvfrom(sock, (void *)buffer, length, 0, (struct sockaddr *)&gcAddr, &fromlen);

        if(nb_read == -1)
            throw std::logic_error("Unable to read from TCP socket.");

        for(int i=0;i<nb_read;i++)
        {
            if(mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status))
            {
                std::vector<MavMessengerInterface*>::iterator it = listeners.begin();
                for(;it != listeners.end();++it)
                {
                    (*it)->send_message(msg);
                }
            }
        }
    }
}
//----------------------------------------------------------------------------//
bool MavlinkTCP::send_message(mavlink_message_t &msg)
{
    uint8_t buf[512];
    ssize_t bytes_sent;
    uint16_t len;
    len = mavlink_msg_to_send_buffer(buf, &msg);

    mutex.lock();
    //TODO handle SIGPIPE to manage closed socket (and maybe go back to listening for servers)
    bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
    mutex.unlock();

    return len == bytes_sent;
}
//----------------------------------------------------------------------------//
