#ifndef MAVLINK_SERIAL_H
#define MAVLINK_SERIAL_H

#include <mavkit/MavMessengerInterface.h>
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
    mavlink_status_t status;
    int fd, index, first_free;

    static const int buffer_length = 256;
    uint8_t read_buffer[buffer_length];
};

#endif
