#ifndef MAVLINK_SERIAL_H
#define MAVLINK_SERIAL_H

#include <mavkit/MavMessengerInterface.h>
#include <thread>
#include <string>

class MavlinkSerial : public MavMessengerInterface
{
public:
    MavlinkSerial(std::string port, int baudrate);
    ~MavlinkSerial();

    static bool is_valid_tty(const char* path);
    bool send_message(mavlink_message_t &msg);
    bool receive_message(mavlink_message_t &msg);

private:
    int serial_fd, storage_pipe[2];
    std::thread *buffering_thread;

    void bufferize();
};

#endif
