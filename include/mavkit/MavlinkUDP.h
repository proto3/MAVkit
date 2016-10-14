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

    static bool is_valid_ip(const char* ip);
    bool send_message(mavlink_message_t &msg);
    bool receive_message(mavlink_message_t &msg);

private:
    int sock;
    struct sockaddr_in gcAddr;
    mavlink_status_t status;
};

#endif
