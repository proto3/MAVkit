#ifndef MAVLINK_SERIAL_H
#define MAVLINK_SERIAL_H

#include <mavkit/MavMessengerInterface.h>
#include <string>

// The maximum packet length is 263 bytes for full payload.
#define BUFFER_LENGTH 512

class MavlinkSerial : public MavMessengerInterface
{
public:
    MavlinkSerial(std::string port, int baudrate);
    ~MavlinkSerial();

    static bool is_valid_file(const char* path);
    bool send_message(mavlink_message_t &msg);
    bool receive_message(mavlink_message_t &msg);

private:
    mavlink_status_t status;
    int fd, first_byte, nb_bytes;
    uint8_t read_buffer[BUFFER_LENGTH];

    void fill_read_buffer();
    bool parse_buffer(mavlink_message_t &message);
    // bool write_buffer(const char* buffer, size_t length);
};

#endif
