#include <mavkit/MavlinkUDP.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>

//----------------------------------------------------------------------------//
uint64_t microsSinceEpoch()
{
    struct timeval tv;
    uint64_t micros = 0;

    gettimeofday(&tv, NULL);
    micros = ((uint64_t)tv.tv_sec) * 1000000 + tv.tv_usec;

    return micros;
}
//----------------------------------------------------------------------------//
MavlinkUDP::MavlinkUDP(std::string target_ip, int target_port, int local_port)
{
    struct sockaddr_in locAddr;
    memset(&locAddr, 0, sizeof(struct sockaddr_in));
    locAddr.sin_family = AF_INET;
    locAddr.sin_port = htons(local_port);
    locAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to local_port
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(bind(sock,(struct sockaddr *)&locAddr, sizeof(struct sockaddr)) == -1)
    {
        close(sock);
        throw std::logic_error(std::string("error bind failed: ") + strerror(errno));
    }

    //make socket non blocking
    if (fcntl(sock, F_SETFL, FASYNC) < 0)
    {
        close(sock);
        throw std::logic_error(std::string("error setting nonblocking: ") + strerror(errno));
    }

    memset(&gcAddr, 0, sizeof(struct sockaddr_in));
    gcAddr.sin_family = AF_INET;
    gcAddr.sin_port = htons(target_port);
    gcAddr.sin_addr.s_addr = inet_addr(target_ip.c_str());

    std::cout << "to   " << target_ip << ":" << target_port << std::endl;
    std::cout << "from " << inet_ntoa(locAddr.sin_addr) << ":" << local_port << std::endl;

    reading_thread = new std::thread(&MavlinkUDP::read_loop, this);
}
//----------------------------------------------------------------------------//
MavlinkUDP::~MavlinkUDP()
{
    close(sock);
}
//----------------------------------------------------------------------------//
bool MavlinkUDP::is_valid_ip(const char* ip)
{
    char* dst[INET_ADDRSTRLEN];
    return inet_pton(AF_INET, ip, dst) == 1;
}
//----------------------------------------------------------------------------//
void MavlinkUDP::append_listener(MavMessengerInterface* listener)
{
    if(listener != NULL)
        listeners.push_back(listener);
}
//----------------------------------------------------------------------------//
void MavlinkUDP::read_loop()
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
            throw std::logic_error("Unable to read from UDP socket.");

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
bool MavlinkUDP::send_message(mavlink_message_t &msg)
{
    uint8_t buf[512];
    uint16_t len, bytes_sent;
    len = mavlink_msg_to_send_buffer(buf, &msg);

    mutex.lock();
    bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
    mutex.unlock();

    return len == bytes_sent;
}
//----------------------------------------------------------------------------//
