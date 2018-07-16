#ifndef MAVLINK_TCP_H
#define MAVLINK_TCP_H

#include <mavkit/MavMessengerInterface.h>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <arpa/inet.h>

class MavlinkTCP : public MavMessengerInterface
{
public:
    //Client constructor
    MavlinkTCP(std::string target_ip, int target_port);
    //Server constructor
    MavlinkTCP(int local_port);
    ~MavlinkTCP();

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

    bool connect_client();
    void connect_server();
    void reconnect();
    bool is_server;
    bool connected;
    std::string _target_ip;
    int _target_port;
    int csock;
    int sock;
    struct sockaddr_in gcAddr;
};

#endif
