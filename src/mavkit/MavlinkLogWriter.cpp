#include <mavkit/MavlinkLogWriter.h>

#include <iostream>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <dirent.h>
#include <boost/regex.hpp>

//----------------------------------------------------------------------------//
MavlinkLogWriter::MavlinkLogWriter(std::string log_path)
{
    int log_id = create_log_files(log_path);

    std::string out_raw = log_path + "log" + std::to_string(log_id) + ".raw";
    std::string out_ts = log_path + "log" + std::to_string(log_id) + ".ts";

    out_raw_fd = open(out_raw.c_str(), O_WRONLY | O_CREAT, 0666);
    out_ts_fd  = open(out_ts.c_str(),  O_WRONLY | O_CREAT, 0666);

    if(out_raw_fd == -1)
        throw std::logic_error(std::string("cannot open ") + out_raw + ": " + strerror(errno));

    if(out_ts_fd == -1)
        throw std::logic_error(std::string("cannot open ") + out_ts + ": " + strerror(errno));

    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
}
//----------------------------------------------------------------------------//
MavlinkLogWriter::~MavlinkLogWriter()
{
    close(out_raw_fd);
    close(out_ts_fd);
}
//----------------------------------------------------------------------------//
void MavlinkLogWriter::append_listener(MavMessengerInterface* listener)
{}
//----------------------------------------------------------------------------//
void MavlinkLogWriter::start()
{}
//----------------------------------------------------------------------------//
void MavlinkLogWriter::join()
{}
//----------------------------------------------------------------------------//
bool MavlinkLogWriter::send_message(mavlink_message_t &msg)
{
    uint16_t length = mavlink_msg_get_send_buffer_length(&msg);
    uint8_t buffer[length];
    mavlink_msg_to_send_buffer(buffer, &msg);

    mutex.lock();

    int bytes_sent = write(out_raw_fd, buffer, length);
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    uint64_t delta_us = (uint64_t)(now.tv_sec - start_time.tv_sec) * 1000000 + (now.tv_nsec - start_time.tv_nsec) / 1000;
    write(out_ts_fd, (uint8_t*)&delta_us, 8);

    mutex.unlock();

    return length == bytes_sent;
}
//----------------------------------------------------------------------------//
int MavlinkLogWriter::create_log_files(std::string path)
{
    if(path.back() != '/')
        path += "/";

    struct stat info;
    if(stat(path.c_str(), &info) == -1)
    {
        if(mkdir(path.c_str(), 0777) == -1)
        {
            throw std::logic_error("Cannot create " + path + " directory for log :" + strerror(errno));
        }
    }

    int max_id = 0;
    DIR *directory = opendir(path.c_str());
    std::string name_prefix = "log";
    if(directory != NULL)
    {
        boost::regex regex_raw(name_prefix + "[[:digit:]]+" + ".raw");
        boost::regex regex_time(name_prefix + "[[:digit:]]+" + ".ts");
        struct dirent *entry;
        //for each file name matching regex
        while((entry = readdir(directory)) != NULL)
        {
            //compute new max id
            std::string entry_name(entry->d_name);
            if(boost::regex_match(entry_name, regex_raw) || boost::regex_match(entry_name, regex_time))
            {
                entry_name.erase(0, name_prefix.length());
                max_id = std::max(max_id, std::stoi(entry_name));
            }
        }
        closedir(directory);
    }
    else
    {
        throw std::logic_error("Cannot open " + path + " directory :" + strerror(errno));
    }

    return max_id + 1;
}
//----------------------------------------------------------------------------//
