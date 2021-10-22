cmake_minimum_required(VERSION 3.21)

cmake_policy(SET CMP0074 NEW)

project(risc-v VERSION 0.0.1)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/riscv/")
