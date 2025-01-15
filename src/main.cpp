#include "modbus_config.hpp"
#include "serial.hpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cstdint>

// Helper function to print and parse Modbus response
void processResponse(const uint8_t* response, int length) {
    if (length < 5) { // 最少需要5字节（地址 + 功能码 + 数据长度 + 数据 + CRC）
        std::cerr << "Invalid response length: " << length << std::endl;
        return;
    }

    // 打印原始响应
    std::cout << "Raw Response: ";
    for (int i = 0; i < length; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(response[i]) << " ";
    }
    std::cout << std::endl;

    // 提取返回的有效数据（以例子 "19 03 04 00 00 3D 9B 33 09" 为例）
    if (response[1] == 0x03 && response[2] == 0x04) { // 确保是功能码 0x03 且数据长度为 4
        uint32_t data = (response[3] << 24) | (response[4] << 16) | (response[5] << 8) | response[6];
        double result = data * 0.0001; // 根据协议单位转换为米
        std::cout << "Parsed Distance (m): " << result << "m" << std::endl;
    } else {
        std::cout << "Unexpected response format." << std::endl;
    }
}

int main() {
    try {
        const std::string port = "/dev/ttyUSB0"; // 根据实际设备调整
        int baudRate = 115200;
        int slaveAddress = 1;

        // 初始化串口
        if (!initializeSerialPort(port, baudRate)) {
            std::cerr << "Failed to initialize serial port!" << std::endl;
            return -1;
        }

        // 创建 Modbus 连接
        modbus_t* mb = createModbusConnection(port, baudRate, slaveAddress);
 
        // 无限循环发送请求帧
        while (true) {
            //查看频率
            // uint8_t request[] = {0x01, 0x03, 0x00, 0x07, 0x00, 0x01, 0x35, 0xCB}; // 
            //设置频率
            // uint8_t request[] = {0x01, 0x06, 0x00, 0x07, 0x00, 0x04, 0x35, 0xCB}; // 0004 30HZ
            // 测量距离
            uint8_t request[] = {0x01, 0x03, 0x00, 0x02, 0x00, 0x02, 0x65, 0xCB}; // 
            if (modbus_send_raw_request(mb, request, sizeof(request)) == -1) {
                std::cerr << "Failed to send custom frame!" << std::endl;
            } else {
                std::cout << "Custom frame sent successfully." << std::endl;

                // 接收响应
                uint8_t response[256]; // 用于存储响应数据
                int response_length = modbus_receive_confirmation(mb, response);
                if (response_length == -1) {
                    std::cerr << "Failed to receive response!" << std::endl;
                } else {
                    // 处理并解析响应数据
                    processResponse(response, response_length);
                }
            }

            // 延时 1 秒，防止过快发送请求
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
        }

        // 关闭连接（理论上不会到达这里，因为无限循环）
        closeModbusConnection(mb);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
