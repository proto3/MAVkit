#ifndef MAVLINK_FILE_H
#define MAVLINK_FILE_H

#include <mavkit/MavMessengerInterface.h>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

class MavlinkFile : public MavMessengerInterface
{
public:
    MavlinkFile(std::string in_file, std::string log_path);
    ~MavlinkFile();

    static bool is_valid_file(const char* path);
    bool send_message(mavlink_message_t &msg);
    void append_listener(MavMessengerInterface* listener);

private:
    std::thread *reading_thread;
    std::mutex mutex;
    std::vector<MavMessengerInterface*> listeners;
    void read_loop();

    int create_log_files(std::string path);
    int in_fd, out_raw_fd, out_ts_fd;
    struct timespec start;
};

#endif
