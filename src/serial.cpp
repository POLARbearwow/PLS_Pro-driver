#include "serial.hpp"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>

bool initializeSerialPort(const std::string& port, int baudRate) {
    int fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd == -1) {
        std::cerr << "Failed to open serial port: " << port << std::endl;
        return false;
    }

    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        std::cerr << "Failed to get serial port attributes!" << std::endl;
        close(fd);
        return false;
    }

    cfsetospeed(&tty, baudRate);
    cfsetispeed(&tty, baudRate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    tty.c_iflag &= ~IGNBRK;                    // disable break processing
    tty.c_lflag=0;                             // no signaling chars, no echo, no canonical processing
    tty.c_oflag = 0;                           // no remapping, no delays
    tty.c_cc[VMIN]  = 1;                       // read doesn't block
    tty.c_cc[VTIME] = 1;                       // 0.1 seconds read timeout

    tty.c_cflag |= (CLOCAL | CREAD);           // ignore modem controls, enable reading
    tty.c_cflag &= ~(PARENB | PARODD);         // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Failed to set serial port attributes!" << std::endl;
        close(fd);
        return false;
    }

    close(fd);
    return true;
}
