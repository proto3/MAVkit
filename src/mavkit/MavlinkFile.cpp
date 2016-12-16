#include <mavkit/MavlinkFile.h>

#include <iostream>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

//----------------------------------------------------------------------------//
MavlinkFile::MavlinkFile(std::string file_path) :
	first_free(0),
	index(0)
{
    fd = open(file_path.c_str(), O_RDONLY);

    if(fd == -1)
        throw std::logic_error(std::string("cannot open ") + file_path + ": " + strerror(errno));

    std::cout << "Connected to " << file_path << std::endl;
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
bool MavlinkFile::receive_message(mavlink_message_t &msg)
{
    mavlink_status_t status;

    while(true)
    {
        if(index >= first_free)
        {
            index = 0;
            first_free = read(fd, read_buffer, buffer_length);

            if(first_free == -1)
                throw std::logic_error("Unable to read file.");
        }

        while(index < first_free)
        {
            uint8_t current_byte = read_buffer[index];
            index++;
            if(mavlink_parse_char(MAVLINK_COMM_0, current_byte, &msg, &status))
                return true;
        }
    }
}
//----------------------------------------------------------------------------//
bool MavlinkFile::send_message(mavlink_message_t &msg)
{
	// Should not be called
	return false;
}
//----------------------------------------------------------------------------//

