#ifndef MAVLINK_FILE_H
#define MAVLINK_FILE_H

#include <mavkit/MavMessengerInterface.h>
#include <thread>
#include <vector>
#include <string>

class MavlinkFile : public MavMessengerInterface
{
public:
    MavlinkFile(std::string file_path);
    ~MavlinkFile();

    static bool is_valid_file(const char* path);
    bool send_message(mavlink_message_t &msg);
    void append_listener(MavMessengerInterface* listener);

private:
    std::thread *reading_thread;
    std::vector<MavMessengerInterface*> listeners;
    void read_loop();

    int fd;
};

#endif
