#ifndef MAVLINK_LOG_READER_H
#define MAVLINK_LOG_READER_H

#include <mavkit/MavMessengerInterface.h>
#include <thread>
#include <vector>
#include <string>

class MavlinkLogReader : public MavMessengerInterface
{
public:
    MavlinkLogReader(std::string log_file);
    ~MavlinkLogReader();

    static bool is_valid_file(const char* path);
    bool send_message(mavlink_message_t &msg);
    void append_listener(MavMessengerInterface* listener);
    void join();

private:
    std::thread *reading_thread;
    std::vector<MavMessengerInterface*> listeners;
    void read_loop();

    int create_log_files(std::string path);
    int raw_fd, ts_fd;
    struct timespec start;
};

#endif
