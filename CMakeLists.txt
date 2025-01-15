cmake_minimum_required(VERSION 3.10)
project(PLS_DRIVER)

set(CMAKE_CXX_STANDARD 17)

# 添加 include 目录
include_directories(${PROJECT_SOURCE_DIR}/include)
# 添加 libmodbus 的 include 路径
include_directories(/usr/include/modbus)

# 查找 libmodbus
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMODBUS REQUIRED libmodbus)

# 添加源文件
add_executable(pls_driver src/main.cpp src/modbus_config.cpp src/serial.cpp)

# 链接 libmodbus
target_link_libraries(pls_driver ${LIBMODBUS_LIBRARIES})
