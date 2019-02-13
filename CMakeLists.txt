cmake_minimum_required(VERSION 2.6)
project(CC2500_SPI)

add_executable(
        receiver common.c receiver.c)

add_executable(
        transmitter common.c transmitter.c)

target_link_libraries(receiver wiringPi)
target_link_libraries(transmitter wiringPi)

