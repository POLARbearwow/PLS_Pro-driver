#ifndef MODBUS_CONFIG_HPP
#define MODBUS_CONFIG_HPP

#include <modbus.h>
#include <string>

// 默认配置常量
const int DEFAULT_BAUDRATE = 9600;
const char DEFAULT_PARITY = 'N';
const int DEFAULT_DATA_BITS = 8;
const int DEFAULT_STOP_BITS = 1;
const int DEFAULT_SLAVE_ADDRESS = 1;

// 创建和关闭 Modbus 连接的函数声明
modbus_t* createModbusConnection(const std::string& port, int baudRate = DEFAULT_BAUDRATE, int slaveAddress = DEFAULT_SLAVE_ADDRESS);
void closeModbusConnection(modbus_t* mb);

#endif // MODBUS_CONFIG_HPP
