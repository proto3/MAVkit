#ifndef MAVLINK_FILE_H
#define MAVLINK_FILE_H

#include <mavkit/MavMessengerInterface.h>
#include <string>

class MavlinkFile : public MavMessengerInterface
{
public:
    MavlinkFile(std::string file_path);
    ~MavlinkFile();

    static bool is_valid_file(const char* path);
    bool send_message(mavlink_message_t &msg);
    bool receive_message(mavlink_message_t &msg);

private:
    mavlink_status_t status;
    int fd, index, first_free;

    static const int buffer_length = 256;
    uint8_t read_buffer[buffer_length];
};

#endif
