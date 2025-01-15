#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <string>

// 初始化串口设备
bool initializeSerialPort(const std::string& port, int baudRate);

#endif // SERIAL_HPP
