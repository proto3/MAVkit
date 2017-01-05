#include <mavkit/MavlinkSerial.h>

#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

//----------------------------------------------------------------------------//
MavlinkSerial::MavlinkSerial(std::string port , int baudrate)
: reading_thread(NULL),
  buffering_thread(NULL)
{
    int result = pipe(storage_pipe);
    if(result == -1)
        throw std::logic_error(std::string("cannot open internal pipe for fast buffering: ") + strerror(errno));

    serial_fd = open(port.c_str(), O_RDWR);

    if(serial_fd == -1)
        throw std::logic_error(std::string("cannot open ") + port + ": " + strerror(errno));

    if(!isatty(serial_fd))
        throw std::logic_error(port + " is not a tty.");

    //TODO why to reset flags on new fd ?
    //aren't they clean ?
    fcntl(serial_fd, F_SETFL, 0);

    // Read file descriptor configuration
    struct termios config;
    if(tcgetattr(serial_fd, &config) < 0)
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
    config.c_cc[VMIN]  = 1;
    config.c_cc[VTIME] = 0;

    // Apply baudrate
    int baudrate_val;
    switch (baudrate)
    {
        case 9600:
            baudrate_val = B9600;
            break;
        case 19200:
            baudrate_val = B19200;
            break;
        case 38400:
            baudrate_val = B38400;
            break;
        case 57600:
            baudrate_val = B57600;
            break;
        case 115200:
            baudrate_val = B115200;
            break;
        case 230400:
            baudrate_val = B230400;
            break;
        case 460800:
            baudrate_val = B460800;
            break;
        case 921600:
            baudrate_val = B921600;
            break;
        default:
            throw std::logic_error("Cannot handle baudrate : " + std::to_string(baudrate));
            break;
    }

    if(cfsetispeed(&config, baudrate_val) < 0 || cfsetospeed(&config, baudrate_val) < 0)
        throw std::logic_error("Cannot set baudrate");

    // Finally, apply the configuration
    if(tcsetattr(serial_fd, TCSAFLUSH, &config) < 0)
        throw std::logic_error("Cannot set file descriptor configuration");

    std::cout << "Connected to " << port << " " << baudrate << std::endl;

    //Issue in linux kernel, tcflush is not effective immediately after serial port opening
    usleep(1000000);
    tcflush(serial_fd, TCIOFLUSH);
}
//----------------------------------------------------------------------------//
MavlinkSerial::~MavlinkSerial()
{
    close(serial_fd);
    close(storage_pipe[0]);
    close(storage_pipe[1]);
    //TODO stop bufferize thread
}
//----------------------------------------------------------------------------//
bool MavlinkSerial::is_valid_tty(const char* path)
{
    bool ret = true;
    int fd = open(path, O_RDWR | O_NOCTTY);

    if(fd == -1 || !isatty(fd))
        ret = false;

    close(fd);
    return ret;
}
//----------------------------------------------------------------------------//
void MavlinkSerial::append_listener(MavMessengerInterface* listener)
{
    if(listener != NULL)
        listeners.push_back(listener);
}
//----------------------------------------------------------------------------//
void MavlinkSerial::start()
{
    if(reading_thread == NULL && buffering_thread == NULL)
    {
        reading_thread = new std::thread(&MavlinkSerial::read_loop, this);
        buffering_thread = new std::thread(&MavlinkSerial::bufferize, this);
    }
}
//----------------------------------------------------------------------------//
void MavlinkSerial::join()
{
    reading_thread->join();
}
//----------------------------------------------------------------------------//
void MavlinkSerial::read_loop()
{
    size_t length = 256;
    uint8_t buffer[length];
    mavlink_status_t status;
    mavlink_message_t msg;
    while(true)
    {
        ssize_t nb_read = read(storage_pipe[0], buffer, length);
        if(nb_read == -1)
            throw std::logic_error("Unable to read from storage pipe.");

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
// Thread loop to move data from serial RX buffer to RAM as fast as possible
// so as to avoid buffer overflows if RX buffer is small.
void MavlinkSerial::bufferize()
{
    size_t length = 256;
    uint8_t buffer[length];
    while(true)
    {
        ssize_t nb_read = read(serial_fd, buffer, length);
        if(nb_read == -1)
            throw std::logic_error("Unable to read from serial port.");

        ssize_t nb_write = write(storage_pipe[1], buffer, nb_read);
        if(nb_write == -1 || nb_write < nb_read)
            throw std::logic_error("Unable to write to pipe."); //TODO figure out why the write fail instead of throwing error immediately
    }
}
//----------------------------------------------------------------------------//
bool MavlinkSerial::send_message(mavlink_message_t &msg)
{
    uint16_t length = mavlink_msg_get_send_buffer_length(&msg);
    uint8_t buffer[length];
    mavlink_msg_to_send_buffer(buffer, &msg);

    mutex.lock();
    int bytes_sent = write(serial_fd, buffer, length);
    mutex.unlock();

    return length == bytes_sent;
}
//----------------------------------------------------------------------------//
