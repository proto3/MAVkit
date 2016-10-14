#ifndef MAV_MESSENGER_INTERFACE_H
#define MAV_MESSENGER_INTERFACE_H

#include <mavlink_headers/ardupilotmega/mavlink.h>

class MavMessengerInterface
{
public:
    virtual ~MavMessengerInterface(){};

    //return true if the message is sent
    //false is an has error occured
    virtual bool send_message(mavlink_message_t &msg) = 0;

    //return true when a message is received
    //false if no message
    virtual bool receive_message(mavlink_message_t &msg) = 0;
};

#endif
