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

    //add a MavMessengerInterface to the listener list
    //listeners are sent any message that is received by this MavMessengerInterface
    virtual void append_listener(MavMessengerInterface* listener) = 0;

    //start internal thread(s) that read interface continuously
    //no effect if implementation has no thread
    virtual void start() = 0;

    //join internal thread
    //return immediately if implementation has no thread (ex: logger)
    virtual void join() = 0;
};

#endif
