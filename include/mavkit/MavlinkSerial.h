#ifndef MAVLINK_SERIAL_H
#define MAVLINK_SERIAL_H

#include <mavkit/MavMessengerInterface.h>
#include <thread>
#include <mutex>
#include <vector>
#include <string>


class MavlinkSerial : public MavMessengerInterface
{
public:
    MavlinkSerial(std::string port, int baudrate);
    ~MavlinkSerial();

    static bool is_valid_tty(const char* path);
    bool send_message(mavlink_message_t &msg);
    void append_listener(MavMessengerInterface* listener);
    void start();
    void join();

private:
    std::thread *reading_thread;
    std::mutex mutex;
    std::vector<MavMessengerInterface*> listeners;
    void read_loop();

    int serial_fd;
    std::thread *buffering_thread;
    int storage_pipe[2];
    void bufferize();
};

#endif
