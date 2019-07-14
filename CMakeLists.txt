cmake_minimum_required(VERSION 3.13)
project(RISCV_Simulator)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "-O3")

add_executable(RISCV_Simulator main.cpp src/Memory.cpp src/Register.cpp src/Instruction.cpp src/Buffer.cpp src/Parallel.cpp src/Pipeline/Fetch.cpp src/Pipeline/Execute.cpp src/Pipeline/Decode.cpp)