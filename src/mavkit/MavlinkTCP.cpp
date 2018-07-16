#include <mavkit/MavlinkTCP.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>

//----------------------------------------------------------------------------//
MavlinkTCP::MavlinkTCP(std::string target_ip, int target_port)
: is_server(false), reading_thread(NULL), _target_ip(target_ip), _target_port(target_port)
{
    if(_target_port < 0 || _target_port > 65535)
        throw std::logic_error(std::string("Port outside range."));

    if(!connect_client())
    {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
    }

    std::cout << "to " << _target_ip << ":" << _target_port << std::endl;

    connected = true;
    reading_thread = new std::thread(&MavlinkTCP::read_loop, this);
}
//----------------------------------------------------------------------------//
MavlinkTCP::MavlinkTCP(int local_port)
: is_server(true), reading_thread(NULL)
{
    if(local_port < 0 || local_port > 65535)
        throw std::logic_error(std::string("Port outside range."));

    struct sockaddr_in locAddr;
    memset(&locAddr, 0, sizeof(struct sockaddr_in));
    locAddr.sin_family = AF_INET;
    locAddr.sin_port = htons(local_port);
    locAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to local_port
    csock = socket(AF_INET, SOCK_STREAM, 0);
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

    connect_server();

    std::cout << "from " << inet_ntoa(locAddr.sin_addr) << ":" << local_port << std::endl;

    connected = true;
    reading_thread = new std::thread(&MavlinkTCP::read_loop, this);
}
//----------------------------------------------------------------------------//
void MavlinkTCP::connect_server()
{
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
}
//----------------------------------------------------------------------------//
bool MavlinkTCP::connect_client()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&gcAddr, 0, sizeof(struct sockaddr_in));
    gcAddr.sin_family = AF_INET;
    gcAddr.sin_port = htons(_target_port);
    gcAddr.sin_addr.s_addr = inet_addr(_target_ip.c_str());

    return connect(sock, (struct sockaddr*) &gcAddr, sizeof(struct sockaddr)) == 0;
}
//----------------------------------------------------------------------------//
void MavlinkTCP::reconnect()
{
    if(is_server)
    {
        std::cout << "[TCP] Disconnected.\n[TCP] server listening for connection..."<< std::endl;
        connect_server();
    }
    else
    {
        std::cout << "[TCP] Disconnected."<< std::endl;
        while(!connect_client())
        {
            std::cout << "[TCP] Connection retry..."<< std::endl;
            usleep(1000000);
        }
    }
    connected = true;
    std::cout << "[TCP] Connected."<< std::endl;
}
//----------------------------------------------------------------------------//
MavlinkTCP::~MavlinkTCP()
{
    if(is_server)
    {
        close(csock);
    }
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
        if(!connected)
        {
            close(sock);
            reconnect();
        }

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
    ssize_t bytes_sent = 0;
    uint16_t len;
    len = mavlink_msg_to_send_buffer(buf, &msg);

    mutex.lock();
    if(connected)
    {
        bytes_sent = sendto(sock, buf, len, MSG_NOSIGNAL, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
        connected = (bytes_sent != -1);
    }
    mutex.unlock();

    return len == bytes_sent;
}
//----------------------------------------------------------------------------//
