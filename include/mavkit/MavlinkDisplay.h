#ifndef MAVLINK_DISPLAY_H
#define MAVLINK_DISPLAY_H

#include <mavkit/MavMessengerInterface.h>
#include <mutex>
#include <string>

class MavlinkDisplay : public MavMessengerInterface
{
public:
    MavlinkDisplay();
    ~MavlinkDisplay();

    bool send_message(mavlink_message_t &msg);
    void append_listener(MavMessengerInterface* listener);
    void start();
    void join();

private:
    std::mutex mutex;
    std::string _name;
};

#endif
