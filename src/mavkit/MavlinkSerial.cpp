#include <mavkit/MavlinkSerial.h>

#include <iostream>
#include <stdexcept>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

//----------------------------------------------------------------------------//
MavlinkSerial::MavlinkSerial(std::string port , int baudrate)
: first_free(0),
  index(0)
{
    fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    if(fd == -1)
        throw std::logic_error(std::string("cannot open ") + port + ": " + strerror(errno));

    if(!isatty(fd))
        throw std::logic_error(port + " is not a tty.");

    //TODO why to reset flags on new fd ?
    //aren't they clean ?
    fcntl(fd, F_SETFL, 0);

    // Read file descriptor configuration
    struct termios config;
    if(tcgetattr(fd, &config) < 0)
        throw std::logic_error(std::string("Cannot read file descriptor configuration: ") + strerror(errno));

    // Input flags - Turn off input processing
    // convert break to null byte, no CR to NL translation,
    // no NL to CR translation, don't mark parity errors or breaks
    // no input parity check, don't strip high bit off,
    // no XON/XOFF software flow control
    config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);

    // Output flags - Turn off output processing
    // no CR to NL translation, no NL to CR-NL translation,
    // no NL to CR translation, no column 0 CR suppression,
    // no Ctrl-D suppression, no fill characters, no case mapping,
    // no local output processing
    config.c_oflag &= ~(OCRNL | ONLCR | ONLRET | ONOCR | OFILL | OPOST);

    #ifdef OLCUC
        config.c_oflag &= ~OLCUC;
    #endif

    #ifdef ONOEOT
        config.c_oflag &= ~ONOEOT;
    #endif

    // No line processing:
    // echo off, echo newline off, canonical mode off,
    // extended input processing off, signal chars off
    config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

    // Turn off character processing
    // clear current char size mask, no parity checking,
    // no output processing, force 8 bit input
    config.c_cflag &= ~(CSIZE | PARENB);
    config.c_cflag |= CS8;

    // One input byte is enough to return from read()
    // Inter-character timer off
    config.c_cc[VMIN]  = 255;
    config.c_cc[VTIME] = 1;

    // Get the current options for the port
    ////struct termios options;
    ////tcgetattr(fd, &options);

    // Apply baudrate
    switch (baudrate)
    {
        case 9600:
            baudrate = B9600;
            break;
        case 19200:
            baudrate = B19200;
            break;
        case 38400:
            baudrate = B38400;
            break;
        case 57600:
            baudrate = B57600;
            break;
        case 115200:
            baudrate = B115200;
            break;
        case 230400:
            baudrate = B230400;
            break;
        case 460800:
            baudrate = B460800;
            break;
        case 921600:
            baudrate = B921600;
            break;
        default:
            baudrate = B115200;
            break;
    }

    if(cfsetispeed(&config, baudrate) < 0 || cfsetospeed(&config, baudrate) < 0)
        throw std::logic_error("Cannot set baudrate");

    // Finally, apply the configuration
    if(tcsetattr(fd, TCSAFLUSH, &config) < 0)
        throw std::logic_error("Cannot set file descriptor configuration");

    std::cout << "Connected to " << port << std::endl;
}
//----------------------------------------------------------------------------//
MavlinkSerial::~MavlinkSerial()
{
    close(fd);
}
//----------------------------------------------------------------------------//
bool MavlinkSerial::is_valid_tty(const char* path)
{
	std::string cPath = path;
    return cPath.find("/dev/tty") != std::string::npos;
}
//----------------------------------------------------------------------------//
bool MavlinkSerial::receive_message(mavlink_message_t &msg)
{
    mavlink_status_t status;

    while(true)
    {
        if(index >= first_free)
        {
            index = 0;
            first_free = read(fd, read_buffer, buffer_length);

            if(first_free == -1)
                throw std::logic_error("Unable to read on serial port.");
        }

        char str[8];
        int n = 0;
        std::string a;
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
bool MavlinkSerial::send_message(mavlink_message_t &msg)
{
    uint8_t buf[buffer_length];
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

    int bytes_sent = write(fd, buf, len);

    return len == bytes_sent;
}
//----------------------------------------------------------------------------//
