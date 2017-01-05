#ifndef MAVLINK_UDP_H
#define MAVLINK_UDP_H

#include <mavkit/MavMessengerInterface.h>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <arpa/inet.h>

class MavlinkUDP : public MavMessengerInterface
{
public:
    //Client constructor
    MavlinkUDP(std::string target_ip, int target_port);
    //Server constructor
    MavlinkUDP(int local_port);
    ~MavlinkUDP();

    static bool is_valid_ip(const char* ip);
    bool send_message(mavlink_message_t &msg);
    void append_listener(MavMessengerInterface* listener);
    void start();
    void join();

private:
    std::thread *reading_thread;
    std::mutex mutex;
    std::vector<MavMessengerInterface*> listeners;
    void read_loop();

    int sock;
    struct sockaddr_in gcAddr;
    std::thread *buffering_thread;
    int storage_pipe[2];
    void bufferize();
};

#endif
