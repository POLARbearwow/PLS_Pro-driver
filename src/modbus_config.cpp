#include "modbus_config.hpp"
#include <stdexcept>
#include <iostream>

modbus_t* createModbusConnection(const std::string& port, int baudRate, int slaveAddress) {
    // 创建 RTU 模式的 Modbus 连接
    modbus_t* mb = modbus_new_rtu(port.c_str(), baudRate, DEFAULT_PARITY, DEFAULT_DATA_BITS, DEFAULT_STOP_BITS);
    if (mb == nullptr) {
        std::cerr << "Unable to create Modbus RTU connection!" << std::endl;
        throw std::runtime_error("Failed to create Modbus connection");
    }

    // 设置从机地址
    if (modbus_set_slave(mb, slaveAddress) == -1) {
        modbus_free(mb);
        std::cerr << "Failed to set slave address!" << std::endl;
        throw std::runtime_error("Invalid slave address");
    }

    // 打开连接
    if (modbus_connect(mb) == -1) {
        modbus_free(mb);
        std::cerr << "Failed to connect to Modbus slave!" << std::endl;
        throw std::runtime_error("Connection failed");
    }

    return mb;
}

void closeModbusConnection(modbus_t* mb) {
    if (mb) {
        modbus_close(mb);
        modbus_free(mb);
    }
}
