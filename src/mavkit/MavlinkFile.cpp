#include <mavkit/MavlinkFile.h>

#include <iostream>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

//----------------------------------------------------------------------------//
MavlinkFile::MavlinkFile(std::string file_path)
{
    fd = open(file_path.c_str(), O_RDONLY);

    if(fd == -1)
        throw std::logic_error(std::string("cannot open ") + file_path + ": " + strerror(errno));

    std::cout << "Connected to " << file_path << std::endl;

    reading_thread = new std::thread(&MavlinkFile::read_loop, this);
}
//----------------------------------------------------------------------------//
MavlinkFile::~MavlinkFile()
{
    close(fd);
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
        ssize_t nb_read = read(fd, buffer, length);
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
    // Should not be called
    return true;
}
//----------------------------------------------------------------------------//
