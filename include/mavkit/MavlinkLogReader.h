#ifndef MAVLINK_LOG_READER_H
#define MAVLINK_LOG_READER_H

#include <mavkit/MavMessengerInterface.h>
#include <thread>
#include <vector>
#include <string>

class MavlinkLogReader : public MavMessengerInterface
{
public:
    MavlinkLogReader(std::string log_file, float speed_multiplier, float target_time_sec);
    ~MavlinkLogReader();

    static bool is_valid_file(const char* path);
    bool send_message(mavlink_message_t &msg);
    void append_listener(MavMessengerInterface* listener);
    void start();
    void join();

private:
    std::thread *reading_thread;
    std::vector<MavMessengerInterface*> listeners;
    void read_loop();

    const float _speed_multiplier;
    int create_log_files(std::string path);
    int raw_fd, ts_fd;
    struct timespec start_time;
    float _target_time_sec;
};

#endif
