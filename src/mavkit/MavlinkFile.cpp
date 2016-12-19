#include <mavkit/MavlinkFile.h>

#include <iostream>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <dirent.h>
#include <boost/regex.hpp>

//----------------------------------------------------------------------------//
MavlinkFile::MavlinkFile(std::string in_file, std::string log_path)
{
    int log_id = create_log_files(log_path);

    std::string out_raw = log_path + "log" + std::to_string(log_id) + ".raw";
    std::string out_ts = log_path + "log" + std::to_string(log_id) + ".ts";

    in_fd = open(in_file.c_str(), O_RDONLY, 0666);

    out_raw_fd = open(out_raw.c_str(), O_WRONLY | O_CREAT, 0666);
    out_ts_fd  = open(out_ts.c_str(),  O_WRONLY | O_CREAT, 0666);

    if(in_fd == -1 && (out_raw_fd == -1 || out_ts_fd == -1))
        throw std::logic_error(std::string("cannot open ") + in_file + " or " + out_raw + " or " + out_ts + ": " + strerror(errno));

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    if(in_fd != -1)
    reading_thread = new std::thread(&MavlinkFile::read_loop, this);
}
//----------------------------------------------------------------------------//
MavlinkFile::~MavlinkFile()
{
    close(in_fd);
    close(out_raw_fd);
    close(out_ts_fd);
}
//----------------------------------------------------------------------------//
bool MavlinkFile::is_valid_file(const char* path)
{
    return access(path, F_OK) != -1;
}
//----------------------------------------------------------------------------//
void MavlinkFile::append_listener(MavMessengerInterface* listener)
{
    if(listener != NULL)
        listeners.push_back(listener);
}
//----------------------------------------------------------------------------//
void MavlinkFile::read_loop()
{
    size_t length = 256;
    uint8_t buffer[length];
    mavlink_status_t status;
    mavlink_message_t msg;
    while(true)
    {
        ssize_t nb_read = read(in_fd, buffer, length);
        if(nb_read == -1)
            throw std::logic_error("Unable to read from file.");

        for(int i=0;i<nb_read;i++)
        {
            if(mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status))
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
//----------------------------------------------------------------------------//
bool MavlinkFile::send_message(mavlink_message_t &msg)
{
    if(out_raw_fd == -1 || out_ts_fd == -1)
        return false;

    uint16_t length = mavlink_msg_get_send_buffer_length(&msg);
    uint8_t buffer[length];
    mavlink_msg_to_send_buffer(buffer, &msg);

    mutex.lock();

    int bytes_sent = write(out_raw_fd, buffer, length);
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    uint64_t delta_us = (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_nsec - start.tv_nsec) / 1000;
    write(out_ts_fd, (uint8_t*)&delta_us, 8);

    mutex.unlock();

    return length == bytes_sent;
}
//----------------------------------------------------------------------------//
int MavlinkFile::create_log_files(std::string path)
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
