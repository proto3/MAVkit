#ifndef MAVLINK_UDP_H
#define MAVLINK_UDP_H

#include <mavkit/MavMessengerInterface.h>
#include <string>
#include <arpa/inet.h>

class MavlinkUDP : public MavMessengerInterface
{
public:
    MavlinkUDP(std::string target_ip, int target_port, int local_port);
    ~MavlinkUDP();

    //return true if the message is sent
    //false is an has error occured
    bool send_message(mavlink_message_t &msg);

    //return true when a message is received
    //false if no message
    bool receive_message(mavlink_message_t &msg);

private:
    int sock;
    struct sockaddr_in gcAddr;
    mavlink_status_t status;
};

#endif
