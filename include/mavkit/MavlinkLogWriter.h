#ifndef MAVLINK_LOG_WRITER_H
#define MAVLINK_LOG_WRITER_H

#include <mavkit/MavMessengerInterface.h>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

class MavlinkLogWriter : public MavMessengerInterface
{
public:
    MavlinkLogWriter(std::string log_path);
    ~MavlinkLogWriter();

    bool send_message(mavlink_message_t &msg);
    void append_listener(MavMessengerInterface* listener);
    void start();
    void join();

private:
    std::mutex mutex;

    int create_log_files(std::string path);
    int in_fd, out_raw_fd, out_ts_fd;
    struct timespec start_time;
};

#endif
