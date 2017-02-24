#include <mavkit/MavlinkLogReader.h>

#include <iostream>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

//----------------------------------------------------------------------------//
MavlinkLogReader::MavlinkLogReader(std::string log_file, float speed_multiplier, float target_time_sec)
: _speed_multiplier(speed_multiplier),
  reading_thread(NULL),
  _target_time_sec(target_time_sec)
{
    if(speed_multiplier <= 0.0)
    {
        throw std::logic_error(std::string("LogReader speed_multiplier cannot be negative or zero"));
    }

    std::string raw_file = log_file + ".raw";
    std::string ts_file = log_file + ".ts";

    raw_fd = open(raw_file.c_str(), O_RDONLY);
    ts_fd  = open(ts_file.c_str(),  O_RDONLY);

    if(raw_fd == -1)
        throw std::logic_error(std::string("cannot open ") + raw_file + ": " + strerror(errno));

    if(ts_fd == -1)
        throw std::logic_error(std::string("cannot open ") + ts_file + ": " + strerror(errno));

    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
}
//----------------------------------------------------------------------------//
MavlinkLogReader::~MavlinkLogReader()
{
    close(raw_fd);
    close(ts_fd);
}
//----------------------------------------------------------------------------//
bool MavlinkLogReader::is_valid_file(const char* path)
{
    std::string raw_file = path;
    raw_file += ".raw";
    return access(raw_file.c_str(), F_OK) != -1;
}
//----------------------------------------------------------------------------//
void MavlinkLogReader::append_listener(MavMessengerInterface* listener)
{
    if(listener != NULL)
        listeners.push_back(listener);
}
//----------------------------------------------------------------------------//
void MavlinkLogReader::start()
{
    if(reading_thread == NULL)
        reading_thread = new std::thread(&MavlinkLogReader::read_loop, this);
}
//----------------------------------------------------------------------------//
void MavlinkLogReader::join()
{
    reading_thread->join();
}
//----------------------------------------------------------------------------//
void MavlinkLogReader::read_loop()
{
    size_t length = 256;
    uint8_t buffer[length];
    mavlink_status_t status;
    mavlink_message_t msg;

    uint64_t time_offset = 0;

    while(true)
    {
        ssize_t nb_read = read(raw_fd, buffer, length);
        if(nb_read == -1)
            throw std::logic_error("Unable to read from file.");

        for(int i=0;i<nb_read;i++)
        {
            if(mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status))
            {
                bool drop_msg = false;
                if(ts_fd != -1)
                {
                    uint64_t msg_time;
                    uint8_t* p = (uint8_t*)&msg_time;
                    int nb_read_ts = read(ts_fd, p, sizeof(uint64_t));

                    if(nb_read_ts != sizeof(uint64_t))
                        throw std::logic_error("Unable to read from .ts file.");

                    uint64_t msg_time_mult = msg_time / _speed_multiplier;

                    struct timespec now;
                    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
                    uint64_t elapsed_time = (uint64_t)(now.tv_sec - start_time.tv_sec) * 1000000 + (now.tv_nsec - start_time.tv_nsec) / 1000 + time_offset;

                    if(msg_time > _target_time_sec * 1000000)
                    {
                        if(msg_time_mult > elapsed_time)
                            usleep(msg_time_mult - elapsed_time);
                    }
                    else
                    {
                        time_offset = msg_time_mult - elapsed_time + time_offset;
                        drop_msg = true;
                    }
                }
                if(!drop_msg)
                {
                    std::vector<MavMessengerInterface*>::iterator it = listeners.begin();
                    for(;it != listeners.end();++it)
                    {
                        (*it)->send_message(msg);
                    }
                }
            }
        }
    }
}
//----------------------------------------------------------------------------//
bool MavlinkLogReader::send_message(mavlink_message_t &msg)
{
    return false;
}
//----------------------------------------------------------------------------//
